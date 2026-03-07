#pragma once

#include "entity.h"

/*********************************************
 * TimedEntity
 * An entity that disappears after a certain amount of time.
 *********************************************/
class TimedEntity : public Entity
{
public:
   // Constructors
   TimedEntity() : Entity(), age(0), expireTime(0) {}
   TimedEntity(const Position& pos, const Velocity& vel, int expireTime)
       : Entity(pos, vel), age(0), expireTime(expireTime)
   {
   }
   TimedEntity(const Position& pos, const Velocity& vel, const Angle& head, int expireTime)
       : Entity(pos, vel, head), age(0), expireTime(expireTime)
   {
   }

   // Destructor
   ~TimedEntity() = default;

   // setters
   void setExpireTime(int expireTime) { this->expireTime = expireTime; }
   void setAge(int age) { this->age = age; }

   // getters
   int getExpireTime() const { return expireTime; }
   int getAge() const { return age; }


   // Update the entity's state
   void update(int deltaTime)
   {
       age += deltaTime;
       if (age >= expireTime)
       {
          // remove the entity from the simulation
       }
   }

private:
   int age; // The current age of the entity
   int expireTime; // The time at which the entity should expire
};
