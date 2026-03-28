/*************************************************************
 * 1. Name:
 *      Demo
 * 2. Assignment Name:
 *      Lab 07: Orbit Simulator
 * 3. Assignment Description:
 *      Simulate satellites orbiting the earth
 * 4. What was the hardest part? Be as specific as possible.
 *      I decided to be pedantic and go with Earth's gravitational constant,
 *which led to a lot of math, so implementing that was tricky. Getting the stars
 *to twinkle independently was tricky too, but I'm pretty pround of my solution.
 * 5. How long did it take for you to complete the assignment?
 *      12 hours between the two of us.
 *****************************************************************/

#include "constants.h"
#include <algorithm>
#include <cassert> // for ASSERT
#include <cmath>   // for sqrt()
#include <memory>
#include <utility>
#include <vector>

#include "position.h" // for POINT
#include "projectile.h"
#include "satellite.h" // for Sattelite
#include "ship.h"
#include "test.h"
#include "uiDraw.h"     // for RANDOM and DRAW*
#include "uiInteract.h" // for INTERFACE
#include "velocity.h"   // for Velocity
using namespace std;
namespace
{
   constexpr double EARTH_COLLISION_RADIUS_PIXELS = 50.0;

   bool isCollidingWithEarth(const Entity& entity)
   {
      const double entityRadiusPixels = entity.getCollisionRadiusPixels();
      if (entityRadiusPixels <= 0.0)
         return false;

      const double xMeters = entity.getPosition().getMetersX();
      const double yMeters = entity.getPosition().getMetersY();
      const double distanceMeters = std::sqrt(xMeters * xMeters +
                                              yMeters * yMeters);
      const double collisionRadiusMeters = (EARTH_COLLISION_RADIUS_PIXELS +
                                            entityRadiusPixels) *
                                           entity.getPosition().getZoom();
      return distanceMeters <= collisionRadiusMeters;
   }
   bool isCollisionImmune(const Entity& entity)
   {
      const Fragment* fragment = dynamic_cast<const Fragment*>(&entity);
      if (fragment != nullptr && fragment->isCollisionImmune())
         return true;

      const SatellitePart* part = dynamic_cast<const SatellitePart*>(&entity);
      return part != nullptr && part->isCollisionImmune();
   }
   bool areColliding(const Entity& lhs, const Entity& rhs)
   {
      if (isCollisionImmune(lhs) || isCollisionImmune(rhs))
         return false;
      const double lhsRadiusPixels = lhs.getCollisionRadiusPixels();
      const double rhsRadiusPixels = rhs.getCollisionRadiusPixels();
      if (lhsRadiusPixels <= 0.0 || rhsRadiusPixels <= 0.0)
         return false;

      const double collisionRadiusMeters = (lhsRadiusPixels + rhsRadiusPixels) *
                                           lhs.getPosition().getZoom();
      const double distanceMeters = computeDistance(lhs.getPosition(),
                                                    rhs.getPosition());
      return distanceMeters <= collisionRadiusMeters;
   }

   void breakEntity(const Entity& entity,
                    std::vector<std::unique_ptr<Entity>>& spawnedDebris)
   {
      const Satellite* satellite = dynamic_cast<const Satellite*>(&entity);
      if (satellite != nullptr)
      {
         satellite->createBreakupDebris(spawnedDebris);
         return;
      }

      const SatellitePart* part = dynamic_cast<const SatellitePart*>(&entity);
      if (part != nullptr)
      {
         part->createBreakupDebris(spawnedDebris);
         return;
      }
   }

   bool shouldRemoveEntity(const std::unique_ptr<Entity>& entity)
   {
      const Projectile* projectile = dynamic_cast<const Projectile*>(
         entity.get());
      if (projectile != nullptr && projectile->isExpired())
         return true;

      const Fragment* fragment = dynamic_cast<const Fragment*>(entity.get());
      return fragment != nullptr && fragment->isExpired();
   }
} // namespace

/*************************************************************************
 * Demo
 * Test structure to capture the LM that will move around the screen
 *************************************************************************/
class Demo
{
public:
   Demo(const Position& ptUpperRight)
       : ptUpperRight(ptUpperRight), angleEarth(0.0), ship(nullptr)
   {
      constexpr double TWO_PI = 6.28318530717958647692;
      constexpr int GPS_SATELLITE_COUNT = 10;

      entities.push_back(std::unique_ptr<Entity>(new Hubble()));
      entities.push_back(std::unique_ptr<Entity>(new Sputnik()));
      entities.push_back(std::unique_ptr<Entity>(new Starlink()));
      entities.push_back(std::unique_ptr<Entity>(new CrewDragon()));

      for (int i = 0; i < GPS_SATELLITE_COUNT; ++i)
      {
         const double theta = (TWO_PI * static_cast<double>(i)) /
                              GPS_SATELLITE_COUNT;
         entities.push_back(std::unique_ptr<Entity>(new GPS(theta)));
      }

      Position shipPosition;
      shipPosition.setPixelsX(-450.0);
      shipPosition.setPixelsY(450.0);
      Ship* managedShip = new Ship(shipPosition, Velocity(0.0, -2000.0),
                                   Angle(0.0));
      ship = managedShip;
      entities.push_back(std::unique_ptr<Entity>(managedShip));

      // Generate stars

      for (int i = 0; i < NUM_STARS; i++)
      {
         Star star;

         star.position.setPixelsX(ptUpperRight.getPixelsX() *
                                  random(-0.5, 0.5));
         star.position.setPixelsY(ptUpperRight.getPixelsY() *
                                  random(-0.5, 0.5));

         // Random starting phase
         star.phase = static_cast<unsigned char>(random(0, 255));

         // Random twinkle speed
         // I tried a couple different ranges, 1-4 did best, I think it looks
         // beautiful at this speed!
         star.speed = static_cast<unsigned char>(random(1, 4));

         stars.push_back(star);
      }
   }
   Position ptUpperRight;
   double angleEarth;
   Ship* ship;
   std::vector<std::unique_ptr<Entity>> entities;
   struct Star
   {
      Position position;
      unsigned char
         phase; // char allows 0–255, giving 255 phases the star could be in.
      unsigned char speed; // likewise, the twinkle speed can be any of 255
                           // values, allowing for a lot of variation.
      // I'm not using all of them at the moment though.
      // char is also just a small data type that won't take up much space,
      // which is good, because there will be a lot of stars, and they're
      // already slowing down the simulation a little.
   };
   vector<Star> stars;
};

