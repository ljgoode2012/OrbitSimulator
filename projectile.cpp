#include "projectile.h"

#include <cmath>

#include "ship.h"

Projectile Projectile::createFromShip(const Ship& ship)
{
   const double radians = ship.getRotation().getRadians();

   Position projectilePosition = ship.getPosition();
   projectilePosition.addPixelsX(std::sin(radians) * SPAWN_OFFSET_PIXELS);
   projectilePosition.addPixelsY(std::cos(radians) * SPAWN_OFFSET_PIXELS);

   Velocity projectileVelocity(ship.getVelocityDX() + std::sin(radians) * SPEED_BOOST_METERS_PER_SECOND,
                               ship.getVelocityDY() + std::cos(radians) * SPEED_BOOST_METERS_PER_SECOND);

   return Projectile(projectilePosition, projectileVelocity);
}
