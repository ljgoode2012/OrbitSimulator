/***********************************************************************
 * Header File:
 *    Satellite : The representation of satellites in orbit
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Everything we need to know about satellites, parts, and fragments
 ************************************************************************/
#pragma once
#include <cstdlib>
#include <memory>
#include <vector>

#include "breakableEntity.h"
#include "constants.h"
#include "timedEntity.h"

/*********************************************
 * Fragment
 * A small piece of debris with a limited
 * lifetime and wild rotation
 *********************************************/
class Fragment : public TimedEntity
{
public:
   Fragment() : TimedEntity(), collisionImmunityFrames(0) {}
   Fragment(const Position& pos, const Velocity& vel, double expireTimeSeconds)
       : TimedEntity(pos, vel, expireTimeSeconds),
         collisionImmunityFrames(COLLISION_IMMUNITY_FRAMES)
   {
   }

   void draw(ogstream& gout) const override;
   void update(double dt) override;
   void setInitialRotation(const Angle& rotation) { setRotation(rotation); }
   bool isCollisionImmune() const { return collisionImmunityFrames > 0; }
   double getCollisionRadiusPixels() const override
   {
      return FRAGMENT_COLLISION_RADIUS_PIXELS;
   }

private:
   int collisionImmunityFrames;
};

/*********************************************
 * SatellitePart
 * A piece of a broken satellite that can
 * break into fragments
 *********************************************/
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

   SatellitePart()
       : BreakableEntity(), drawType(STARLINK_BODY), radiusPixels(0.0),
         fragmentsOnBreak(0), collisionImmunityFrames(0)
   {
   }
   SatellitePart(const Position& pos, const Velocity& vel,
                 const Angle& rotation, DrawType drawType, double radiusPixels,
                 int fragmentsOnBreak);

   void draw(ogstream& gout) const override;
   void update(double dt) override;
   double getCollisionRadiusPixels() const override { return radiusPixels; }
   int getFragmentsOnBreak() const { return fragmentsOnBreak; }
   bool isCollisionImmune() const { return collisionImmunityFrames > 0; }
   void createBreakupDebris(
      std::vector<std::unique_ptr<Entity>>& debris) const override;

private:
   DrawType drawType;
   double radiusPixels;
   int fragmentsOnBreak;
   int collisionImmunityFrames;
};

/*********************************************
 * Satellite
 * A satellite in orbit around Earth that
 * can break into parts when hit
 *********************************************/
class Satellite : public BreakableEntity
{
private:
   bool isDefunct;
   void setRandomSpinRate();

protected:
   void initializeCircularOrbit(double orbitalRadiusMeters,
                                double phaseRadians = 0.0);

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
      return SATELLITE_COLLISION_RADIUS_PIXELS;
   }
   virtual void createBreakupDebris(
      std::vector<std::unique_ptr<Entity>>& debris) const;

   // SET Methods
   void setIsDefunct(bool isDefunct);

   // GET Methods
   bool getIsDefunct() const { return isDefunct; }
};

void createFragmentsFromEntity(const Entity& parent, int fragmentCount,
                               std::vector<std::unique_ptr<Entity>>& debrisOut);

/*********************************************
 * Hubble
 * The Hubble Space Telescope
 *********************************************/
class Hubble : public Satellite
{
public:
   explicit Hubble(double phaseRadians = HUBBLE_DEFAULT_PHASE_RADIANS)
       : Satellite()
   {
      initializeCircularOrbit(HUBBLE_ORBIT_RADIUS_METERS, phaseRadians);
   }

   void draw(ogstream& gout) const override;
   double getCollisionRadiusPixels() const override
   {
      return HUBBLE_COLLISION_RADIUS_PIXELS;
   }
   void createBreakupDebris(
      std::vector<std::unique_ptr<Entity>>& debris) const override;
};

/*********************************************
 * Starlink
 * A SpaceX Starlink satellite
 *********************************************/
class Starlink : public Satellite
{
public:
   explicit Starlink(double phaseRadians = STARLINK_DEFAULT_PHASE_RADIANS)
       : Satellite()
   {
      initializeCircularOrbit(STARLINK_ORBIT_RADIUS_METERS, phaseRadians);
   }

   void draw(ogstream& gout) const override;
   double getCollisionRadiusPixels() const override
   {
      return STARLINK_COLLISION_RADIUS_PIXELS;
   }
   void createBreakupDebris(
      std::vector<std::unique_ptr<Entity>>& debris) const override;
};

/*********************************************
 * CrewDragon
 * A SpaceX Crew Dragon capsule
 *********************************************/
class CrewDragon : public Satellite
{
public:
   explicit CrewDragon(double phaseRadians = CREW_DRAGON_DEFAULT_PHASE_RADIANS)
       : Satellite()
   {
      initializeCircularOrbit(CREW_DRAGON_ORBIT_RADIUS_METERS, phaseRadians);
   }

   void draw(ogstream& gout) const override;
   double getCollisionRadiusPixels() const override
   {
      return CREW_DRAGON_COLLISION_RADIUS_PIXELS;
   }
   void createBreakupDebris(
      std::vector<std::unique_ptr<Entity>>& debris) const override;
};

/*********************************************
 * GPS
 * A Global Positioning System satellite
 *********************************************/
class GPS : public Satellite
{
public:
   explicit GPS(double phaseRadians = GPS_DEFAULT_PHASE_RADIANS) : Satellite()
   {
      initializeCircularOrbit(GPS_ORBIT_RADIUS_METERS, phaseRadians);
   }

   void draw(ogstream& gout) const override;
   double getCollisionRadiusPixels() const override
   {
      return GPS_COLLISION_RADIUS_PIXELS;
   }
   void createBreakupDebris(
      std::vector<std::unique_ptr<Entity>>& debris) const override;
};

/*********************************************
 * Sputnik
 * The first artificial satellite
 *********************************************/
class Sputnik : public Satellite
{
public:
   explicit Sputnik(double phaseRadians = SPUTNIK_DEFAULT_PHASE_RADIANS)
       : Satellite()
   {
      initializeCircularOrbit(SPUTNIK_ORBIT_RADIUS_METERS, phaseRadians);
   }

   void draw(ogstream& gout) const override;
   double getCollisionRadiusPixels() const override
   {
      return SPUTNIK_COLLISION_RADIUS_PIXELS;
   }
   void createBreakupDebris(
      std::vector<std::unique_ptr<Entity>>& debris) const override;
};
