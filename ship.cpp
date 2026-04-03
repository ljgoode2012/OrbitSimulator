/***********************************************************************
 * Source File:
 *    Ship : The representation of a player-controlled ship
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Handles ship thrust and movement controls
 ************************************************************************/

#include "ship.h"
#include "constants.h"

/******************************************
 * Ship : Apply Thrust
 * Apply thrust to the ship in the direction of its current heading
 *****************************************/
void Ship::applyThrust(double direction, double dt)
{
   const double heading = getRotation().getRadians();
   const double deltaV = direction *
                         SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED *
                         dt;

   Velocity velocity = getVelocity();
   velocity.dx += std::sin(heading) * deltaV;
   velocity.dy += std::cos(heading) * deltaV;
   setVelocity(velocity);
}
