#pragma once

#include <cmath>
#include <memory>

#include "breakableEntity.h"
#include "timedEntity.h"
#include "unitTest.h"
#include "constants.h"  // Add this include

class EntityDouble : public Entity
{
public:
   EntityDouble() : Entity() {}
   EntityDouble(const Position& position,
                const Velocity& velocity,
                const Angle& rotation,
                double angularVelocity)
      : Entity(position, velocity, rotation, angularVelocity)
   {
   }

   double getVelocityDX() const { return getVelocity().dx; }
   double getVelocityDY() const { return getVelocity().dy; }
   void setRotationRadians(double radians) { setRotation(Angle(radians)); }
};

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
      report("Entity");
   }

private:
   static constexpr double TWO_PI = 2.0 * M_PI;  // Use M_PI from constants.h

   void construct_default()
   {
      // SETUP
      std::unique_ptr<EntityDouble> entityPtr(new EntityDouble);
      EntityDouble& entity = *entityPtr;

      // EXERCISE

      // VERIFY
      assertEquals(entity.getPosition().getMetersX(), 0.0);
      assertEquals(entity.getPosition().getMetersY(), 0.0);
      assertEquals(entity.getVelocityDX(), 0.0);
      assertEquals(entity.getVelocityDY(), 0.0);
      assertEquals(entity.getRotation(), 0.0);
      assertEquals(entity.getAngularVelocity(), 0.0);

      // TEARDOWN
      entityPtr.reset();
   }

   void construct_nonDefault()
   {
      // SETUP
      Position position;
      position.setMetersX(10.0);
      position.setMetersY(20.0);
      Velocity velocity;
      velocity.dx = 30.0;
      velocity.dy = 40.0;
      const Angle rotation(1.5);
      const double angularVelocity = -0.25;
      std::unique_ptr<EntityDouble> entityPtr(
         new EntityDouble(position, velocity, rotation, angularVelocity));
      EntityDouble& entity = *entityPtr;

      // EXERCISE

      // VERIFY
      assertEquals(entity.getPosition().getMetersX(), 10.0);
      assertEquals(entity.getPosition().getMetersY(), 20.0);
      assertEquals(entity.getVelocityDX(), 30.0);
      assertEquals(entity.getVelocityDY(), 40.0);
      assertEquals(entity.getRotation(), 1.5);
      assertEquals(entity.getAngularVelocity(), -0.25);

      // TEARDOWN
      entityPtr.reset();
   }

   void setPosition_updatesPosition()
   {
      // SETUP
      std::unique_ptr<EntityDouble> entityPtr(new EntityDouble);
      EntityDouble& entity = *entityPtr;
      Position position;
      position.setMeters(1234.5, -6789.0);

      // EXERCISE
      entity.setPosition(position);

      // VERIFY
      assertEquals(entity.getPosition().getMetersX(), 1234.5);
      assertEquals(entity.getPosition().getMetersY(), -6789.0);

      // TEARDOWN
      entityPtr.reset();
   }

   void setVelocity_updatesVelocity()
   {
      // SETUP
      std::unique_ptr<EntityDouble> entityPtr(new EntityDouble);
      EntityDouble& entity = *entityPtr;
      Velocity velocity;
      velocity.dx = -101.0;
      velocity.dy = 202.0;

      // EXERCISE
      entity.setVelocity(velocity);

      // VERIFY
      assertEquals(entity.getVelocityDX(), -101.0);
      assertEquals(entity.getVelocityDY(), 202.0);

      // TEARDOWN
      entityPtr.reset();
   }

   void setAngularVelocity_updatesAngularVelocity()
   {
      // SETUP
      std::unique_ptr<EntityDouble> entityPtr(new EntityDouble);
      EntityDouble& entity = *entityPtr;

      // EXERCISE
      entity.setAngularVelocity(0.75);

      // VERIFY
      assertEquals(entity.getAngularVelocity(), 0.75);

      // TEARDOWN
      entityPtr.reset();
   }

   void update_zeroPositionNoGravity()
   {
      // SETUP
      std::unique_ptr<EntityDouble> entityPtr(new EntityDouble);
      EntityDouble& entity = *entityPtr;
      entity.setVelocity(Velocity(10.0, -20.0));

      // EXERCISE
      entity.update(2.0);

      // VERIFY
      assertEquals(entity.getPosition().getMetersX(), 20.0);
      assertEquals(entity.getPosition().getMetersY(), -40.0);
      assertEquals(entity.getVelocityDX(), 10.0);
      assertEquals(entity.getVelocityDY(), -20.0);

      // TEARDOWN
      entityPtr.reset();
   }

   void update_nonZeroPositionChangesState()
   {
      // SETUP
      Position position;
      position.setMeters(7000000.0, 0.0);
      Velocity velocity;
      velocity.dx = 0.0;
      velocity.dy = 7500.0;
      std::unique_ptr<EntityDouble> entityPtr(
         new EntityDouble(position, velocity, Angle(0.0), 0.0));
      EntityDouble& entity = *entityPtr;

      // EXERCISE
      entity.update(1.0);

      // VERIFY
      assertUnit(entity.getVelocityDX() < 0.0);
      assertEqualsTolerance(entity.getVelocityDY(), 7500.0, 0.001);
      assertUnit(entity.getPosition().getMetersX() < 7000000.0);
      assertUnit(entity.getPosition().getMetersY() > 0.0);

      // TEARDOWN
      entityPtr.reset();
   }

   void update_nonZeroAngularVelocityChangesRotation()
   {
      // SETUP
      std::unique_ptr<EntityDouble> entityPtr(new EntityDouble);
      EntityDouble& entity = *entityPtr;
      entity.setAngularVelocity(0.5);

      // EXERCISE
      entity.update(2.0);

      // VERIFY
      assertEqualsTolerance(entity.getRotation(), 1.0, 0.0001);

      // TEARDOWN
      entityPtr.reset();
   }

   void update_rotationWrapsPositive()
   {
      // SETUP
      std::unique_ptr<EntityDouble> entityPtr(new EntityDouble);
      EntityDouble& entity = *entityPtr;
      entity.setRotationRadians(TWO_PI - 0.25);
      entity.setAngularVelocity(1.0);

      // EXERCISE
      entity.update(0.5);

      // VERIFY
      assertEqualsTolerance(entity.getRotation(), 0.25, 0.0001);

      // TEARDOWN
      entityPtr.reset();
   }

   void update_rotationWrapsNegative()
   {
      // SETUP
      std::unique_ptr<EntityDouble> entityPtr(new EntityDouble);
      EntityDouble& entity = *entityPtr;
      entity.setRotationRadians(0.20);
      entity.setAngularVelocity(-1.0);
      const double expectedRotation = TWO_PI - 0.30;

      // EXERCISE
      entity.update(0.5);

      // VERIFY
      assertEqualsTolerance(entity.getRotation(), expectedRotation, 0.0001);

      // TEARDOWN
      entityPtr.reset();
   }
   void update_negativeTimeDelta()
   {
      // SETUP
      EntityDouble entity;
      entity.setPosition(Position(0.0, 0.0));
      entity.setVelocity(Velocity(10.0, 20.0));

      // EXERCISE
      entity.update(-1.0);

      // VERIFY - Entity should move backwards (reverse time)
      assertEquals(entity.getPosition().getMetersX(), -10.0); // 0 + (10 * -1)
      assertEquals(entity.getPosition().getMetersY(), -20.0); // 0 + (20 * -1)
      assertEquals(entity.getVelocityDX(),
                   10.0); // Velocity unchanged (no gravity at origin)
      assertEquals(entity.getVelocityDY(), 20.0); // Velocity unchanged
   }
   void update_zeroTimeDelta()
   {
      // SETUP
      EntityDouble entity;
      entity.setPosition(Position(100.0, 200.0));
      entity.setVelocity(Velocity(10.0, 20.0));
      entity.setAngularVelocity(0.5);

      // EXERCISE
      entity.update(0.0); // Zero time - nothing should change

      // VERIFY
      assertEquals(entity.getPosition().getMetersX(), 100.0);
      assertEquals(entity.getPosition().getMetersY(), 200.0);
      assertEquals(entity.getVelocityDX(), 10.0);
      assertEquals(entity.getVelocityDY(), 20.0);
      assertEquals(entity.getRotation(), 0.0);
   }
};

