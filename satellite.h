#pragma once
#include <cstdlib>

#include "breakableEntity.h"

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

   // SET Methods
   void setIsDefunct(bool isDefunct)
   {
      this->isDefunct = isDefunct;
      if (this->isDefunct)
         setRandomSpinRate();
      else
         setAngularVelocity(0.0);
   }

   // GET Methods
   bool getIsDefunct() const
   {
      return isDefunct;
   }
};

class Hubble : public Satellite
{
public:
   static constexpr double ORBIT_RADIUS_METERS = 42164000.0;

   explicit Hubble(double phaseRadians = 0.0) : Satellite()
   {
      initializeCircularOrbit(ORBIT_RADIUS_METERS, phaseRadians);
   }
};

class Starlink : public Satellite
{
public:
   static constexpr double ORBIT_RADIUS_METERS = 13020000.0;

   explicit Starlink(double phaseRadians = 0.0) : Satellite()
   {
      initializeCircularOrbit(ORBIT_RADIUS_METERS, phaseRadians);
   }
};

class CrewDragon : public Satellite
{
public:
   static constexpr double ORBIT_RADIUS_METERS = 8000000.0;

   explicit CrewDragon(double phaseRadians = 0.0) : Satellite()
   {
      initializeCircularOrbit(ORBIT_RADIUS_METERS, phaseRadians);
   }
};

class GPS : public Satellite
{
public:
   static constexpr double ORBIT_RADIUS_METERS = 42164000.0;

   explicit GPS(double phaseRadians = 0.0) : Satellite()
   {
      initializeCircularOrbit(ORBIT_RADIUS_METERS, phaseRadians);
   }
};

class Sputnik : public Satellite
{
public:
   // Artificially pushed out for visibility at this simulation scale.
   static constexpr double ORBIT_RADIUS_METERS = 50000000.0;

   explicit Sputnik(double phaseRadians = 0.0) : Satellite()
   {
      initializeCircularOrbit(ORBIT_RADIUS_METERS, phaseRadians);
   }
};
