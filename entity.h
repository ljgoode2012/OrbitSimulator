#pragma once

#include <cmath>
#include "constants.h"
#include "acceleration.h"
#include "angle.h"

#include "position.h"
#include "velocity.h"
class ogstream;

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
   double getVelocityDX() const { return velocity.dx; }
   double getVelocityDY() const { return velocity.dy; }

   // setters
   void setPosition(const Position& position) { this->position = position; }
   void setVelocity(const Velocity& velocity) { this->velocity = velocity; }
   void setAngularVelocity(double angularVelocity) { this->angularVelocity = angularVelocity; }
   // Update the entity's position and velocity using basic kinematics
   virtual void update(double dt);

   virtual void draw(ogstream& gout) const
   {
      (void)gout;
   }

   virtual double getCollisionRadiusPixels() const
   {
      return 0.0;
   }

protected:
   Position& getPositionMutable() { return position; }
   const Velocity& getVelocity() const { return velocity; }
   void setRotation(const Angle& rotation)
   {
      this->rotation = rotation;
   }

private:
   static Acceleration computeGravityAcceleration(const Position& position);
   Position position;
   Velocity velocity;
   Angle rotation;
   double angularVelocity;
};
