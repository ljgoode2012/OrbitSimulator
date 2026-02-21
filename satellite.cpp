/***********************************************************************
 * Source File:
 *    Point : The representation of a position on the screen
 * Author:
 *    Lindsey Goode and Porter Williams
 * Summary:
 *    Everything we need to know about a location on the screen.
 ************************************************************************/


#include "position.h"
#include "velocity.h"
#include "acceleration.h"
#include "satellite.h"

/******************************************
 * Satellite : Update
 * Update the satellite's position
 *****************************************/

 void Satellite::update(double dt)
{
   // Update velocity: v = v0 + a * t
   velocity.dx += acceleration.ddx * dt;
   velocity.dy += acceleration.ddy * dt;

   // Update position: s = s0 + v * t
   position.setMetersX(position.getMetersX() + velocity.dx * dt);
   position.setMetersY(position.getMetersY() + velocity.dy * dt);
};