class BreakableEntityDouble : public BreakableEntity
{
public:
   BreakableEntityDouble() : BreakableEntity() {}
   BreakableEntityDouble(const Position& position, const Velocity& velocity)
      : BreakableEntity(position, velocity)
   {
   }
   double getVelocityDX() const { return getVelocity().dx; }
   double getVelocityDY() const { return getVelocity().dy; }
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
      // SETUP
      std::unique_ptr<BreakableEntity> entityPtr(new BreakableEntity);
      BreakableEntity& entity = *entityPtr;

      // EXERCISE

      // VERIFY
      assertUnit(!entity.getIsHit());
      assertEquals(entity.getPosition().getMetersX(), 0.0);
      assertEquals(entity.getPosition().getMetersY(), 0.0);

      // TEARDOWN
      entityPtr.reset();
   }

   void construct_nonDefault()
   {
      // SETUP
      Position position;
      position.setMeters(100.0, 200.0);
      Velocity velocity;
      velocity.dx = 3.0;
      velocity.dy = 4.0;
      std::unique_ptr<BreakableEntityDouble> entityPtr(
         new BreakableEntityDouble(position, velocity));
      BreakableEntityDouble& entity = *entityPtr;

      // EXERCISE

      // VERIFY
      assertUnit(!entity.getIsHit());
      assertEquals(entity.getPosition().getMetersX(), 100.0);
      assertEquals(entity.getPosition().getMetersY(), 200.0);
      assertEquals(entity.getVelocityDX(), 3.0);
      assertEquals(entity.getVelocityDY(), 4.0);

      // TEARDOWN
      entityPtr.reset();
   }

   void setIsHit_true()
   {
      // SETUP
      std::unique_ptr<BreakableEntity> entityPtr(new BreakableEntity);
      BreakableEntity& entity = *entityPtr;

      // EXERCISE
      entity.setIsHit();

      // VERIFY
      assertUnit(entity.getIsHit());

      // TEARDOWN
      entityPtr.reset();
   }

   void setIsHit_false()
   {
      // SETUP
      std::unique_ptr<BreakableEntity> entityPtr(new BreakableEntity);
      BreakableEntity& entity = *entityPtr;
      entity.setIsHit();

      // EXERCISE
      entity.setIsHit(false);

      // VERIFY
      assertUnit(!entity.getIsHit());

      // TEARDOWN
      entityPtr.reset();
   }
};

