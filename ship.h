#pragma once

#include <cmath>

#include "entity.h"

class Ship : public Entity
{
public:
   static constexpr double TURN_ANGLE_RADIANS = 0.1;
   static constexpr double THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED = 2.0;

   Ship() : Entity() {}
   Ship(const Position& position, const Velocity& velocity, const Angle& rotation = Angle())
      : Entity(position, velocity, rotation, 0.0)
   {
   }

   void turnLeft()
   {
      turnBy(-TURN_ANGLE_RADIANS);
   }

   void turnRight()
   {
      turnBy(TURN_ANGLE_RADIANS);
   }

   void thrustForward(double dt)
   {
      applyThrust(1.0, dt);
   }

   void thrustReverse(double dt)
   {
      applyThrust(-1.0, dt);
   }

   double getVelocityDX() const
   {
      return getVelocity().dx;
   }

   double getVelocityDY() const
   {
      return getVelocity().dy;
   }

private:
   void turnBy(double deltaRadians)
   {
      setRotation(Angle(getRotation().getRadians() + deltaRadians));
   }

   void applyThrust(double direction, double dt)
   {
      const double heading = getRotation().getRadians();
      const double deltaV = direction * THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED * dt;

      Velocity velocity = getVelocity();
      velocity.dx += std::sin(heading) * deltaV;
      velocity.dy += std::cos(heading) * deltaV;
      setVelocity(velocity);
   }
};
