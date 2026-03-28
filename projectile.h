#pragma once

#include "entity.h"
#include "uiDraw.h"

class Ship;

class Projectile : public Entity
{
public:
   static constexpr double SPEED_BOOST_METERS_PER_SECOND = 9000.0;
   static constexpr double SPAWN_OFFSET_PIXELS = 19.0;
   static constexpr int LIFETIME_GAME_UNITS = 70;

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
      return 1.0;
   }

   bool isExpired() const
   {
      return ageGameUnits >= LIFETIME_GAME_UNITS;
   }

private:
   int ageGameUnits;
};
