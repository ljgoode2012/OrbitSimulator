#pragma once

#include <cmath>

#include "position.h"
#include "velocity.h"

class Entity
{
public:

   // Constructors
   Entity() : position(), velocity(), rotation(0.0), angularVelocity(0.0) {}
   Entity(const Position& pos, const Velocity& vel,
          double rotation = 0.0,
          double angularVelocity = 0.0)
       : position(pos), velocity(vel),
         rotation(rotation), angularVelocity(angularVelocity)
   {
   }

   virtual ~Entity() = default;

   // getters
   const Position& getPosition() const { return position; }
   const Velocity& getVelocity() const { return velocity; }
   double getRotation() const { return rotation; }
   double getAngularVelocity() const { return angularVelocity; }

   // setters
   void setPosition(const Position& position) { this->position = position; }
   void setVelocity(const Velocity& velocity) { this->velocity = velocity; }
   void setRotation(double rotation)
   {
      this->rotation = rotation;
      normalizeRotation();
   }
   void setAngularVelocity(double angularVelocity) { this->angularVelocity = angularVelocity; }

   // Update the entity's position and velocity using basic kinematics
   virtual void update(double dt)
   {
      // Earth gravitational parameter (mu = G*M) in m^3/s^2
      constexpr double MU = 3.986004418e14;

      // Acceleration due to gravity at the entity's current position.
      const double x = position.getMetersX();
      const double y = position.getMetersY();
      const double r2 = x * x + y * y;
      double ax = 0.0;
      double ay = 0.0;

      if (r2 > 0.0)
      {
         const double r = std::sqrt(r2);
         const double r3 = r2 * r;
         ax = (-MU * x) / r3;
         ay = (-MU * y) / r3;
      }

      velocity.dx += ax * dt;
      velocity.dy += ay * dt;

      position.setMetersX(position.getMetersX() + velocity.dx * dt);
      position.setMetersY(position.getMetersY() + velocity.dy * dt);

      rotation += angularVelocity * dt;
      normalizeRotation();
   }

protected:
   Position& getPositionMutable() { return position; }
   Velocity& getVelocityMutable() { return velocity; }

   void normalizeRotation()
   {
      constexpr double TWO_PI = 6.28318530717958647692;
      rotation = std::fmod(rotation, TWO_PI);
      if (rotation < 0.0)
         rotation += TWO_PI;
   }

private:
   Position position;
   Velocity velocity;
   double rotation;
   double angularVelocity;
};
