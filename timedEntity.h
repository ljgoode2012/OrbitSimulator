/***********************************************************************
 * Header File:
 *    TimedEntity : An entity with a limited lifetime
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    An entity that expires after a certain amount of time
 ************************************************************************/

#pragma once

#include "entity.h"

/*********************************************
 * TimedEntity
 * An entity that tracks its age and
 * expires after a set time period
 *********************************************/
class TimedEntity : public Entity
{
public:
   // Constructors
   TimedEntity() : Entity(), age(0.0), expireTime(0.0) {}
   TimedEntity(const Position& pos, const Velocity& vel, double expireTime)
       : Entity(pos, vel), age(0.0), expireTime(expireTime)
   {
   }

   // setters
   void setExpireTime(double expireTime) { this->expireTime = expireTime; }
   void setAge(double age) { this->age = age; }

   // getters
   double getExpireTime() const { return expireTime; }
   double getAge() const { return age; }
   bool isExpired() const { return expireTime > 0.0 && age >= expireTime; }

   void updateTimer(double dt) { age += dt; }

   void update(double dt) override
   {
      Entity::update(dt);
      updateTimer(dt);
   }

private:
   double age;
   double expireTime;
};
