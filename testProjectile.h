/***********************************************************************
 * Header File:
 *    Test Projectile : Test the Projectile class
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    All the unit tests for Projectile
 ************************************************************************/

#pragma once

#include "constants.h"
#include "projectile.h"
#include "unitTest.h"
#include <memory>

/*********************************************
 * TEST PROJECTILE
 * Unit tests for the Projectile class
 *********************************************/
class TestProjectile : public UnitTest
{
public:
   void run()
   {
      construct_setsPosition();
      construct_setsVelocity();
      construct_initializesAge();
      getCollisionRadiusPixels_returns1();
      update_incrementsAge();
      update_incrementsPosition();
      isExpired_falseWhenNew();
      isExpired_falseBeforeLifetime();
      isExpired_trueWhenLifetimeReached();
      isExpired_trueWhenLifetimeExceeded();
      report("Projectile");
   }

private:
   void construct_setsPosition()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;

      // EXERCISE
      Projectile projectile(position, velocity);

      // VERIFY
      assertEquals(projectile.getPosition().getMetersX(), 5000.0);
      assertEquals(projectile.getPosition().getMetersY(), 10000.0);

      // TEARDOWN
   }

   void construct_setsVelocity()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1500.0;
      velocity.dy = -3000.0;

      // EXERCISE
      Projectile projectile(position, velocity);

      // VERIFY
      assertEquals(projectile.getVelocity().dx, 1500.0);
      assertEquals(projectile.getVelocity().dy, -3000.0);

      // TEARDOWN
   }

   void construct_initializesAge()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;

      // EXERCISE
      Projectile projectile(position, velocity);

      // VERIFY - Should not be expired when first created
      assertUnit(!projectile.isExpired());

      // TEARDOWN
   }

   void getCollisionRadiusPixels_returns1()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile(position, velocity);

      // EXERCISE
      double radius = projectile.getCollisionRadiusPixels();

      // VERIFY
      assertEquals(radius, PROJECTILE_COLLISION_RADIUS_PIXELS);

      // TEARDOWN
   }

   void update_incrementsAge()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile(position, velocity);

      // EXERCISE
      projectile.update(1.0);

      // VERIFY - Should not be expired after just 1 update
      assertUnit(!projectile.isExpired());

      // TEARDOWN
   }

   void update_incrementsPosition()
   {
      // SETUP
      Position position;
      position.x = 0.0;
      position.y = 0.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Projectile projectile(position, velocity);

      // EXERCISE
      projectile.update(2.0);

      // VERIFY - Position should have moved: pos + vel * dt
      assertEquals(projectile.getPosition().getMetersX(), 200.0);
      assertEquals(projectile.getPosition().getMetersY(), 400.0);

      // TEARDOWN
   }

   void isExpired_falseWhenNew()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile(position, velocity);

      // EXERCISE

      // VERIFY - Brand new projectile should not be expired
      assertUnit(!projectile.isExpired());

      // TEARDOWN
   }

   void isExpired_falseBeforeLifetime()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile(position, velocity);

      // EXERCISE - Update to just before lifetime
      for (int i = 0; i < PROJECTILE_LIFETIME_GAME_UNITS - 1; ++i)
      {
         projectile.update(1.0);
      }

      // VERIFY - Should still be alive
      assertUnit(!projectile.isExpired());

      // TEARDOWN
   }

   void isExpired_trueWhenLifetimeReached()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile(position, velocity);

      // EXERCISE - Update exactly to lifetime
      for (int i = 0; i < PROJECTILE_LIFETIME_GAME_UNITS; ++i)
      {
         projectile.update(1.0);
      }

      // VERIFY - Should be expired when age >= lifetime
      assertUnit(projectile.isExpired());

      // TEARDOWN
   }

   void isExpired_trueWhenLifetimeExceeded()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile(position, velocity);

      // EXERCISE - Update past lifetime
      for (int i = 0; i < PROJECTILE_LIFETIME_GAME_UNITS + 10; ++i)
      {
         projectile.update(1.0);
      }

      // VERIFY - Should still be expired
      assertUnit(projectile.isExpired());

      // TEARDOWN
   }
};
