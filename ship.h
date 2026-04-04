/***********************************************************************
 * Header File:
 *    Ship : The player-controlled spaceship
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    The Dream Chaser spacecraft that the player controls
 ************************************************************************/
#pragma once

#include <cmath>

#include "constants.h"
#include "entity.h"
#include "uiDraw.h"

class TestShip; // Forward declaration

/*********************************************
 * Ship
 * The player-controlled Dream Chaser
 * spaceship that can rotate and thrust
 *********************************************/
class Ship : public Entity
{
public:
   friend class TestShip;

   Ship() : Entity(), thrusting(false) {}
   Ship(const Position& position, const Velocity& velocity,
        const Angle& rotation = Angle())
       : Entity(position, velocity, rotation, 0.0), thrusting(false)
   {
   }

   void turnLeft() { turnBy(-SHIP_TURN_ANGLE_RADIANS); }
   void turnRight() { turnBy(SHIP_TURN_ANGLE_RADIANS); }

   void thrustForward(double dt) { applyThrust(1.0, dt); }
   void setThrusting(bool thrusting) { this->thrusting = thrusting; }

   void draw(ogstream& gout) const override
   {
      gout.drawShip(getPosition(), getRotation(), thrusting);
   }

   double getCollisionRadiusPixels() const override
   {
      return SHIP_COLLISION_RADIUS_PIXELS;
   }

   double getVelocityDX() const { return getVelocity().dx; }
   double getVelocityDY() const { return getVelocity().dy; }

private:
   void turnBy(double deltaRadians)
   {
      setRotation(Angle(getRotation().getRadians() + deltaRadians));
   }

   void applyThrust(double direction, double dt);

   bool thrusting;
};
