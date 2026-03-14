/***********************************************************************
 * Source File:
 *    Entity : The representation of a physical object in space
 * Author:
 *    Lindsey Goode and Porter Williams
 * Summary:
 *    Everything we need to know about an entity's position and velocity.
 ************************************************************************/

#include "entity.h"

#include <cmath>

#include "acceleration.h"
#include "position.h"
#include "velocity.h"

/******************************************
 * Entity : Update
 * Update the entity's position
 *****************************************/

void Entity::update(double dt)
{
   // Acceleration due to gravity at the entity's current position.
   const double x = position.getMetersX();
   const double y = position.getMetersY();
   const double r2 = x * x + y * y;

   Acceleration acceleration;
   if (r2 > 0.0)
   {
      const double r = std::sqrt(r2);
      const double r3 = r2 * r;
      acceleration.setDDX((-MU * x) / r3);
      acceleration.setDDY((-MU * y) / r3);
   }
   else
   {
      acceleration.setDDX(0.0);
      acceleration.setDDY(0.0);
   }

   velocity.addDX(acceleration.getDDX() * dt);
   velocity.addDY(acceleration.getDDY() * dt);

   position.setMetersX(position.getMetersX() + velocity.getDX() * dt);
   position.setMetersY(position.getMetersY() + velocity.getDY() * dt);
}
