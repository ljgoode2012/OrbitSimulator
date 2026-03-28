#pragma once

#include <cmath>
#include <memory>

#include "satellite.h"
#include "unitTest.h"

class SatelliteDouble : public Satellite
{
public:
   SatelliteDouble() : Satellite() {}
   void setRotationRadians(double radians) { setRotation(Angle(radians)); }
};

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
      satellites_initializeWithCounterClockwiseEarthFacingRotation();
      satellite_setIsDefunctTrue_assignsRandomSpinRate();
      satellite_setIsDefunctFalse_resetsSpinRate();
      satellite_update_notDefunctFacesEarth();
      satellite_update_defunctUsesAngularVelocity();
      report("Satellite");
   }

private:
   static constexpr double EARTH_MU = 3.986004418e14;
   static constexpr double HALF_PI = 1.57079632679489661923;
   static constexpr double COUNTERCLOCKWISE_OFFSET_RADIANS = -HALF_PI;
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

   static double earthFacingRotationRadians(const Position& position)
   {
      Angle earthFacingRotation;
      earthFacingRotation.setDxDy(-position.getMetersX(),
                                  -position.getMetersY());
      earthFacingRotation.addRadians(COUNTERCLOCKWISE_OFFSET_RADIANS);
      return earthFacingRotation.getRadians();
   }

   void hubble_orbitRadiusAndSpeed()
   {
      // SETUP
      std::unique_ptr<HubbleDouble> hubblePtr(new HubbleDouble);
      HubbleDouble& hubble = *hubblePtr;

      // EXERCISE
      const double radius = computeRadius(hubble.getPosition());
      const double speed = computeSpeed(hubble.getVelocityDX(),
                                        hubble.getVelocityDY());

      // VERIFY
      assertEqualsTolerance(radius, HUBBLE_ORBIT_RADIUS_METERS, 0.001);
      assertEqualsTolerance(speed,
                            circularOrbitSpeed(HUBBLE_ORBIT_RADIUS_METERS),
                            0.001);

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
      const double speed = computeSpeed(gps.getVelocityDX(),
                                        gps.getVelocityDY());

      // VERIFY
      assertEqualsTolerance(radius, GPS_ORBIT_RADIUS_METERS, 0.001);
      assertEqualsTolerance(speed, circularOrbitSpeed(GPS_ORBIT_RADIUS_METERS),
                            0.001);

      // TEARDOWN
      gpsPtr.reset();
   }

   void gps_phaseControlsInitialPositionAndVelocity()
   {
      // SETUP
      std::unique_ptr<GPSDouble> gpsPtr(new GPSDouble(HALF_PI));
      GPSDouble& gps = *gpsPtr;
      const double expectedSpeed = circularOrbitSpeed(GPS_ORBIT_RADIUS_METERS);

      // EXERCISE
      const Position& position = gps.getPosition();

      // VERIFY
      assertEqualsTolerance(position.getMetersX(), 0.0, 0.001);
      assertEqualsTolerance(position.getMetersY(), GPS_ORBIT_RADIUS_METERS,
                            0.001);
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
      assertUnit(SPUTNIK_ORBIT_RADIUS_METERS > GPS_ORBIT_RADIUS_METERS);
      assertUnit(SPUTNIK_ORBIT_RADIUS_METERS > HUBBLE_ORBIT_RADIUS_METERS);
      assertUnit(SPUTNIK_ORBIT_RADIUS_METERS > STARLINK_ORBIT_RADIUS_METERS);
      assertUnit(SPUTNIK_ORBIT_RADIUS_METERS > CREW_DRAGON_ORBIT_RADIUS_METERS);

      // TEARDOWN
   }

   void satellites_initializeWithCounterClockwiseEarthFacingRotation()
   {
      // SETUP
      std::unique_ptr<Hubble> hubblePtr(new Hubble);
      std::unique_ptr<Starlink> starlinkPtr(new Starlink);
      std::unique_ptr<CrewDragon> crewDragonPtr(new CrewDragon);
      std::unique_ptr<GPS> gpsPtr(new GPS);
      std::unique_ptr<Sputnik> sputnikPtr(new Sputnik);

      // EXERCISE

      // VERIFY
      assertEqualsTolerance(
         hubblePtr->getRotation(),
         earthFacingRotationRadians(hubblePtr->getPosition()), 0.0001);
      assertEqualsTolerance(
         starlinkPtr->getRotation(),
         earthFacingRotationRadians(starlinkPtr->getPosition()), 0.0001);
      assertEqualsTolerance(
         crewDragonPtr->getRotation(),
         earthFacingRotationRadians(crewDragonPtr->getPosition()), 0.0001);
      assertEqualsTolerance(gpsPtr->getRotation(),
                            earthFacingRotationRadians(gpsPtr->getPosition()),
                            0.0001);
      assertEqualsTolerance(
         sputnikPtr->getRotation(),
         earthFacingRotationRadians(sputnikPtr->getPosition()), 0.0001);

      // TEARDOWN
      hubblePtr.reset();
      starlinkPtr.reset();
      crewDragonPtr.reset();
      gpsPtr.reset();
      sputnikPtr.reset();
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

   void satellite_update_notDefunctFacesEarth()
   {
      // SETUP
      std::unique_ptr<Satellite> satellitePtr(new Satellite);
      Satellite& satellite = *satellitePtr;
      satellite.setIsDefunct(false);

      // Set a non-zero position so the satellite can compute earth-facing
      // rotation
      Position testPosition;
      testPosition.setMeters(1000000.0, 2000000.0); // Some non-zero position
      satellite.setPosition(testPosition);

      const double dt = 48.0;

      // EXERCISE
      satellite.update(dt);

      // VERIFY
      assertEqualsTolerance(satellite.getRotation(),
                            earthFacingRotationRadians(satellite.getPosition()),
                            0.0001);

      // TEARDOWN
      satellitePtr.reset();
   }

   void satellite_update_defunctUsesAngularVelocity()
   {
      // SETUP
      std::unique_ptr<Satellite> satellitePtr(new Satellite);
      Satellite& satellite = *satellitePtr;
      satellite.setIsDefunct(true);
      const double dt = 48.0;
      const double initialRotation = satellite.getRotation();
      const double angularVelocity = satellite.getAngularVelocity();
      const double expectedRotation = initialRotation + angularVelocity * dt;

      // EXERCISE
      satellite.update(dt);

      // VERIFY
      assertEqualsTolerance(satellite.getRotation(), expectedRotation, 0.0001);

      // TEARDOWN
      satellitePtr.reset();
   }
};
