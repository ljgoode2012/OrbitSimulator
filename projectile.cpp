#include "projectile.h"
#include "constants.h"
#include "ship.h"
#include <cmath>

Projectile Projectile::createFromShip(const Ship& ship)
{
   const double radians = ship.getRotation().getRadians();

   Position projectilePosition = ship.getPosition();
   projectilePosition.addPixelsX(std::sin(radians) *
                                 PROJECTILE_SPAWN_OFFSET_PIXELS);
   projectilePosition.addPixelsY(std::cos(radians) *
                                 PROJECTILE_SPAWN_OFFSET_PIXELS);

   Velocity projectileVelocity(
      ship.getVelocityDX() +
         std::sin(radians) * PROJECTILE_SPEED_BOOST_METERS_PER_SECOND,
      ship.getVelocityDY() +
         std::cos(radians) * PROJECTILE_SPEED_BOOST_METERS_PER_SECOND);

   return Projectile(projectilePosition, projectileVelocity);
}
