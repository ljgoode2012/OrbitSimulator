/***********************************************************************
 * Header File:
 *    Test Fragment : Test the Fragment class
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    All the unit tests for Fragment
 ************************************************************************/

#pragma once

#include "constants.h"
#include "satellite.h"
#include "unitTest.h"
#include <memory>

/*********************************************
 * TEST FRAGMENT
 * Unit tests for the Fragment class
 *********************************************/
class TestFragment : public UnitTest
{
public:
   void run()
   {
      construct_default();
      construct_nonDefault();
      construct_setsCollisionImmunity();
      update_decrementsImmunity();
      update_decrementsImmunityToZero();
      isCollisionImmune_trueWhenActive();
      isCollisionImmune_falseWhenExpired();
      isExpired_falseWhenAgeIsLess();
      isExpired_trueWhenLifetimeExceeded();
      isExpired_trueWhenLifetimeEqual();
      getCollisionRadiusPixels_returnsFragmentRadius();
      update_incrementsAge();
      report("Fragment");
   }

private:
   /*********************************************
    * name:    CONSTRUCT DEFAULT
    * input:   nothing
    * output:  position=(0,0), age=0, expireTime=0, not collision immune
    *********************************************/
   void construct_default()
   {
      // SETUP
      Fragment fragment;

      // EXERCISE

      // VERIFY
      assertEquals(fragment.position.x, 0.0);
      assertEquals(fragment.position.y, 0.0);
      assertEquals(fragment.age, 0.0);
      assertEquals(fragment.expireTime, 0.0);
      assertUnit(!fragment.isCollisionImmune());

      // TEARDOWN
   }

   /*********************************************
    * name:    CONSTRUCT NON-DEFAULT
    * input:   position=(1000.0, 2000.0), velocity=(500.0, -300.0), expireTime=75.0
    * output:  fragment with specified values, age=0.0
    *********************************************/
   void construct_nonDefault()
   {
      // SETUP
      Position position;
      position.x = 1000.0;
      position.y = 2000.0;
      Velocity velocity;
      velocity.dx = 500.0;
      velocity.dy = -300.0;
      double expireTime = 75.0;

      // EXERCISE
      Fragment fragment(position, velocity, expireTime);

      // VERIFY
      assertEquals(fragment.position.x, 1000.0);
      assertEquals(fragment.position.y, 2000.0);
      assertEquals(fragment.velocity.dx, 500.0);
      assertEquals(fragment.velocity.dy, -300.0);
      assertEquals(fragment.age, 0.0);
      assertEquals(fragment.expireTime, expireTime);

      // TEARDOWN
   }

   /*********************************************
    * name:    CONSTRUCT SETS COLLISION IMMUNITY
    * input:   position=(1000.0, 2000.0), velocity=(100.0, 200.0), expireTime=100.0
    * output:  collisionImmunityFrames=COLLISION_IMMUNITY_FRAMES
    *********************************************/
   void construct_setsCollisionImmunity()
   {
      // SETUP
      Position position;
      position.x = 1000.0;
      position.y = 2000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;

      // EXERCISE
      Fragment fragment(position, velocity, 100.0);

      // VERIFY
      assertEquals(fragment.collisionImmunityFrames, COLLISION_IMMUNITY_FRAMES);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE DECREMENTS IMMUNITY
    * input:   fragment with collisionImmunityFrames=2, dt=1.0
    * output:  collisionImmunityFrames=1
    *********************************************/
   void update_decrementsImmunity()
   {
      // SETUP
      Position position;
      position.x = 1000.0;
      position.y = 2000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment;
      fragment.position = position;
      fragment.velocity = velocity;
      fragment.collisionImmunityFrames = 2;

      // EXERCISE
      fragment.update(1.0);

      // VERIFY
      assertEquals(fragment.collisionImmunityFrames, 1);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE DECREMENTS IMMUNITY TO ZERO
    * input:   fragment with collisionImmunityFrames=1, dt=1.0
    * output:  collisionImmunityFrames=0
    *********************************************/
   void update_decrementsImmunityToZero()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment;
      fragment.position = position;
      fragment.velocity = velocity;
      fragment.collisionImmunityFrames = 1;

      // EXERCISE
      fragment.update(1.0);

      // VERIFY
      assertEquals(fragment.collisionImmunityFrames, 0);

      // TEARDOWN
   }

   /*********************************************
    * name:    IS COLLISION IMMUNE TRUE WHEN ACTIVE
    * input:   fragment with collisionImmunityFrames=3
    * output:  isCollisionImmune()=true
    *********************************************/
   void isCollisionImmune_trueWhenActive()
   {
      // SETUP
      Fragment fragment;
      fragment.collisionImmunityFrames = 3; // Set immunity to active value

      // EXERCISE
      bool result = fragment.isCollisionImmune();

      // VERIFY
      assertEquals(result, true);

      // TEARDOWN
   }

   /*********************************************
    * name:    IS COLLISION IMMUNE FALSE WHEN EXPIRED
    * input:   fragment with collisionImmunityFrames=0
    * output:  isCollisionImmune()=false
    *********************************************/
   void isCollisionImmune_falseWhenExpired()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment;
      fragment.position = position;
      fragment.velocity = velocity;
      fragment.collisionImmunityFrames = 0;

      // EXERCISE
      bool result = fragment.isCollisionImmune();

      // VERIFY
      assertEquals(result, false);

      // TEARDOWN
   }

