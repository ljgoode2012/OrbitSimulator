/***********************************************************************
 * Header File:
 *    BreakableEntity : An entity that can be destroyed
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    An entity that can be hit and create debris when destroyed
 ************************************************************************/

#pragma once

#include "entity.h"
#include <memory>
#include <vector>

/*********************************************
 * BreakableEntity
 * An entity that can be destroyed and
 * create debris when hit
 *********************************************/
class BreakableEntity : public Entity
{
public:
   // Constructors
   BreakableEntity() : Entity(), isHit(false) {}
   BreakableEntity(const Position& pos, const Velocity& vel)
       : Entity(pos, vel), isHit(false)
   {
   }

   // setters
   void setIsHit() { isHit = true; }
   void setIsHit(bool isHit) { this->isHit = isHit; }

   // getters
   bool getIsHit() const { return isHit; }

   // Inline default implementation - derived classes can override
   virtual void createBreakupDebris(
      std::vector<std::unique_ptr<Entity>>& debris) const
   {
      (void)debris;
   }

   void breakInto(std::vector<std::unique_ptr<Entity>>& debris) const
   {
      createBreakupDebris(debris);
   }

private:
   bool isHit;
};
