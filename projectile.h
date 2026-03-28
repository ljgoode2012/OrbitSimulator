/***********************************************************************
 * Header File:
 *    Projectile : A bullet fired from the ship
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    A projectile fired by the Dream Chaser to destroy debris
 ************************************************************************/
#pragma once

#include "constants.h"
#include "entity.h"
#include "uiDraw.h"

class Ship;

/*********************************************
 * Projectile
 * A bullet fired from the ship that
 * expires after a set time
 *********************************************/
class Projectile : public Entity
{
public:
   Projectile(const Position& position, const Velocity& velocity)
       : Entity(position, velocity), ageGameUnits(0)
   {
   }

   static Projectile createFromShip(const Ship& ship);

   void update(double dt) override
   {
      Entity::update(dt);
      ++ageGameUnits;
   }

   void draw(ogstream& gout) const override
   {
      gout.drawProjectile(getPosition());
   }

   double getCollisionRadiusPixels() const override
   {
      return PROJECTILE_COLLISION_RADIUS_PIXELS;
   }

   bool isExpired() const
   {
      return ageGameUnits >= PROJECTILE_LIFETIME_GAME_UNITS;
   }

private:
   int ageGameUnits;
};