/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/
void callBack(const Interface* pUI, void* p)
{
   // the first step is to cast the void pointer into a game object. This
   // is the first step of every single callback function in OpenGL.
   Demo* pDemo = (Demo*)p;

   //
   // accept input

   if (pDemo->ship != nullptr)
   {
      if (pUI->isLeft())
         pDemo->ship->turnLeft();
      if (pUI->isRight())
         pDemo->ship->turnRight();

      const bool isThrusting = pUI->isDown();
      pDemo->ship->setThrusting(isThrusting);
      if (isThrusting)
         pDemo->ship->thrustForward(SIM_SECONDS_PER_FRAME);
      if (pUI->isSpace())
      {
         pDemo->entities.push_back(std::unique_ptr<Entity>(
            new Projectile(Projectile::createFromShip(*pDemo->ship))));
      }
   }

   //
   // perform all the game logic
   //

   // Rotate Earth at (approximately) the sidereal-day rate so a GEO satellite
   // remains over the same point on the Earth's surface.
   constexpr double TWO_PI = 6.28318530717958647692;
   constexpr double SIDEREAL_DAY_SECONDS = 86164.0905;
   const double earthRotationRate = TWO_PI / SIDEREAL_DAY_SECONDS; // rad/sec

   // NOTE: The rotate() helper in uiDraw.cpp applies a clockwise rotation for a
   // positive angle, so we subtract here to make Earth rotate the expected way.
   pDemo->angleEarth -= earthRotationRate * SIM_SECONDS_PER_FRAME;

   // Keep the angle bounded to avoid numeric blow-up over long runs.
   pDemo->angleEarth = std::fmod(pDemo->angleEarth, TWO_PI);
   if (pDemo->angleEarth < 0.0)
      pDemo->angleEarth += TWO_PI;

   for (auto& star : pDemo->stars)
   {
      star.phase += star.speed; // unsigned char auto-wraps at 255
   }
   for (auto& entity : pDemo->entities)
      entity->update(SIM_SECONDS_PER_FRAME);
   std::vector<bool> removeEntity(pDemo->entities.size(), false);
   std::vector<std::unique_ptr<Entity>> spawnedDebris;

   for (size_t i = 0; i < pDemo->entities.size(); ++i)
   {
      for (size_t j = i + 1; j < pDemo->entities.size(); ++j)
      {
         if (removeEntity[i] || removeEntity[j])
            continue;

         if (!areColliding(*pDemo->entities[i], *pDemo->entities[j]))
            continue;

         breakEntity(*pDemo->entities[i], spawnedDebris);
         breakEntity(*pDemo->entities[j], spawnedDebris);
         removeEntity[i] = true;
         removeEntity[j] = true;
      }
   }

   for (size_t i = 0; i < pDemo->entities.size(); ++i)
   {
      if (removeEntity[i])
         continue;

      if (!isCollidingWithEarth(*pDemo->entities[i]))
         continue;

      breakEntity(*pDemo->entities[i], spawnedDebris);
      removeEntity[i] = true;
   }

   std::vector<std::unique_ptr<Entity>> survivors;
   survivors.reserve(pDemo->entities.size());
   for (size_t i = 0; i < pDemo->entities.size(); ++i)
   {
      if (pDemo->entities[i].get() == pDemo->ship &&
          (removeEntity[i] || shouldRemoveEntity(pDemo->entities[i])))
         pDemo->ship = nullptr;
      if (!removeEntity[i] && !shouldRemoveEntity(pDemo->entities[i]))
         survivors.push_back(std::move(pDemo->entities[i]));
   }

   for (std::unique_ptr<Entity>& debris : spawnedDebris)
      survivors.push_back(std::move(debris));

   pDemo->entities = std::move(survivors);
   //
   // draw everything
   //

   Position pt;
   ogstream gout(pt);

   // draw entities
   for (const auto& entity : pDemo->entities)
      entity->draw(gout);

   // draw stars
   for (const auto& star : pDemo->stars)
   {
      gout.drawStar(star.position, star.phase);
   }

   // draw the earth
   pt.setMeters(0.0, 0.0);
   gout.drawEarth(pt, pDemo->angleEarth);
}

double Position::metersFromPixels = 40.0;

/*********************************
 * Initialize the simulation and set it in motion
 *********************************/
#ifdef _WIN32_X
#include <windows.h>
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine, _In_ int nCmdShow)
#else  // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{
   testRunner();

   // Initialize OpenGL
   Position ptUpperRight;
   ptUpperRight.setZoom(128000.0 /* 128km equals 1 pixel */);
   ptUpperRight.setPixelsX(1000.0);
   ptUpperRight.setPixelsY(1000.0);
   Interface ui(0, NULL, "Demo", /* name on the window */
                ptUpperRight);

   // Initialize the demo
   Demo demo(ptUpperRight);

   // set everything into action
   ui.run(callBack, &demo);

   return 0;
}
