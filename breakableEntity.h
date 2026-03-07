/***********************************************************************
 * Header File:
 *    BreakableEntity: The representation of a physical object in space that can
 * break into parts when it collides with another entity
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Each BreakableEntity inherits from Entity and has an additional boolean
 * member variable to track whether it has been hit by another entity.
 ************************************************************************/
#pragma once

#include "entity.h"

/*********************************************
 * BreakableEntity
 * An entity that can break into parts when it collides with another entity
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
   BreakableEntity(const Position& pos, const Velocity& vel, const Angle& head)
       : Entity(pos, vel, head), isHit(false)
   {
   }
   // Destructor
   ~BreakableEntity() = default;

   // setters
   void setIsHit()   { isHit = true;}

   // getters
   bool getIsHit() const { return isHit; }

private:
   bool isHit;
};
