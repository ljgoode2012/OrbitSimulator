/***********************************************************************
 * Source File:
 *    Point : The representation of a position on the screen
 * Author:
 *    Lindsey Goode and Porter Williams
 * Summary:
 *    Everything we need to know about a location on the screen.
 ************************************************************************/

#include "satellite.h"

#include <cmath>

#include "acceleration.h"
#include "position.h"
#include "velocity.h"

/******************************************
 * Satellite : Update
 * Update the satellite's position
 *****************************************/

void Satellite::update(double dt)
{
   // Earth gravitational parameter (mu = G*M) in m^3/s^2
   constexpr double MU = 3.986004418e14;

   // Acceleration due to gravity at the satellite's current position.
   const double x = position.getMetersX();
   const double y = position.getMetersY();
   const double r2 = x * x + y * y;

   if (r2 > 0.0)
   {
      const double r = std::sqrt(r2);
      const double r3 = r2 * r;
      acceleration.ddx = (-MU * x) / r3;
      acceleration.ddy = (-MU * y) / r3;
   }
   else
   {
      acceleration.ddx = 0.0;
      acceleration.ddy = 0.0;
   }

   velocity.dx += acceleration.ddx * dt;
   velocity.dy += acceleration.ddy * dt;

   position.setMetersX(position.getMetersX() + velocity.dx * dt);
   position.setMetersY(position.getMetersY() + velocity.dy * dt);
}
