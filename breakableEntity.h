#pragma once

#include "entity.h"

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

private:
   bool isHit;
};
