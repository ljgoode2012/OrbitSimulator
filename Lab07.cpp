/*************************************************************
 * 1. Name:
 *      Demo
 * 2. Assignment Name:
 *      Lab 07: Orbit Simulator
 * 3. Assignment Description:
 *      Simulate satellites orbiting the earth
 * 4. What was the hardest part? Be as specific as possible.
 *      I decided to be pedantic and go with Earth's gravitational constant,
 *      which led to a lot of math, so implementing that was tricky. Getting
 *      the stars to twinkle independently was tricky too, but I'm pretty
 *      proud of my solution.
 * 5. How long did it take for you to complete the assignment?
 *      12 hours between the two of us.
 *****************************************************************/

#include <cmath>
#include <memory>
#include <utility>
#include <vector>

#include "constants.h"
#include "earth.h"
#include "position.h"
#include "projectile.h"
#include "satellite.h"
#include "ship.h"
#include "test.h"
#include "uiDraw.h"
#include "uiInteract.h"
#include "velocity.h"

/*************************************************************************
 * Demo
 * Holds all simulation state: entities, the player ship, earth rotation,
 * and background stars.
 *************************************************************************/
class Demo
{
public:
   Demo(const Position& ptUpperRight)
       : ship(nullptr)
   {
      initEntities();
      initStars(ptUpperRight);
   }

   void handleInput(const Interface* pUI)
   {
      if (ship == nullptr)
         return;
      if (pUI->isLeft())
         ship->turnLeft();
      if (pUI->isRight())
         ship->turnRight();
      const bool isThrusting = pUI->isDown();
      ship->setThrusting(isThrusting);
      if (isThrusting)
         ship->thrustForward(SIM_SECONDS_PER_FRAME);
      if (pUI->isSpace())
         entities.push_back(std::unique_ptr<Entity>(
            new Projectile(Projectile::createFromShip(*ship))));
   }

   void update()
   {
      earth.update(SIM_SECONDS_PER_FRAME);

      for (auto& star : stars)
         star.phase += star.speed; // unsigned char auto-wraps at 255
      for (auto& entity : entities)
         entity->update(SIM_SECONDS_PER_FRAME);

      std::vector<bool> removeEntity(entities.size(), false);
      std::vector<std::unique_ptr<Entity>> spawnedDebris;

      for (size_t i = 0; i < entities.size(); ++i)
      {
         for (size_t j = i + 1; j < entities.size(); ++j)
         {
            if (removeEntity[i] || removeEntity[j])
               continue;
            if (!areColliding(*entities[i], *entities[j]))
               continue;
            if (auto* b = dynamic_cast<BreakableEntity*>(entities[i].get()))
               b->breakInto(spawnedDebris);
            if (auto* b = dynamic_cast<BreakableEntity*>(entities[j].get()))
               b->breakInto(spawnedDebris);
            removeEntity[i] = true;
            removeEntity[j] = true;
         }
      }

      for (size_t i = 0; i < entities.size(); ++i)
      {
         if (removeEntity[i])
            continue;
         if (!earth.isCollidingWith(*entities[i]))
            continue;
         if (auto* b = dynamic_cast<BreakableEntity*>(entities[i].get()))
            b->breakInto(spawnedDebris);
         removeEntity[i] = true;
      }

      std::vector<std::unique_ptr<Entity>> survivors;
      survivors.reserve(entities.size());
      for (size_t i = 0; i < entities.size(); ++i)
      {
         const bool expired = entities[i]->isExpired();
         if (entities[i].get() == ship && (removeEntity[i] || expired))
            ship = nullptr;
         if (!removeEntity[i] && !expired)
            survivors.push_back(std::move(entities[i]));
      }
      for (std::unique_ptr<Entity>& debris : spawnedDebris)
         survivors.push_back(std::move(debris));
      entities = std::move(survivors);
   }

   void draw()
   {
      Position pt;
      ogstream gout(pt);
      // draw entities
      for (const auto& entity : entities)
         entity->draw(gout);
      // draw stars
      for (const auto& star : stars)
         gout.drawStar(star.position, star.phase);
      // draw the earth
      earth.draw(gout);
   }

private:
   struct Star
   {
      Position position;
      unsigned char phase; // twinkle phase (0-255, wraps automatically)
      unsigned char speed; // twinkle speed per frame (1-4)
   };

   void initEntities()
   {
      entities.reserve(GPS_SATELLITE_COUNT + 5);
      entities.push_back(std::make_unique<Hubble>());
      entities.push_back(std::make_unique<Sputnik>());
      entities.push_back(std::make_unique<Starlink>());
      entities.push_back(std::make_unique<CrewDragon>());

      for (int i = 0; i < GPS_SATELLITE_COUNT; ++i)
      {
         const double theta = (TWO_PI * static_cast<double>(i)) /
                              GPS_SATELLITE_COUNT;
         entities.push_back(std::make_unique<GPS>(theta));
      }

      Position shipPosition;
      shipPosition.setPixelsX(-450.0);
      shipPosition.setPixelsY(450.0);
      entities.push_back(std::make_unique<Ship>(
         shipPosition, Velocity(0.0, -2000.0), Angle(0.0)));
      ship = static_cast<Ship*>(entities.back().get());
   }

   void initStars(const Position& bounds)
   {
      stars.reserve(NUM_STARS);
      for (int i = 0; i < NUM_STARS; ++i)
      {
         stars.emplace_back();
         Star& star = stars.back();
         star.position.setPixelsX(bounds.getPixelsX() * random(-0.5, 0.5));
         star.position.setPixelsY(bounds.getPixelsY() * random(-0.5, 0.5));
         star.phase = static_cast<unsigned char>(random(0, 255));
         star.speed = static_cast<unsigned char>(random(1, 4));
      }
   }

   Earth earth;
   Ship* ship;
   std::vector<std::unique_ptr<Entity>> entities;
   std::vector<Star> stars;
};

/***********************************************************************
 * callBack
 * Called by OpenGL each frame to process input, update simulation
 * state, and draw the scene.
 ***********************************************************************/
void callBack(const Interface* pUI, void* p)
{
   Demo* pDemo = static_cast<Demo*>(p);
   pDemo->handleInput(pUI);
   pDemo->update();
   pDemo->draw();
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
   Interface ui(0, nullptr, "Demo", /* name on the window */
                ptUpperRight);

   // Initialize the demo
   Demo demo(ptUpperRight);

   // set everything into action
   ui.run(callBack, &demo);

   return 0;
}
