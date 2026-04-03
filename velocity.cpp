/***********************************************************************
 * Source File:
 *    Velocity : The representation of velocity in 2D space
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Handles velocity calculations and operations
 ************************************************************************/

#include "velocity.h"
#include "acceleration.h"
#include "angle.h"
#include <cmath>

/******************************************
 * Velocity : getAngle
 * Get the angle of velocity
 *****************************************/
Angle Velocity::getAngle() const
{
   Angle angle;
   angle.setDxDy(dx, dy);
   return angle;
}

/******************************************
 * Velocity : Assignment Operator
 * Assign one velocity to another
 *****************************************/
Velocity& Velocity::operator=(const Velocity& v)
{
   if (this != &v)
   {
      dx = v.dx;
      dy = v.dy;
   }
   return *this;
}
