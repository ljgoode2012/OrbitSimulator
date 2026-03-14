#include "velocity.h"
#include "angle.h"
#include "acceleration.h"
#include <cmath>

/******************************************
 * Velocity : getSpeed
 * Get the magnitude of velocity
 *****************************************/
double Velocity::getSpeed() const
{
   return std::sqrt(dx * dx + dy * dy);
}

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
 * Velocity : set
 * Set velocity from angle and magnitude
 *****************************************/
void Velocity::set(const Angle& angle, double magnitude)
{
   dx = angle.getDx() * magnitude;
   dy = angle.getDy() * magnitude;
}

/******************************************
 * Velocity : add
 * Add acceleration to velocity over time
 *****************************************/
void Velocity::add(const Acceleration& acceleration, double time)
{
   dx += acceleration.getDDX() * time;
   dy += acceleration.getDDY() * time;
}

/******************************************
 * Velocity : reverse
 * Reverse the velocity direction
 *****************************************/
void Velocity::reverse()
{
   dx = -dx;
   dy = -dy;
}
