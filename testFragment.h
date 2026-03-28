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
   void construct_default()
   {
      // SETUP
      Fragment fragment;

      // EXERCISE

      // VERIFY
      assertEquals(fragment.getPosition().getMetersX(), 0.0);
      assertEquals(fragment.getPosition().getMetersY(), 0.0);
      assertEquals(fragment.getAge(), 0.0);
      assertEquals(fragment.getExpireTime(), 0.0);
      assertUnit(!fragment.isCollisionImmune());

      // TEARDOWN
   }

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
      assertEquals(fragment.getPosition().getMetersX(), 1000.0);
      assertEquals(fragment.getPosition().getMetersY(), 2000.0);
      assertEquals(fragment.getVelocity().dx, 500.0);
      assertEquals(fragment.getVelocity().dy, -300.0);
      assertEquals(fragment.getAge(), 0.0);
      assertEquals(fragment.getExpireTime(), expireTime);

      // TEARDOWN
   }

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
      assertUnit(fragment.isCollisionImmune());

      // TEARDOWN
   }

   void update_decrementsImmunity()
   {
      // SETUP
      Position position;
      position.x = 1000.0;
      position.y = 2000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment(position, velocity, 100.0);

      // EXERCISE
      fragment.update(1.0);

      // VERIFY - Should still have 2 frames remaining
      assertUnit(fragment.isCollisionImmune());

      // TEARDOWN
   }

   void update_decrementsImmunityToZero()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment(position, velocity, 100.0);

      // EXERCISE - Update COLLISION_IMMUNITY_FRAMES times
      fragment.update(1.0);
      fragment.update(1.0);
      fragment.update(1.0);

      // VERIFY - Immunity should now be expired
      assertUnit(!fragment.isCollisionImmune());

      // TEARDOWN
   }

   void isCollisionImmune_trueWhenActive()
   {
      // SETUP
      Position position;
      position.x = 1000.0;
      position.y = 2000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment(position, velocity, 100.0);

      // EXERCISE

      // VERIFY - Just created, should have full immunity
      assertUnit(fragment.isCollisionImmune());

      // TEARDOWN
   }

   void isCollisionImmune_falseWhenExpired()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment(position, velocity, 100.0);
      fragment.update(1.0);
      fragment.update(1.0);
      fragment.update(1.0);

      // EXERCISE

      // VERIFY - After 3 updates, immunity should be gone
      assertUnit(!fragment.isCollisionImmune());

      // TEARDOWN
   }

   void isExpired_falseWhenAgeIsLess()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment(position, velocity, 100.0);
      fragment.update(1.0);

      // EXERCISE

      // VERIFY - Age is 1.0, expire time is 100.0
      assertUnit(!fragment.isExpired());

      // TEARDOWN
   }

   void isExpired_trueWhenLifetimeExceeded()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment(position, velocity, 5.0);

      // EXERCISE - Age past expire time
      for (int i = 0; i < 6; ++i)
      {
         fragment.update(1.0);
      }

      // VERIFY - Age is 6.0, expire time is 5.0
      assertUnit(fragment.isExpired());

      // TEARDOWN
   }

   void isExpired_trueWhenLifetimeEqual()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment(position, velocity, 5.0);

      // EXERCISE - Age exactly to expire time
      for (int i = 0; i < 5; ++i)
      {
         fragment.update(1.0);
      }

      // VERIFY - Age is 5.0, expire time is 5.0
      assertUnit(fragment.isExpired());

      // TEARDOWN
   }

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

   void update_incrementsAge()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 2000000.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Fragment fragment(position, velocity, 100.0);

      // EXERCISE
      fragment.update(1.0);
      fragment.update(1.0);
      fragment.update(1.0);

      // VERIFY - Should have aged 3 seconds
      assertEquals(fragment.getAge(), 3.0);

      // TEARDOWN
   }
};
