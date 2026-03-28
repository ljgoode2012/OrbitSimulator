#pragma once

#include <memory>

#include "ship.h"
#include "unitTest.h"

class ShipDouble : public Ship
{
public:
   using Ship::Ship;
   void setRotationRadians(double radians) { setRotation(Angle(radians)); }
};

class TestShip : public UnitTest
{
public:
   void run()
   {
      construct_default();
      turnLeft_rotatesCounterClockwise();
      turnRight_rotatesClockwise();
      thrustForward_changesVelocityAlongHeading();
      thrustForward_quarterTurnChangesXVelocity();
      update_movesShipFromVelocity();
      report("Ship");
   }

private:
   static constexpr double HALF_PI = 1.57079632679489661923;
   static constexpr double TWO_PI = 6.28318530717958647692;

   void construct_default()
   {
      // SETUP
      std::unique_ptr<ShipDouble> shipPtr(new ShipDouble);
      ShipDouble& ship = *shipPtr;

      // EXERCISE

      // VERIFY
      assertEquals(ship.getPosition().getMetersX(), 0.0);
      assertEquals(ship.getPosition().getMetersY(), 0.0);
      assertEquals(ship.getVelocityDX(), 0.0);
      assertEquals(ship.getVelocityDY(), 0.0);
      assertEquals(ship.getRotation(), 0.0);

      // TEARDOWN
      shipPtr.reset();
   }

   void turnLeft_rotatesCounterClockwise()
   {
      // SETUP
      std::unique_ptr<ShipDouble> shipPtr(new ShipDouble);
      ShipDouble& ship = *shipPtr;
      const double expected = TWO_PI - SHIP_TURN_ANGLE_RADIANS;

      // EXERCISE
      ship.turnLeft();

      // VERIFY
      assertEqualsTolerance(ship.getRotation(), expected, 0.0001);

      // TEARDOWN
      shipPtr.reset();
   }

   void turnRight_rotatesClockwise()
   {
      // SETUP
      std::unique_ptr<ShipDouble> shipPtr(new ShipDouble);
      ShipDouble& ship = *shipPtr;
      const double expected = SHIP_TURN_ANGLE_RADIANS;

      // EXERCISE
      ship.turnRight();

      // VERIFY
      assertEqualsTolerance(ship.getRotation(), expected, 0.0001);

      // TEARDOWN
      shipPtr.reset();
   }

   void thrustForward_changesVelocityAlongHeading()
   {
      // SETUP
      std::unique_ptr<ShipDouble> shipPtr(new ShipDouble);
      ShipDouble& ship = *shipPtr;
      const double dt = 48.0;
      const double expectedDeltaV =
         SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED * dt;

      // EXERCISE
      ship.thrustForward(dt);

      // VERIFY
      assertEqualsTolerance(ship.getVelocityDX(), 0.0, 0.0001);
      assertEqualsTolerance(ship.getVelocityDY(), expectedDeltaV, 0.0001);

      // TEARDOWN
      shipPtr.reset();
   }

   void thrustForward_quarterTurnChangesXVelocity()
   {
      // SETUP
      std::unique_ptr<ShipDouble> shipPtr(new ShipDouble);
      ShipDouble& ship = *shipPtr;
      ship.setRotationRadians(HALF_PI);
      const double dt = 48.0;
      const double expectedDeltaV =
         SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED * dt;

      // EXERCISE
      ship.thrustForward(dt);

      // VERIFY
      assertEqualsTolerance(ship.getVelocityDX(), expectedDeltaV, 0.0001);
      assertEqualsTolerance(ship.getVelocityDY(), 0.0, 0.0001);

      // TEARDOWN
      shipPtr.reset();
   }

   void update_movesShipFromVelocity()
   {
      // SETUP
      std::unique_ptr<ShipDouble> shipPtr(new ShipDouble);
      ShipDouble& ship = *shipPtr;
      ship.thrustForward(48.0);
      const double expectedVelocity =
         SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED * 48.0;
      const double dt = 3.0;

      // EXERCISE
      ship.update(dt);

      // VERIFY
      assertEqualsTolerance(ship.getVelocityDY(), expectedVelocity, 0.0001);
      assertEqualsTolerance(ship.getPosition().getMetersY(),
                            expectedVelocity * dt, 0.0001);

      // TEARDOWN
      shipPtr.reset();
   }
};