class TimedEntityDouble : public TimedEntity
{
public:
   TimedEntityDouble() : TimedEntity() {}
   TimedEntityDouble(const Position& position, const Velocity& velocity, double expireTime)
      : TimedEntity(position, velocity, expireTime)
   {
   }
   double getVelocityDX() const { return getVelocity().dx; }
   double getVelocityDY() const { return getVelocity().dy; }
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
      // SETUP
      std::unique_ptr<TimedEntityDouble> entityPtr(new TimedEntityDouble);
      TimedEntityDouble& entity = *entityPtr;

      // EXERCISE

      // VERIFY
      assertEquals(entity.getAge(), 0.0);
      assertEquals(entity.getExpireTime(), 0.0);
      assertUnit(!entity.isExpired());

      // TEARDOWN
      entityPtr.reset();
   }

   void construct_nonDefault()
   {
      // SETUP
      Position position;
      position.setMeters(1.0, 2.0);
      Velocity velocity;
      velocity.dx = 3.0;
      velocity.dy = 4.0;
      std::unique_ptr<TimedEntityDouble> entityPtr(
         new TimedEntityDouble(position, velocity, 10.0));
      TimedEntityDouble& entity = *entityPtr;

      // EXERCISE

      // VERIFY
      assertEquals(entity.getPosition().getMetersX(), 1.0);
      assertEquals(entity.getPosition().getMetersY(), 2.0);
      assertEquals(entity.getVelocityDX(), 3.0);
      assertEquals(entity.getVelocityDY(), 4.0);
      assertEquals(entity.getAge(), 0.0);
      assertEquals(entity.getExpireTime(), 10.0);

      // TEARDOWN
      entityPtr.reset();
   }

   void updateTimer_increasesAge()
   {
      // SETUP
      std::unique_ptr<TimedEntityDouble> entityPtr(new TimedEntityDouble);
      TimedEntityDouble& entity = *entityPtr;
      entity.setExpireTime(10.0);

      // EXERCISE
      entity.updateTimer(4.0);

      // VERIFY
      assertEquals(entity.getAge(), 4.0);
      assertUnit(!entity.isExpired());

      // TEARDOWN
      entityPtr.reset();
   }

   void update_increasesAgeAndMovesEntity()
   {
      // SETUP
      std::unique_ptr<TimedEntityDouble> entityPtr(
         new TimedEntityDouble(Position(7000000.0, 0.0), Velocity(0.0, 7500.0), 2.0));
      TimedEntityDouble& entity = *entityPtr;

      // EXERCISE
      entity.update(1.0);

      // VERIFY
      assertEquals(entity.getAge(), 1.0);
      assertUnit(entity.getPosition().getMetersX() < 7000000.0);
      assertUnit(entity.getPosition().getMetersY() > 0.0);
      assertUnit(!entity.isExpired());

      // TEARDOWN
      entityPtr.reset();
   }
};
