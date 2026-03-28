#pragma once
#include <cstdlib>
#include <memory>
#include <vector>

#include "breakableEntity.h"
#include "timedEntity.h"

class Fragment : public TimedEntity
{
public:
   static constexpr double RADIUS_PIXELS = 2.0;
   static constexpr int INITIAL_COLLISION_IMMUNITY_FRAMES = 3;

   Fragment() : TimedEntity(), collisionImmunityFrames(0) {}
   Fragment(const Position& pos, const Velocity& vel, double expireTimeSeconds)
      : TimedEntity(pos, vel, expireTimeSeconds), collisionImmunityFrames(INITIAL_COLLISION_IMMUNITY_FRAMES)
   {
   }

   void draw(ogstream& gout) const override;
   void update(double dt) override;
   void setInitialRotation(const Angle& rotation)
   {
      setRotation(rotation);
   }
   bool isCollisionImmune() const
   {
      return collisionImmunityFrames > 0;
   }
   double getCollisionRadiusPixels() const override
   {
      return RADIUS_PIXELS;
   }

private:
   int collisionImmunityFrames;
};

class SatellitePart : public BreakableEntity
{
public:
   enum DrawType
   {
      STARLINK_BODY,
      STARLINK_ARRAY,
      CREW_DRAGON_CENTER,
      CREW_DRAGON_LEFT,
      CREW_DRAGON_RIGHT,
      HUBBLE_TELESCOPE,
      HUBBLE_COMPUTER,
      HUBBLE_LEFT,
      HUBBLE_RIGHT,
      GPS_CENTER,
      GPS_LEFT,
      GPS_RIGHT
   };

   static constexpr int INITIAL_COLLISION_IMMUNITY_FRAMES = 3;

   SatellitePart() : BreakableEntity(), drawType(STARLINK_BODY), radiusPixels(0.0), fragmentsOnBreak(0), collisionImmunityFrames(0) {}
   SatellitePart(const Position& pos,
                 const Velocity& vel,
                 const Angle& rotation,
                 DrawType drawType,
                 double radiusPixels,
                 int fragmentsOnBreak);

   void draw(ogstream& gout) const override;
   void update(double dt) override;
   double getCollisionRadiusPixels() const override
   {
      return radiusPixels;
   }
   int getFragmentsOnBreak() const
   {
      return fragmentsOnBreak;
   }
   bool isCollisionImmune() const
   {
      return collisionImmunityFrames > 0;
   }

private:
   DrawType drawType;
   double radiusPixels;
   int fragmentsOnBreak;
   int collisionImmunityFrames;
};

class Satellite : public BreakableEntity
{
private:
   bool isDefunct;
   void setRandomSpinRate();
protected:
   void initializeCircularOrbit(double orbitalRadiusMeters, double phaseRadians = 0.0);

public:
   // Constructors
   Satellite() : BreakableEntity(), isDefunct(false)
   {
      setIsDefunct((std::rand() % 5) == 0);
   }
   Satellite(const Position& pos, const Velocity& vel)
       : BreakableEntity(pos, vel), isDefunct(false)
   {
      // Has a one in five chance of being defunct.
      setIsDefunct((std::rand() % 5) == 0);
   }

   // Update the satellite's position and velocity using basic kinematics
   void update(double dt) override;
   void draw(ogstream& gout) const override;
   virtual double getCollisionRadiusPixels() const override
   {
      return 6.0;
   }
   virtual void createBreakupDebris(std::vector<std::unique_ptr<Entity>>& debris) const;

   // SET Methods
   void setIsDefunct(bool isDefunct);

   // GET Methods
   bool getIsDefunct() const
   {
      return isDefunct;
   }
};

void createFragmentsFromEntity(const Entity& parent,
                               int fragmentCount,
                               std::vector<std::unique_ptr<Entity>>& debrisOut);

class Hubble : public Satellite
{
public:
   static constexpr double ORBIT_RADIUS_METERS = 42164000.0;
   static constexpr double DEFAULT_PHASE_RADIANS = -1.57079632679489661923;

   explicit Hubble(double phaseRadians = DEFAULT_PHASE_RADIANS) : Satellite()
   {
      initializeCircularOrbit(ORBIT_RADIUS_METERS, phaseRadians);
   }

   void draw(ogstream& gout) const override;
   double getCollisionRadiusPixels() const override
   {
      return 10.0;
   }
   void createBreakupDebris(std::vector<std::unique_ptr<Entity>>& debris) const override;
};

class Starlink : public Satellite
{
public:
   static constexpr double ORBIT_RADIUS_METERS = 13020000.0;
   static constexpr double DEFAULT_PHASE_RADIANS = -1.57079632679489661923;

   explicit Starlink(double phaseRadians = DEFAULT_PHASE_RADIANS) : Satellite()
   {
      initializeCircularOrbit(ORBIT_RADIUS_METERS, phaseRadians);
   }

   void draw(ogstream& gout) const override;
   double getCollisionRadiusPixels() const override
   {
      return 6.0;
   }
   void createBreakupDebris(std::vector<std::unique_ptr<Entity>>& debris) const override;
};

class CrewDragon : public Satellite
{
public:
   static constexpr double ORBIT_RADIUS_METERS = 8000000.0;
   static constexpr double DEFAULT_PHASE_RADIANS = 1.57079632679489661923;

   explicit CrewDragon(double phaseRadians = DEFAULT_PHASE_RADIANS) : Satellite()
   {
      initializeCircularOrbit(ORBIT_RADIUS_METERS, phaseRadians);
   }

   void draw(ogstream& gout) const override;
   double getCollisionRadiusPixels() const override
   {
      return 8.0;
   }
   void createBreakupDebris(std::vector<std::unique_ptr<Entity>>& debris) const override;
};

class GPS : public Satellite
{
public:
   // Nominal GPS orbital radius from Earth's center (~26,560 km).
   static constexpr double ORBIT_RADIUS_METERS = 26560000.0;

   explicit GPS(double phaseRadians = 0.0) : Satellite()
   {
      initializeCircularOrbit(ORBIT_RADIUS_METERS, phaseRadians);
   }

   void draw(ogstream& gout) const override;
   double getCollisionRadiusPixels() const override
   {
      return 12.0;
   }
   void createBreakupDebris(std::vector<std::unique_ptr<Entity>>& debris) const override;
};

class Sputnik : public Satellite
{
public:
   // Artificially pushed out for visibility at this simulation scale.
   static constexpr double ORBIT_RADIUS_METERS = 50000000.0;
   static constexpr double DEFAULT_PHASE_RADIANS = 5.340707511102648;

   explicit Sputnik(double phaseRadians = DEFAULT_PHASE_RADIANS) : Satellite()
   {
      initializeCircularOrbit(ORBIT_RADIUS_METERS, phaseRadians);
   }

   void draw(ogstream& gout) const override;
   double getCollisionRadiusPixels() const override
   {
      return 4.0;
   }
   void createBreakupDebris(std::vector<std::unique_ptr<Entity>>& debris) const override;
};
