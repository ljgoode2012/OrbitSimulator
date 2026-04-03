/***********************************************************************
 * Header File:
 *    Test Ship : Test the Ship class
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    All the unit tests for Ship
 ************************************************************************/

#pragma once

#include "constants.h"
#include "ship.h"
#include "unitTest.h"

/*********************************************
 * SHIP DOUBLE
 * A test double for Ship to expose protected members
 *********************************************/
class ShipDouble : public Ship
{
public:
   using Ship::Ship;
   void setRotationRadians(double radians) { setRotation(Angle(radians)); }
};

/*********************************************
 * TEST SHIP
 * Unit tests for the Ship class
 *********************************************/
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
      thrust_actualAccelerationIs0Point2G();
      thrust_velocityChangeOverOneFrame();
      thrust_velocityChangeOverOneSecond();
      report("Ship");
   }

private:
   static constexpr double HALF_PI = 1.57079632679489661923;
   static constexpr double TWO_PI = 6.28318530717958647692;
   static constexpr double EARTH_GRAVITY_M_S_SQ = 9.8;
   static constexpr double TOLERANCE = 0.001;

   /*********************************************
    * name:    DEFAULT CONSTRUCTOR
    * input:   nothing
    * output:  position=(0,0), velocity=(0,0), rotation=0
    *********************************************/
   void construct_default()
   {
      // SETUP
      Ship ship;

      // EXERCISE

      // VERIFY
      assertEquals(ship.position.x, 0.0);
      assertEquals(ship.position.y, 0.0);
      assertEquals(ship.velocity.dx, 0.0);
      assertEquals(ship.velocity.dy, 0.0);
      assertEquals(ship.rotation.radians, 0.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    TURN LEFT ROTATES COUNTER CLOCKWISE
    * input:   ship at rotation=0
    * output:  rotation = 2π - SHIP_TURN_ANGLE_RADIANS
    *********************************************/
   void turnLeft_rotatesCounterClockwise()
   {
      // SETUP
      Ship ship;
      const double expected = TWO_PI - SHIP_TURN_ANGLE_RADIANS;

      // EXERCISE
      ship.turnLeft();

      // VERIFY
      assertEqualsTolerance(ship.rotation.radians, expected, 0.0001);

      // TEARDOWN
   }

   /*********************************************
    * name:    TURN RIGHT ROTATES CLOCKWISE
    * input:   ship at rotation=0
    * output:  rotation = SHIP_TURN_ANGLE_RADIANS
    *********************************************/
   void turnRight_rotatesClockwise()
   {
      // SETUP
      Ship ship;
      const double expected = SHIP_TURN_ANGLE_RADIANS;

      // EXERCISE
      ship.turnRight();

      // VERIFY
      assertEqualsTolerance(ship.rotation.radians, expected, 0.0001);

      // TEARDOWN
   }

   /*********************************************
    * name:    THRUST FORWARD CHANGES VELOCITY ALONG HEADING
    * input:   ship at rotation=0, dt=48.0
    * output:  velocity.dy = SHIP_THRUST_ACCELERATION * 48.0
    *********************************************/
   void thrustForward_changesVelocityAlongHeading()
   {
      // SETUP
      Ship ship;
      const double dt = 48.0;
      const double expectedDeltaV =
         SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED * dt;

      // EXERCISE
      ship.thrustForward(dt);

      // VERIFY
      assertEqualsTolerance(ship.velocity.dx, 0.0, 0.0001);
      assertEqualsTolerance(ship.velocity.dy, expectedDeltaV, 0.0001);

      // TEARDOWN
   }

   /*********************************************
    * name:    THRUST FORWARD QUARTER TURN CHANGES X VELOCITY
    * input:   ship at rotation=π/2, dt=48.0
    * output:  velocity.dx = SHIP_THRUST_ACCELERATION * 48.0
    *********************************************/
   void thrustForward_quarterTurnChangesXVelocity()
   {
      // SETUP
      Ship ship;
      ship.rotation.radians = HALF_PI;
      const double dt = 48.0;
      const double expectedDeltaV =
         SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED * dt;

      // EXERCISE
      ship.thrustForward(dt);

      // VERIFY
      assertEqualsTolerance(ship.velocity.dx, expectedDeltaV, 0.0001);
      assertEqualsTolerance(ship.velocity.dy, 0.0, 0.0001);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE MOVES SHIP FROM VELOCITY
    * input:   ship with velocity from thrust, dt=3.0
    * output:  position updated by velocity * dt
    *********************************************/
   void update_movesShipFromVelocity()
   {
      // SETUP
      Ship ship;
      const double expectedVelocity =
         SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED * 48.0;
      ship.velocity.dy = expectedVelocity;
      const double dt = 3.0;

      // EXERCISE
      ship.update(dt);

      // VERIFY
      assertEqualsTolerance(ship.velocity.dy, expectedVelocity, 0.0001);
      assertEqualsTolerance(ship.position.y, expectedVelocity * dt, 0.0001);

      // TEARDOWN
   }

   /*********************************************
    * name:    THRUST ACTUAL ACCELERATION IS 0.2 G
    * input:   SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED
    * output:  G-force = 0.204 G (not dozens of G's)
    *********************************************/
   void thrust_actualAccelerationIs0Point2G()
   {
      // SETUP
      const double thrustAcceleration =
         SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED;

      // EXERCISE
      // Calculate actual G-force: acceleration ÷ Earth's gravity
      const double gForce = thrustAcceleration / EARTH_GRAVITY_M_S_SQ;

      // VERIFY
      // Thrust is 2.0 m/s² ÷ 9.8 m/s²/G = 0.204 G
      assertEqualsTolerance(gForce, 0.204, TOLERANCE);

      // Additional verification: this is less than 1 G
      assertUnit(gForce < 1.0);

      // This is NOT dozens of G's - it's gentle
      assertUnit(gForce < 10.0); // Not even 1 dozen

      // TEARDOWN
   }

   /*********************************************
    * name:    THRUST VELOCITY CHANGE OVER ONE FRAME
    * input:   ship at rest, thrust for dt=48 seconds (one frame)
    * output:  velocity change = 96 m/s
    *********************************************/
   void thrust_velocityChangeOverOneFrame()
   {
      // SETUP
      Ship ship;
      ship.position.x = 0.0;
      ship.position.y = 0.0;
      ship.velocity.dx = 0.0;
      ship.velocity.dy = 0.0;
      ship.rotation.radians = 0.0; // Pointing up

      const double initialSpeed = 0.0;
      const double dt = SIM_SECONDS_PER_FRAME; // 48 seconds

      // EXERCISE
      // Apply thrust (simulate pressing down arrow)
      ship.thrustForward(dt);

      // Calculate final speed
      const double finalSpeed = std::sqrt(ship.velocity.dx * ship.velocity.dx +
                                          ship.velocity.dy * ship.velocity.dy);

      // VERIFY
      // Expected: Δv = a × t = 2.0 m/s² × 48 sec = 96 m/s
      const double expectedDeltaV =
         SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED * dt;

      assertEqualsTolerance(expectedDeltaV, 96.0, TOLERANCE);
      assertEqualsTolerance(finalSpeed, expectedDeltaV, TOLERANCE);

      // This is correct physics: gentle 0.2 G applied for 48 seconds

      // TEARDOWN
   }

   /*********************************************
    * name:    THRUST VELOCITY CHANGE OVER ONE SECOND
    * input:   ship at rest, thrust for 30 frames (1 real second)
    * output:  velocity change = 2,880 m/s (from time dilation, not high
    *G-force)
    *********************************************/
   void thrust_velocityChangeOverOneSecond()
   {
      // SETUP
      Ship ship;
      ship.position.x = 0.0;
      ship.position.y = 0.0;
      ship.velocity.dx = 0.0;
      ship.velocity.dy = 0.0;
      ship.rotation.radians = 0.0;

      const double dt = SIM_SECONDS_PER_FRAME; // 48 seconds per frame
      const int framesPerSecond = 30;

      // EXERCISE
      // Apply thrust for 30 frames (1 real-world second of gameplay)
      for (int i = 0; i < framesPerSecond; ++i)
      {
         ship.thrustForward(dt);
      }

      // VERIFY
      const double finalSpeed = std::sqrt(ship.velocity.dx * ship.velocity.dx +
                                          ship.velocity.dy * ship.velocity.dy);
      // Total simulated time: 48 sec × 30 frames = 1,440 seconds
      const double totalSimTime = dt * framesPerSecond;
      assertEqualsTolerance(totalSimTime, 1440.0, TOLERANCE);

      // Expected: Δv = 2.0 m/s² × 1,440 sec = 2,880 m/s
      const double expectedDeltaV =
         SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED * totalSimTime;

      assertEqualsTolerance(expectedDeltaV, 2880.0, TOLERANCE);
      assertEqualsTolerance(finalSpeed, expectedDeltaV, TOLERANCE);

      // This is NOT high G-force - it's time dilation
      // 0.2 G for 24 minutes = realistic orbital maneuvering

      // TEARDOWN
   }
};
