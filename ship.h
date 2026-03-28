#pragma once

#include <cmath>

#include "entity.h"
#include "uiDraw.h"

class Ship : public Entity
{
public:
   static constexpr double TURN_ANGLE_RADIANS = 0.1;
   static constexpr double THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED = 2.0;

   Ship() : Entity(), thrusting(false) {}
   Ship(const Position& position, const Velocity& velocity, const Angle& rotation = Angle())
      : Entity(position, velocity, rotation, 0.0), thrusting(false)
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

   void setThrusting(bool thrusting)
   {
      this->thrusting = thrusting;
   }

   void draw(ogstream& gout) const override
   {
      gout.drawShip(getPosition(), getRotation(), thrusting);
   }

   double getCollisionRadiusPixels() const override
   {
      return 10.0;
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

   void applyThrust(double direction, double dt);

   bool thrusting;
};
