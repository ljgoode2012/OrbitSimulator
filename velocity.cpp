#include "velocity.h"
#include "angle.h"
#include "acceleration.h"
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

Velocity& Velocity::operator=(const Velocity& v)
{
   if (this != &v)
   {
      dx = v.dx;
      dy = v.dy;
   }
   return *this;
}