   /*********************************************
    * name:    IS EXPIRED FALSE WHEN AGE IS LESS
    * input:   fragment with expireTime=5.0, age=4.0
    * output:  isExpired()=false
    *********************************************/
   void isExpired_falseWhenAgeIsLess()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment;
      fragment.position = position;
      fragment.velocity = velocity;
      fragment.expireTime = 5.0;
      fragment.age = 4.0;

      // EXERCISE
      bool result = fragment.isExpired();

      // VERIFY
      assertEquals(result, false);

      // TEARDOWN
   }

   /*********************************************
    * name:    IS EXPIRED TRUE WHEN LIFETIME EXCEEDED
    * input:   fragment with expireTime=5.0, age=6.0
    * output:  isExpired()=true
    *********************************************/
   void isExpired_trueWhenLifetimeExceeded()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment;
      fragment.position = position;
      fragment.velocity = velocity;
      fragment.expireTime = 5.0;
      fragment.age = 6.0;

      // EXERCISE
      bool result = fragment.isExpired();

      // VERIFY
      assertEquals(result, true);

      // TEARDOWN
   }

   /*********************************************
    * name:    IS EXPIRED TRUE WHEN LIFETIME EQUAL
    * input:   fragment with expireTime=5.0, age=5.0
    * output:  isExpired()=true
    *********************************************/
   void isExpired_trueWhenLifetimeEqual()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment;
      fragment.position = position;
      fragment.velocity = velocity;
      fragment.expireTime = 5.0;
      fragment.age = 5.0;

      // EXERCISE
      bool result = fragment.isExpired();

      // VERIFY
      assertEquals(result, true);

      // TEARDOWN
   }

   /*********************************************
    * name:    GET COLLISION RADIUS PIXELS RETURNS FRAGMENT RADIUS
    * input:   default fragment
    * output:  radius=FRAGMENT_COLLISION_RADIUS_PIXELS
    *********************************************/
   void getCollisionRadiusPixels_returnsFragmentRadius()
   {
      // SETUP
      Fragment fragment;

      // EXERCISE
      double radius = fragment.getCollisionRadiusPixels();

      // VERIFY
      assertEquals(radius, FRAGMENT_COLLISION_RADIUS_PIXELS);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE INCREMENTS AGE
    * input:   fragment with age=0.0, expireTime=100.0, three updates with dt=1.0
    * output:  age=3.0
    *********************************************/
   void update_incrementsAge()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment;
      fragment.position = position;
      fragment.velocity = velocity;
      fragment.expireTime = 100.0;

      // EXERCISE
      fragment.update(1.0);
      fragment.update(1.0);
      fragment.update(1.0);

      // VERIFY - Should have aged 3 seconds
      assertEquals(fragment.age, 3.0);

      // TEARDOWN
   }
};
