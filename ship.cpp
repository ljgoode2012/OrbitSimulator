#include "ship.h"

void Ship::applyThrust(double direction, double dt)
{
   const double heading = getRotation().getRadians();
   const double deltaV = direction * THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED * dt;

   Velocity velocity = getVelocity();
   velocity.dx += std::sin(heading) * deltaV;
   velocity.dy += std::cos(heading) * deltaV;
   setVelocity(velocity);
}
