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
   /*********************************************
    * name:    CONSTRUCT SETS POSITION
    * input:   position=(5000.0, 10000.0), velocity=(1000.0, 2000.0)
    * output:  projectile.position=(5000.0, 10000.0)
    *********************************************/
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
      assertEquals(projectile.position.x, 5000.0);
      assertEquals(projectile.position.y, 10000.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    CONSTRUCT SETS VELOCITY
    * input:   position=(5000.0, 10000.0), velocity=(1500.0, -3000.0)
    * output:  projectile.velocity=(1500.0, -3000.0)
    *********************************************/
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
      assertEquals(projectile.velocity.dx, 1500.0);
      assertEquals(projectile.velocity.dy, -3000.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    CONSTRUCT INITIALIZES AGE
    * input:   position=(5000.0, 10000.0), velocity=(1000.0, 2000.0)
    * output:  ageGameUnits=0
    *********************************************/
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

      // VERIFY
      assertEquals(projectile.ageGameUnits, 0);

      // TEARDOWN
   }

   /*********************************************
    * name:    GET COLLISION RADIUS PIXELS RETURNS 1
    * input:   projectile at position=(5000.0, 10000.0)
    * output:  radius=PROJECTILE_COLLISION_RADIUS_PIXELS
    *********************************************/
   void getCollisionRadiusPixels_returns1()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile;
      projectile.position = position;
      projectile.velocity = velocity;

      // EXERCISE
      double radius = projectile.getCollisionRadiusPixels();

      // VERIFY
      assertEquals(radius, PROJECTILE_COLLISION_RADIUS_PIXELS);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE INCREMENTS AGE
    * input:   projectile with ageGameUnits=0, dt=1.0
    * output:  ageGameUnits=1
    *********************************************/
   void update_incrementsAge()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile;
      projectile.position = position;
      projectile.velocity = velocity;

      // EXERCISE
      projectile.update(1.0);

      // VERIFY
      assertEquals(projectile.ageGameUnits, 1);

      // TEARDOWN
   }

   /*********************************************
    * name:    UPDATE INCREMENTS POSITION
    * input:   projectile at position=(0,0), velocity=(100,200), dt=2.0
    * output:  position=(200.0, 400.0)
    *********************************************/
   void update_incrementsPosition()
   {
      // SETUP
      Position position;
      position.x = 0.0;
      position.y = 0.0;
      Velocity velocity;
      velocity.dx = 100.0;
      velocity.dy = 200.0;
      Projectile projectile;
      projectile.position = position;
      projectile.velocity = velocity;

      // EXERCISE
      projectile.update(2.0);

      // VERIFY - Position should have moved: pos + vel * dt
      assertEquals(projectile.position.x, 200.0);
      assertEquals(projectile.position.y, 400.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    IS EXPIRED FALSE WHEN NEW
    * input:   projectile with ageGameUnits=0
    * output:  isExpired()=false
    *********************************************/
   void isExpired_falseWhenNew()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile;
      projectile.position = position;
      projectile.velocity = velocity;

      // EXERCISE
      bool result = projectile.isExpired();

      // VERIFY
      assertEquals(result, false);

      // TEARDOWN
   }

   /*********************************************
    * name:    IS EXPIRED FALSE BEFORE LIFETIME
    * input:   projectile with ageGameUnits=PROJECTILE_LIFETIME_GAME_UNITS-1
    * output:  isExpired()=false
    *********************************************/
   void isExpired_falseBeforeLifetime()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile;
      projectile.position = position;
      projectile.velocity = velocity;
      projectile.ageGameUnits = PROJECTILE_LIFETIME_GAME_UNITS - 1;

      // EXERCISE
      bool result = projectile.isExpired();

      // VERIFY
      assertEquals(result, false);

      // TEARDOWN
   }

   /*********************************************
    * name:    IS EXPIRED TRUE WHEN LIFETIME REACHED
    * input:   projectile with ageGameUnits=PROJECTILE_LIFETIME_GAME_UNITS
    * output:  isExpired()=true
    *********************************************/
   void isExpired_trueWhenLifetimeReached()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile;
      projectile.position = position;
      projectile.velocity = velocity;
      projectile.ageGameUnits = PROJECTILE_LIFETIME_GAME_UNITS;

      // EXERCISE
      bool result = projectile.isExpired();

      // VERIFY
      assertEquals(result, true);

      // TEARDOWN
   }

   /*********************************************
    * name:    IS EXPIRED TRUE WHEN LIFETIME EXCEEDED
    * input:   projectile with ageGameUnits=PROJECTILE_LIFETIME_GAME_UNITS+10
    * output:  isExpired()=true
    *********************************************/
   void isExpired_trueWhenLifetimeExceeded()
   {
      // SETUP
      Position position;
      position.x = 5000.0;
      position.y = 10000.0;
      Velocity velocity;
      velocity.dx = 1000.0;
      velocity.dy = 2000.0;
      Projectile projectile;
      projectile.position = position;
      projectile.velocity = velocity;
      projectile.ageGameUnits = PROJECTILE_LIFETIME_GAME_UNITS + 10;

      // EXERCISE
      bool result = projectile.isExpired();

      // VERIFY
      assertEquals(result, true);

      // TEARDOWN
   }
};
