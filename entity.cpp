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
   const Acceleration gravityAcceleration = computeGravityAcceleration(position);
   velocity.update(gravityAcceleration, dt);
   position.update(velocity, dt);
   rotation.addRadians(angularVelocity * dt);
}

Acceleration Entity::computeGravityAcceleration(const Position& position)
{
   const double xMeters = position.getMetersX();
   const double yMeters = position.getMetersY();
   const double radiusSquared = xMeters * xMeters + yMeters * yMeters;

   if (radiusSquared <= 0.0)
      return Acceleration();

   const double radius = std::sqrt(radiusSquared);
   const double radiusCubed = radiusSquared * radius;
   const double accelerationX = (-MU * xMeters) / radiusCubed;
   const double accelerationY = (-MU * yMeters) / radiusCubed;
   return Acceleration(accelerationX, accelerationY);
}
