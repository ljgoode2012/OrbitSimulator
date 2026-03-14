#pragma once

#include <cmath>
#include <memory>

#include "satellite.h"
#include "unitTest.h"

class HubbleDouble : public Hubble
{
public:
   explicit HubbleDouble(double phaseRadians = 0.0) : Hubble(phaseRadians) {}
   double getVelocityDX() const { return getVelocity().dx; }
   double getVelocityDY() const { return getVelocity().dy; }
};

class GPSDouble : public GPS
{
public:
   explicit GPSDouble(double phaseRadians = 0.0) : GPS(phaseRadians) {}
   double getVelocityDX() const { return getVelocity().dx; }
   double getVelocityDY() const { return getVelocity().dy; }
};

class TestSatellite : public UnitTest
{
public:
   void run()
   {
      hubble_orbitRadiusAndSpeed();
      gps_orbitRadiusAndSpeed();
      gps_phaseControlsInitialPositionAndVelocity();
      sputnik_orbitIsFarthest();
      satellite_setIsDefunctTrue_assignsRandomSpinRate();
      satellite_setIsDefunctFalse_resetsSpinRate();
      report("Satellite");
   }

private:
   static constexpr double EARTH_MU = 3.986004418e14;
   static constexpr double HALF_PI = 1.57079632679489661923;
   static constexpr double MIN_SPIN_RATE = 0.001;
   static constexpr double MAX_SPIN_RATE = 0.010;

   static double computeRadius(const Position& position)
   {
      return std::sqrt(position.getMetersX() * position.getMetersX() +
                       position.getMetersY() * position.getMetersY());
   }

   static double computeSpeed(double dx, double dy)
   {
      return std::sqrt(dx * dx + dy * dy);
   }

   static double circularOrbitSpeed(double radiusMeters)
   {
      return std::sqrt(EARTH_MU / radiusMeters);
   }

   void hubble_orbitRadiusAndSpeed()
   {
      // SETUP
      std::unique_ptr<HubbleDouble> hubblePtr(new HubbleDouble);
      HubbleDouble& hubble = *hubblePtr;

      // EXERCISE
      const double radius = computeRadius(hubble.getPosition());
      const double speed = computeSpeed(hubble.getVelocityDX(), hubble.getVelocityDY());

      // VERIFY
      assertEqualsTolerance(radius, Hubble::ORBIT_RADIUS_METERS, 0.001);
      assertEqualsTolerance(speed, circularOrbitSpeed(Hubble::ORBIT_RADIUS_METERS), 0.001);

      // TEARDOWN
      hubblePtr.reset();
   }

   void gps_orbitRadiusAndSpeed()
   {
      // SETUP
      std::unique_ptr<GPSDouble> gpsPtr(new GPSDouble);
      GPSDouble& gps = *gpsPtr;

      // EXERCISE
      const double radius = computeRadius(gps.getPosition());
      const double speed = computeSpeed(gps.getVelocityDX(), gps.getVelocityDY());

      // VERIFY
      assertEqualsTolerance(radius, GPS::ORBIT_RADIUS_METERS, 0.001);
      assertEqualsTolerance(speed, circularOrbitSpeed(GPS::ORBIT_RADIUS_METERS), 0.001);

      // TEARDOWN
      gpsPtr.reset();
   }

   void gps_phaseControlsInitialPositionAndVelocity()
   {
      // SETUP
      std::unique_ptr<GPSDouble> gpsPtr(new GPSDouble(HALF_PI));
      GPSDouble& gps = *gpsPtr;
      const double expectedSpeed = circularOrbitSpeed(GPS::ORBIT_RADIUS_METERS);

      // EXERCISE
      const Position& position = gps.getPosition();

      // VERIFY
      assertEqualsTolerance(position.getMetersX(), 0.0, 0.001);
      assertEqualsTolerance(position.getMetersY(), GPS::ORBIT_RADIUS_METERS, 0.001);
      assertEqualsTolerance(gps.getVelocityDX(), -expectedSpeed, 0.001);
      assertEqualsTolerance(gps.getVelocityDY(), 0.0, 0.001);

      // TEARDOWN
      gpsPtr.reset();
   }

   void sputnik_orbitIsFarthest()
   {
      // SETUP

      // EXERCISE

      // VERIFY
      assertUnit(Sputnik::ORBIT_RADIUS_METERS > GPS::ORBIT_RADIUS_METERS);
      assertUnit(Sputnik::ORBIT_RADIUS_METERS > Hubble::ORBIT_RADIUS_METERS);
      assertUnit(Sputnik::ORBIT_RADIUS_METERS > Starlink::ORBIT_RADIUS_METERS);
      assertUnit(Sputnik::ORBIT_RADIUS_METERS > CrewDragon::ORBIT_RADIUS_METERS);

      // TEARDOWN
   }

   void satellite_setIsDefunctTrue_assignsRandomSpinRate()
   {
      // SETUP
      std::unique_ptr<Satellite> satellitePtr(new Satellite);
      Satellite& satellite = *satellitePtr;
      satellite.setIsDefunct(false);

      // EXERCISE
      satellite.setIsDefunct(true);
      const double angularVelocity = satellite.getAngularVelocity();
      const double absoluteSpinRate = std::abs(angularVelocity);

      // VERIFY
      assertUnit(satellite.getIsDefunct());
      assertUnit(absoluteSpinRate >= MIN_SPIN_RATE);
      assertUnit(absoluteSpinRate <= MAX_SPIN_RATE);

      // TEARDOWN
      satellitePtr.reset();
   }

   void satellite_setIsDefunctFalse_resetsSpinRate()
   {
      // SETUP
      std::unique_ptr<Satellite> satellitePtr(new Satellite);
      Satellite& satellite = *satellitePtr;
      satellite.setIsDefunct(true);

      // EXERCISE
      satellite.setIsDefunct(false);

      // VERIFY
      assertUnit(!satellite.getIsDefunct());
      assertEquals(satellite.getAngularVelocity(), 0.0);

      // TEARDOWN
      satellitePtr.reset();
   }
};
