#pragma once

#include "breakableEntity.h"
#include "timedEntity.h"
#include "unitTest.h"

class TestEntity : public UnitTest
{
public:
   void run()
   {
      construct_default();
      construct_nonDefault();
      update_zeroPositionNoGravity();
      update_nonZeroPositionChangesState();
      update_nonZeroAngularVelocityChangesRotation();
      report("Entity");
   }

private:
   void construct_default()
   {
      Entity entity;
      assertEquals(entity.getPosition().getMetersX(), 0.0);
      assertEquals(entity.getPosition().getMetersY(), 0.0);
      assertEquals(entity.getVelocity().dx, 0.0);
      assertEquals(entity.getVelocity().dy, 0.0);
      assertEquals(entity.getRotation(), 0.0);
      assertEquals(entity.getAngularVelocity(), 0.0);
   }

   void construct_nonDefault()
   {
      Position position(10.0, 20.0);
      Velocity velocity(30.0, 40.0);
      Entity entity(position, velocity, 1.5, -0.25);

      assertEquals(entity.getPosition().getMetersX(), 10.0);
      assertEquals(entity.getPosition().getMetersY(), 20.0);
      assertEquals(entity.getVelocity().dx, 30.0);
      assertEquals(entity.getVelocity().dy, 40.0);
      assertEquals(entity.getRotation(), 1.5);
      assertEquals(entity.getAngularVelocity(), -0.25);
   }

   void update_zeroPositionNoGravity()
   {
      Entity entity;
      entity.setVelocity(Velocity(10.0, -20.0));

      entity.update(2.0);
      assertEquals(entity.getPosition().getMetersX(), 20.0);
      assertEquals(entity.getPosition().getMetersY(), -40.0);
      assertEquals(entity.getVelocity().dx, 10.0);
      assertEquals(entity.getVelocity().dy, -20.0);
   }

   void update_nonZeroPositionChangesState()
   {
      Entity entity(Position(7000000.0, 0.0), Velocity(0.0, 7500.0));

      entity.update(1.0);
      assertUnit(entity.getVelocity().dx < 0.0);
      assertEqualsTolerance(entity.getVelocity().dy, 7500.0, 0.001);
      assertUnit(entity.getPosition().getMetersX() < 7000000.0);
      assertUnit(entity.getPosition().getMetersY() > 0.0);
   }

   void update_nonZeroAngularVelocityChangesRotation()
   {
      Entity entity;
      entity.setAngularVelocity(0.5);

      entity.update(2.0);

      assertEqualsTolerance(entity.getRotation(), 1.0, 0.0001);
   }
};

class TestBreakableEntity : public UnitTest
{
public:
   void run()
   {
      construct_default();
      construct_nonDefault();
      setIsHit_true();
      setIsHit_false();
      report("BreakableEntity");
   }

private:
   void construct_default()
   {
      BreakableEntity entity;
      assertUnit(!entity.getIsHit());
      assertEquals(entity.getPosition().getMetersX(), 0.0);
      assertEquals(entity.getPosition().getMetersY(), 0.0);
   }

   void construct_nonDefault()
   {
      BreakableEntity entity(Position(100.0, 200.0), Velocity(3.0, 4.0));
      assertUnit(!entity.getIsHit());
      assertEquals(entity.getPosition().getMetersX(), 100.0);
      assertEquals(entity.getPosition().getMetersY(), 200.0);
      assertEquals(entity.getVelocity().dx, 3.0);
      assertEquals(entity.getVelocity().dy, 4.0);
   }

   void setIsHit_true()
   {
      BreakableEntity entity;
      entity.setIsHit();
      assertUnit(entity.getIsHit());
   }

   void setIsHit_false()
   {
      BreakableEntity entity;
      entity.setIsHit();
      entity.setIsHit(false);
      assertUnit(!entity.getIsHit());
   }
};

class TestTimedEntity : public UnitTest
{
public:
   void run()
   {
      construct_default();
      construct_nonDefault();
      updateTimer_increasesAge();
      update_increasesAgeAndMovesEntity();
      report("TimedEntity");
   }

private:
   void construct_default()
   {
      TimedEntity entity;
      assertEquals(entity.getAge(), 0.0);
      assertEquals(entity.getExpireTime(), 0.0);
      assertUnit(!entity.isExpired());
   }

   void construct_nonDefault()
   {
      TimedEntity entity(Position(1.0, 2.0), Velocity(3.0, 4.0), 10.0);
      assertEquals(entity.getPosition().getMetersX(), 1.0);
      assertEquals(entity.getPosition().getMetersY(), 2.0);
      assertEquals(entity.getVelocity().dx, 3.0);
      assertEquals(entity.getVelocity().dy, 4.0);
      assertEquals(entity.getAge(), 0.0);
      assertEquals(entity.getExpireTime(), 10.0);
   }

   void updateTimer_increasesAge()
   {
      TimedEntity entity;
      entity.setExpireTime(10.0);

      entity.updateTimer(4.0);
      assertEquals(entity.getAge(), 4.0);
      assertUnit(!entity.isExpired());

      entity.updateTimer(6.0);
      assertEquals(entity.getAge(), 10.0);
      assertUnit(entity.isExpired());
   }

   void update_increasesAgeAndMovesEntity()
   {
      TimedEntity entity(Position(7000000.0, 0.0), Velocity(0.0, 7500.0), 2.0);

      entity.update(1.0);
      assertEquals(entity.getAge(), 1.0);
      assertUnit(entity.getPosition().getMetersX() < 7000000.0);
      assertUnit(entity.getPosition().getMetersY() > 0.0);
      assertUnit(!entity.isExpired());

      entity.update(1.0);
      assertEquals(entity.getAge(), 2.0);
      assertUnit(entity.getPosition().getMetersX() < 7000000.0);
      assertUnit(entity.getPosition().getMetersY() > 0.0);
      assertUnit(entity.isExpired());
   }
};
