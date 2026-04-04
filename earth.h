/***********************************************************************
 * Header File:
 *    Earth : The planet at the center of the simulation
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    The Earth sits at the origin, rotates in place, and destroys
 *    anything that enters its collision radius.
 ************************************************************************/

#pragma once

#include "constants.h"
#include "entity.h"
#include "position.h"
#include "uiDraw.h"

/*********************************************
 * Earth
 * The planet at the center of the simulation.
 * Rotates in place and collides with entities.
 *********************************************/
class Earth : public Entity
{
public:
   Earth()
       : Entity(Position(), Velocity(), Angle(), -EARTH_ROTATION_RATE)
   {
   }

   double getCollisionRadiusPixels() const override
   {
      return EARTH_COLLISION_RADIUS_PIXELS;
   }

   bool isCollisionImmune() const override { return true; }

   void draw(ogstream& gout) const override
   {
      gout.drawEarth(getPosition(), getRotation().getRadians());
   }

   bool isCollidingWith(const Entity& entity) const
   {
      const double entityRadiusPixels = entity.getCollisionRadiusPixels();
      if (entityRadiusPixels <= 0.0)
         return false;
      const double collisionRadiusMeters =
         (getCollisionRadiusPixels() + entityRadiusPixels) *
         entity.getPosition().getZoom();
      return computeDistance(getPosition(), entity.getPosition()) <=
             collisionRadiusMeters;
   }
};
