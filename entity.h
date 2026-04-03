/***********************************************************************
 * Header File:
 *    Entity : The base class for all objects in the simulation
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Everything we need to know about a movable object in orbit
 ************************************************************************/

#pragma once

#include "acceleration.h"
#include "angle.h"
#include "position.h"
#include "velocity.h"
class ogstream;
class TestFragment;
class TestProjectile;
class TestSatellite;
class TestShip;
class TestEntity;
class BreakableEntity;
class TimedEntity;

/*********************************************
 * Entity
 * The base class for all objects in the
 * simulation that can move and collide
 *********************************************/
class Entity
{
public:
   friend class TestFragment;
   friend class TestProjectile;
   friend class TestSatellite;
   friend class TestShip;
   friend class TestEntity;
   friend class TestBreakableEntity;   // Add this
   friend class TestTimedEntity;        // Add this
   // Constructors
   Entity() : position(), velocity(), rotation(0.0), angularVelocity(0.0) {}
   Entity(const Position& pos, const Velocity& vel,
          const Angle& rotation = Angle(), double angularVelocity = 0.0)
       : position(pos), velocity(vel), rotation(rotation),
         angularVelocity(angularVelocity)
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
   void setAngularVelocity(double angularVelocity)
   {
      this->angularVelocity = angularVelocity;
   }
   // Update the entity's position and velocity using basic kinematics
   virtual void update(double dt);

   virtual void draw(ogstream& gout) const { (void)gout; }

   virtual double getCollisionRadiusPixels() const { return 0.0; }

protected:
   Position& getPositionMutable() { return position; }
   const Velocity& getVelocity() const { return velocity; }
   void setRotation(const Angle& rotation) { this->rotation = rotation; }

private:
   static Acceleration computeGravityAcceleration(const Position& position);
   Position position;
   Velocity velocity;
   Angle rotation;
   double angularVelocity;
};
