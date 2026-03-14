#pragma once

#include <cmath>
#include "acceleration.h"
#include "angle.h"

#include "position.h"
#include "velocity.h"

class Entity
{
public:

   // Constructors
   Entity() : position(), velocity(), rotation(0.0), angularVelocity(0.0) {}
   Entity(const Position& pos, const Velocity& vel,
          const Angle& rotation = Angle(),
          double angularVelocity = 0.0)
       : position(pos), velocity(vel),
         rotation(rotation), angularVelocity(angularVelocity)
   {
   }

   virtual ~Entity() = default;

   // getters
   const Position& getPosition() const { return position; }
   const Angle& getRotation() const { return rotation; }
   double getAngularVelocity() const { return angularVelocity; }

   // setters
   void setPosition(const Position& position) { this->position = position; }
   void setVelocity(const Velocity& velocity) { this->velocity = velocity; }
   void setAngularVelocity(double angularVelocity) { this->angularVelocity = angularVelocity; }
   // Update the entity's position and velocity using basic kinematics
   virtual void update(double dt)
   {
      const Acceleration gravityAcceleration = computeGravityAcceleration(position);
      velocity.update(gravityAcceleration, dt);
      position.update(velocity, dt);
      rotation.addRadians(angularVelocity * dt);
   }

protected:
   Position& getPositionMutable() { return position; }
   const Velocity& getVelocity() const { return velocity; }
   void setRotation(const Angle& rotation)
   {
      this->rotation = rotation;
   }

private:
   static Acceleration computeGravityAcceleration(const Position& position)
   {
      constexpr double EARTH_GRAVITATIONAL_PARAMETER = 3.986004418e14;

      const double xMeters = position.getMetersX();
      const double yMeters = position.getMetersY();
      const double radiusSquared = xMeters * xMeters + yMeters * yMeters;

      if (radiusSquared <= 0.0)
         return Acceleration();

      const double radius = std::sqrt(radiusSquared);
      const double radiusCubed = radiusSquared * radius;

      const double accelerationX = (-EARTH_GRAVITATIONAL_PARAMETER * xMeters) / radiusCubed;
      const double accelerationY = (-EARTH_GRAVITATIONAL_PARAMETER * yMeters) / radiusCubed;
      return Acceleration(accelerationX, accelerationY);
   }
   Position position;
   Velocity velocity;
   Angle rotation;
   double angularVelocity;
};
