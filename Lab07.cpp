/*************************************************************
 * 1. Name:
 *      Demo
 * 2. Assignment Name:
 *      Lab 07: Orbit Simulator
 * 3. Assignment Description:
 *      Simulate satellites orbiting the earth
 * 4. What was the hardest part? Be as specific as possible.
 *      I decided to be pedantic and go with Earth's gravitational constant, which led to a lot of math, so implementing that was tricky.
 *      Getting the stars to twinkle independently was tricky too, but I'm pretty pround of my solution.
 * 5. How long did it take for you to complete the assignment?
 *      12 hours between the two of us.
 *****************************************************************/

#include <cassert>      // for ASSERT
#include <cmath>        // for sqrt()
#include <array>
#include <vector>

#include "uiInteract.h" // for INTERFACE
#include "uiDraw.h"     // for RANDOM and DRAW*
#include "position.h"      // for POINT
#include "test.h"
#include "satellite.h"  // for Sattelite
#include "ship.h"
#include "velocity.h"   // for Velocity
using namespace std;



#define FRAMES_PER_SECOND 30.0
// frames per 1 simulated day, 1 simulated day will be 60 seconds of run time
#define FRAMES_PER_SIM_DAY (FRAMES_PER_SECOND * 60.0) //1800.0
// dialated seconds per frame (there are 86400 seconds in a day)
#define SIM_SECONDS_PER_FRAME (86400.0 / FRAMES_PER_SIM_DAY) // 48.0

/*************************************************************************
 * Demo
 * Test structure to capture the LM that will move around the screen
 *************************************************************************/
class Demo
{
public:
   std::array<GPS, 10> gpsSatellites;
   Hubble hubble;
   Sputnik sputnik;
   Starlink starlink;
   CrewDragon crewDragon;
   Ship ship;

   Demo(Position ptUpperRight)
       : ptUpperRight(ptUpperRight)
   {

      constexpr double TWO_PI = 6.28318530717958647692;
      for (size_t i = 0; i < gpsSatellites.size(); ++i)
      {
         const double theta = (TWO_PI * static_cast<double>(i)) / gpsSatellites.size();
         gpsSatellites[i] = GPS(theta);
      }
      hubble = Hubble();
      Position hubblePosition;
      hubblePosition.setMeters(0.0, -42164000.0);
      hubble.setPosition(hubblePosition);
      hubble.setVelocity(Velocity(3100.0, 0.0));

      crewDragon = CrewDragon();
      Position crewDragonPosition;
      crewDragonPosition.setMeters(0.0, 8000000.0);
      crewDragon.setPosition(crewDragonPosition);
      crewDragon.setVelocity(Velocity(-7900.0, 0.0));

      starlink = Starlink();
      Position starlinkPosition;
      starlinkPosition.setMeters(0.0, -13020000.0);
      starlink.setPosition(starlinkPosition);
      starlink.setVelocity(Velocity(5800.0, 0.0));

      sputnik = Sputnik(0.85 * TWO_PI);

      Position shipPosition;
      shipPosition.setPixelsX(-450.0);
      shipPosition.setPixelsY(450.0);
      ship = Ship(shipPosition, Velocity(0.0, -2000.0), Angle(0.0));
      ptFragmentSputnik.setPixelsX(sputnik.getPosition().getPixelsX() + 20.0);
      ptFragmentSputnik.setPixelsY(sputnik.getPosition().getPixelsY() + 20.0);
      ptFragmentShip.setPixelsX(ship.getPosition().getPixelsX() + 20.0);
      ptFragmentShip.setPixelsY(ship.getPosition().getPixelsY() + 20.0);

      // Generate stars
      const int NUM_STARS = 200;

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
         // I tried a couple different ranges, 1-4 did best, I think it looks beautiful at this speed!
         star.speed = static_cast<unsigned char>(random(1, 4));

         stars.push_back(star);
      }

      angleEarth = 0.0;
   }
   Position ptUpperRight;
   Position ptFragmentSputnik;
   Position ptFragmentShip;
   double angleEarth;
   struct Star
   {
      Position position;
      unsigned char phase; // char allows 0–255, giving 255 phases the star could be in.
      unsigned char speed; // likewise, the twinkle speed can be any of 255 values, allowing for a lot of variation.
      // I'm not using all of them at the moment though.
      // char is also just a small data type that won't take up much space, which is good,
      // because there will be a lot of stars, and they're already slowing down the simulation a little.
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
   

   if (pUI->isLeft())
      pDemo->ship.turnLeft();
   if (pUI->isRight())
      pDemo->ship.turnRight();
   if (pUI->isDown())
      pDemo->ship.thrustForward(SIM_SECONDS_PER_FRAME);


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
   pDemo->hubble.update(SIM_SECONDS_PER_FRAME);
   pDemo->sputnik.update(SIM_SECONDS_PER_FRAME);
   pDemo->starlink.update(SIM_SECONDS_PER_FRAME);
   pDemo->crewDragon.update(SIM_SECONDS_PER_FRAME);
   for (auto& satellite : pDemo->gpsSatellites)
      satellite.update(SIM_SECONDS_PER_FRAME);
   pDemo->ship.update(SIM_SECONDS_PER_FRAME);
   //
   // draw everything
   //

   Position pt;
   ogstream gout(pt);

   // draw satellites
   gout.drawCrewDragon(pDemo->crewDragon.getPosition(), pDemo->crewDragon.getRotation());
   gout.drawHubble    (pDemo->hubble.getPosition(),     pDemo->hubble.getRotation());
   gout.drawSputnik   (pDemo->sputnik.getPosition(),    pDemo->sputnik.getRotation());
   gout.drawStarlink  (pDemo->starlink.getPosition(),   pDemo->starlink.getRotation());
   gout.drawShip      (pDemo->ship.getPosition(),       pDemo->ship.getRotation(), pUI->isDown());
   for (const auto& satellite : pDemo->gpsSatellites)
      gout.drawGPS(satellite.getPosition(), satellite.getRotation());


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
int WINAPI wWinMain(
   _In_ HINSTANCE hInstance,
   _In_opt_ HINSTANCE hPrevInstance,
   _In_ PWSTR pCmdLine,
   _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{

   testRunner();

   // Initialize OpenGL
   Position ptUpperRight;
   ptUpperRight.setZoom(128000.0 /* 128km equals 1 pixel */);
   ptUpperRight.setPixelsX(1000.0);
   ptUpperRight.setPixelsY(1000.0);
   Interface ui(0, NULL,
      "Demo",   /* name on the window */
      ptUpperRight);

   // Initialize the demo
   Demo demo(ptUpperRight);

   // set everything into action
   ui.run(callBack, &demo);


   return 0;
}
