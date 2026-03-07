#pragma once
#include <cstdlib>

#include "breakableEntity.h"

class Satellite : public BreakableEntity
{
private:
   bool isDefunct;
   void setRandomSpinRate();

public:
   // Constructors
   Satellite() : BreakableEntity(), isDefunct(false)
   {
      setIsDefunct((std::rand() % 5) == 0);
   }
   Satellite(const Position& pos, const Velocity& vel)
       : BreakableEntity(pos, vel), isDefunct(false)
   {
      // Has a one in five chance of being defunct.
      setIsDefunct((std::rand() % 5) == 0);
   }

   // Update the satellite's position and velocity using basic kinematics
   void update(double dt) override;

   // SET Methods
   void setIsDefunct(bool isDefunct)
   {
      this->isDefunct = isDefunct;
      if (this->isDefunct)
         setRandomSpinRate();
      else
         setAngularVelocity(0.0);
   }

   // GET Methods
   bool getIsDefunct() const
   {
      return isDefunct;
   }
};
