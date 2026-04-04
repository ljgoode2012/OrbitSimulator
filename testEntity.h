/***********************************************************************
 * Header File:
 *    Test Entity : Test the Entity class
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    All the unit tests for Entity
 ************************************************************************/

#pragma once

#include <cmath>
#include <memory>

#include "breakableEntity.h"
#include "constants.h"
#include "timedEntity.h"
#include "unitTest.h"

/*********************************************
 * ENTITY DOUBLE
 * A test double for Entity to expose protected members
 *********************************************/
class EntityDouble : public Entity
{
public:
   EntityDouble() : Entity() {}
   EntityDouble(const Position& position, const Velocity& velocity,
                const Angle& rotation, double angularVelocity)
       : Entity(position, velocity, rotation, angularVelocity)
   {
   }

   double getVelocityDX() const { return getVelocity().dx; }
   double getVelocityDY() const { return getVelocity().dy; }
   void setRotationRadians(double radians) { setRotation(Angle(radians)); }
};

/*********************************************
 * TEST ENTITY
 * Unit tests for the Entity class
 *********************************************/
class TestEntity : public UnitTest
{
public:
   void run()
   {
      construct_default();
      construct_nonDefault();
      setPosition_updatesPosition();
      setVelocity_updatesVelocity();
      setAngularVelocity_updatesAngularVelocity();
      update_zeroPositionNoGravity();
      update_nonZeroPositionChangesState();
      update_nonZeroAngularVelocityChangesRotation();
      update_rotationWrapsPositive();
      update_rotationWrapsNegative();
      update_negativeTimeDelta();
      update_zeroTimeDelta();
      report("Entity");
   }

private:
   static constexpr double TWO_PI = 2.0 * M_PI;

   /*********************************************
    * name:    DEFAULT CONSTRUCTOR
    * input:   nothing
    * output:  position=(0,0), velocity=(0,0), rotation=0, angularVelocity=0
    *********************************************/
   void construct_default()
   {
      // SETUP

      // EXERCISE
      EntityDouble entity;

      // VERIFY
      assertEquals(entity.position.x, 0.0);
      assertEquals(entity.position.y, 0.0);
      assertEquals(entity.velocity.dx, 0.0);
      assertEquals(entity.velocity.dy, 0.0);
      assertEquals(entity.rotation.radians, 0.0);
      assertEquals(entity.angularVelocity, 0.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    NON-DEFAULT CONSTRUCTOR
    * input:   position=(10,20), velocity=(30,40), rotation=1.5,
    *angularVelocity=-0.25 output:  entity with specified values
    *********************************************/
   void construct_nonDefault()
   {
      // SETUP
      Position position;
      position.setMetersX(10.0);
      position.setMetersY(20.0);
      Velocity velocity;
      velocity.dx = 30.0;
      velocity.dy = 40.0;
      Angle rotation(1.5);
      double angularVelocity = -0.25;

      // EXERCISE
      EntityDouble entity(position, velocity, rotation, angularVelocity);

      // VERIFY
      assertEquals(entity.position.x, 10.0);
      assertEquals(entity.position.y, 20.0);
      assertEquals(entity.velocity.dx, 30.0);
      assertEquals(entity.velocity.dy, 40.0);
      assertEquals(entity.rotation.radians, 1.5);
      assertEquals(entity.angularVelocity, -0.25);

      // TEARDOWN
   }

   /*********************************************
    * name:    SET POSITION UPDATES POSITION
    * input:   position=(1234.5, -6789.0)
    * output:  entity.position=(1234.5, -6789.0)
    *********************************************/
   void setPosition_updatesPosition()
   {
      // SETUP
      EntityDouble entity;
      Position position;
      position.setMeters(1234.5, -6789.0);

      // EXERCISE
      entity.setPosition(position);

      // VERIFY
      assertEquals(entity.position.x, 1234.5);
      assertEquals(entity.position.y, -6789.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    SET VELOCITY UPDATES VELOCITY
    * input:   velocity=(-101.0, 202.0)
    * output:  entity.velocity=(-101.0, 202.0)
    *********************************************/
   void setVelocity_updatesVelocity()
   {
      // SETUP
      EntityDouble entity;
      Velocity velocity;
      velocity.dx = -101.0;
      velocity.dy = 202.0;

      // EXERCISE
      entity.setVelocity(velocity);

      // VERIFY
      assertEquals(entity.velocity.dx, -101.0);
      assertEquals(entity.velocity.dy, 202.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    SET ANGULAR VELOCITY UPDATES ANGULAR VELOCITY
    * input:   angularVelocity=0.75
    * output:  entity.angularVelocity=0.75
    *********************************************/
   void setAngularVelocity_updatesAngularVelocity()
   {
      // SETUP
      EntityDouble entity;

      // EXERCISE
      entity.setAngularVelocity(0.75);

      // VERIFY
      assertEquals(entity.angularVelocity, 0.75);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE ZERO POSITION NO GRAVITY
    * input:   entity at origin with velocity=(10,-20), dt=2.0
    * output:  position=(20,-40) (velocity applied, no gravity at origin)
    *********************************************/
   void update_zeroPositionNoGravity()
   {
      // SETUP
      EntityDouble entity;
      entity.velocity.dx = 10.0;
      entity.velocity.dy = -20.0;

      // EXERCISE
      entity.update(2.0);

      // VERIFY
      assertEquals(entity.position.x, 20.0);
      assertEquals(entity.position.y, -40.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE NON-ZERO POSITION CHANGES STATE
    * input:   entity at position=(1000000,0), velocity=(0,100), rotation=0.5,
    *angularVelocity=0.1, dt=1.0 output:  position and rotation changed
    *********************************************/
   void update_nonZeroPositionChangesState()
   {
      // SETUP
      Position position;
      position.setMeters(1000000.0, 0.0);
      Velocity velocity(0.0, 100.0);
      Angle rotation(0.5);
      EntityDouble entity(position, velocity, rotation, 0.1);
      const double dt = 1.0;

      // EXERCISE
      entity.update(dt);

      // VERIFY
      // Position should have changed due to velocity
      assertUnit(entity.position.y != 0.0);
      // Rotation should have changed due to angular velocity
      assertUnit(entity.rotation.radians != 0.5);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE NON-ZERO ANGULAR VELOCITY CHANGES ROTATION
    * input:   entity with rotation=0, angularVelocity=0.5, dt=2.0
    * output:  rotation=1.0
    *********************************************/
   void update_nonZeroAngularVelocityChangesRotation()
   {
      // SETUP
      EntityDouble entity;
      entity.setAngularVelocity(0.5);
      const double expectedRotation = 0.5 * 2.0;

      // EXERCISE
      entity.update(2.0);

      // VERIFY
      assertEqualsTolerance(entity.rotation.radians, expectedRotation, 0.0001);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE ROTATION WRAPS POSITIVE
    * input:   entity with rotation=2π-0.1, angularVelocity=0.2, dt=1.0
    * output:  rotation wraps to small positive value (<1.0)
    *********************************************/
   void update_rotationWrapsPositive()
   {
      // SETUP
      EntityDouble entity;
      entity.rotation.radians = TWO_PI - 0.1;
      entity.setAngularVelocity(0.2);

      // EXERCISE
      entity.update(1.0);

      // VERIFY
      assertUnit(entity.rotation.radians < 1.0);
      assertUnit(entity.rotation.radians >= 0.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE ROTATION WRAPS NEGATIVE
    * input:   entity with rotation=0.1, angularVelocity=-0.2, dt=1.0
    * output:  rotation wraps to value near 2π
    *********************************************/
   void update_rotationWrapsNegative()
   {
      // SETUP
      EntityDouble entity;
      entity.rotation.radians = 0.1;
      entity.setAngularVelocity(-0.2);

      // EXERCISE
      entity.update(1.0);

      // VERIFY
      assertUnit(entity.rotation.radians > TWO_PI - 1.0);
      assertUnit(entity.rotation.radians < TWO_PI);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE NEGATIVE TIME DELTA
    * input:   entity at position=(1000000,0), velocity=(0,100), dt=-1.0
    * output:  position.y < 0 (moved backwards)
    *********************************************/
   void update_negativeTimeDelta()
   {
      // SETUP
      Position position;
      position.setMeters(1000000.0, 0.0);
      Velocity velocity(0.0, 100.0);
      EntityDouble entity(position, velocity, Angle(), 0.0);

      // EXERCISE
      entity.update(-1.0);

      // VERIFY
      // Position should move backwards
      assertUnit(entity.position.y < 0.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE ZERO TIME DELTA
    * input:   entity at position=(1000,2000), velocity=(100,-50), rotation=1.5,
    *angularVelocity=0.5, dt=0.0 output:  entity state unchanged
    *********************************************/
   void update_zeroTimeDelta()
   {
      // SETUP
      Position position;
      position.setMeters(1000.0, 2000.0);
      Velocity velocity(100.0, -50.0);
      Angle rotation(1.5);
      EntityDouble entity(position, velocity, rotation, 0.5);

      // EXERCISE
      entity.update(0.0);

      // VERIFY
      assertEquals(entity.position.x, 1000.0);
      assertEquals(entity.position.y, 2000.0);
      assertEquals(entity.rotation.radians, 1.5);

      // TEARDOWN
   }
};
