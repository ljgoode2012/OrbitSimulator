/***********************************************************************
 * Header File:
 *    Test Satellite : Test the Satellite class
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    All the unit tests for Satellite
 ************************************************************************/

#pragma once

#include <cmath>
#include <memory>

#include "satellite.h"
#include "unitTest.h"

/*********************************************
 * SATELLITE DOUBLE
 * A test double for Satellite to expose protected members
 *********************************************/
class SatelliteDouble : public Satellite
{
public:
   SatelliteDouble() : Satellite() {}
   void setRotationRadians(double radians) { setRotation(Angle(radians)); }
};

/*********************************************
 * HUBBLE DOUBLE
 * A test double for Hubble satellite
 *********************************************/
class HubbleDouble : public Hubble
{
public:
   explicit HubbleDouble(double phaseRadians = 0.0) : Hubble(phaseRadians) {}
};

/*********************************************
 * GPS DOUBLE
 * A test double for GPS satellite
 *********************************************/
class GPSDouble : public GPS
{
public:
   explicit GPSDouble(double phaseRadians = 0.0) : GPS(phaseRadians) {}
};

/*********************************************
 * TEST SATELLITE
 * Unit tests for the Satellite class
 *********************************************/
class TestSatellite : public UnitTest
{
public:
   void run()
   {
      hubble_orbitRadiusAndSpeed();
      gps_orbitRadiusAndSpeed();
      gps_phaseControlsInitialPositionAndVelocity();
      satellites_initializeWithCounterClockwiseEarthFacingRotation();
      satellite_setIsDefunctTrue_assignsRandomSpinRate();
      satellite_setIsDefunctFalse_resetsSpinRate();
      satellite_update_notDefunctFacesEarth();
      satellite_update_defunctUsesAngularVelocity();
      hubble_breakup_debrisInheritsVelocity();
      satellitePart_breakup_debrisInheritsPartVelocity();
      requirement_example_90DegreeKick();
      report("Satellite");
   }

private:
   static constexpr double EARTH_MU = 3.986004418e14;
   static constexpr double HALF_PI = 1.57079632679489661923;
   static constexpr double COUNTERCLOCKWISE_OFFSET_RADIANS = -HALF_PI;
   static constexpr double MIN_SPIN_RATE = 0.001;
   static constexpr double MAX_SPIN_RATE = 0.010;

   /*********************************************
    * EARTH FACING ROTATION RADIANS
    * Helper: Calculate the rotation angle to face Earth
    *********************************************/
   static double earthFacingRotationRadians(const Position& position)
   {
      // Calculate angle pointing to Earth and offset for counterclockwise
      // orientation
      double radians = std::atan2(-position.x, -position.y) +
                       COUNTERCLOCKWISE_OFFSET_RADIANS;

      // Normalize to [0, 2π) range if needed
      while (radians < 0.0)
         radians += 2.0 * M_PI;
      while (radians >= 2.0 * M_PI)
         radians -= 2.0 * M_PI;

      return radians;
   }

   /*********************************************
    * name:    HUBBLE ORBIT RADIUS AND SPEED
    * input:   Hubble satellite initialized at phase=0
    * output:  radius=42164000m (HUBBLE_ORBIT_RADIUS_METERS), speed≈3074.692 m/s
    *********************************************/
   void hubble_orbitRadiusAndSpeed()
   {
      // SETUP

      // EXERCISE
      HubbleDouble hubble;

      // VERIFY
      const double radius = std::sqrt(hubble.position.x * hubble.position.x +
                                      hubble.position.y * hubble.position.y);
      const double speed = std::sqrt(hubble.velocity.dx * hubble.velocity.dx +
                              hubble.velocity.dy * hubble.velocity.dy);
      
      assertEqualsTolerance(radius, HUBBLE_ORBIT_RADIUS_METERS, 0.001);
      assertEqualsTolerance(speed,
                            3074.692, // sqrt(MU / HUBBLE_ORBIT_RADIUS_METERS)
                            1.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    GPS ORBIT RADIUS AND SPEED
    * input:   GPS satellite initialized at phase=0
    * output:  radius=26560000m (GPS_ORBIT_RADIUS_METERS), speed≈3873.4 m/s
    *********************************************/
   void gps_orbitRadiusAndSpeed()
   {
      // SETUP

      // EXERCISE
      GPSDouble gps;

      // VERIFY
      double speed = std::sqrt(gps.velocity.dx * gps.velocity.dx +
                            gps.velocity.dy * gps.velocity.dy);
      double radius = std::sqrt(gps.position.x * gps.position.x + 
                             gps.position.y * gps.position.y);
      assertEqualsTolerance(radius, GPS_ORBIT_RADIUS_METERS, 0.001);
      assertEqualsTolerance(speed,
                         3873.4, // sqrt(MU / GPS_ORBIT_RADIUS_METERS)
                         1.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    GPS PHASE CONTROLS INITIAL POSITION AND VELOCITY
    * input:   GPS with phase=π/2 (1.5708 radians)
    * output:  position.y=26560000m, position.x≈0.0
    *********************************************/
   void gps_phaseControlsInitialPositionAndVelocity()
   {
      // SETUP
      const double phase = HALF_PI;

      // EXERCISE
      GPSDouble gps(phase);

      // VERIFY
      // At phase π/2, satellite should be on +Y axis
      assertEqualsTolerance(gps.position.y, GPS_ORBIT_RADIUS_METERS, 0.001);
      assertEqualsTolerance(gps.position.x, 0.0, 0.001);

      // TEARDOWN
   }

   /*********************************************
    * name:    SATELLITES INITIALIZE WITH COUNTERCLOCKWISE EARTH FACING ROTATION
    * input:   All satellite types at phase=0
    * output:  rotation.radians = atan2(-pos.x, -pos.y) - π/2, normalized to [0,
    *2π)
    *********************************************/
   void satellites_initializeWithCounterClockwiseEarthFacingRotation()
   {
      // SETUP

      // EXERCISE
      Hubble hubble;
      Starlink starlink;
      CrewDragon crewDragon;
      GPS gps;
      Sputnik sputnik;

      // VERIFY
      assertEqualsTolerance(
         hubble.rotation.radians,
         earthFacingRotationRadians(hubble.position), 0.0001);
      assertEqualsTolerance(
         starlink.rotation.radians,
         earthFacingRotationRadians(starlink.position), 0.0001);
      assertEqualsTolerance(
         crewDragon.rotation.radians,
         earthFacingRotationRadians(crewDragon.position), 0.0001);
      assertEqualsTolerance(
         gps.rotation.radians,
         earthFacingRotationRadians(gps.position), 0.0001);
      assertEqualsTolerance(
         sputnik.rotation.radians,
         earthFacingRotationRadians(sputnik.position), 0.0001);

      // TEARDOWN
   }

   /*********************************************
    * name:    SATELLITE SET IS DEFUNCT TRUE ASSIGNS RANDOM SPIN RATE
    * input:   satellite with angularVelocity=0, setIsDefunct(true)
    * output:  isDefunct=true, |angularVelocity| in [0.001, 0.010] rad/s
    *********************************************/
   void satellite_setIsDefunctTrue_assignsRandomSpinRate()
   {
      // SETUP
      SatelliteDouble satellite;
      satellite.angularVelocity = 0.0;

      // EXERCISE
      satellite.setIsDefunct(true);

      // VERIFY
      const double absoluteSpinRate = std::abs(satellite.angularVelocity);
      assertEquals(satellite.isDefunct, true);
      assertUnit(absoluteSpinRate >= MIN_SPIN_RATE);
      assertUnit(absoluteSpinRate <= MAX_SPIN_RATE);

      // TEARDOWN
   }

   /*********************************************
    * name:    SATELLITE SET IS DEFUNCT FALSE RESETS SPIN RATE
    * input:   satellite with isDefunct=true, setIsDefunct(false)
    * output:  isDefunct=false, angularVelocity=0.0
    *********************************************/
   void satellite_setIsDefunctFalse_resetsSpinRate()
   {
      // SETUP
      SatelliteDouble satellite;
      satellite.isDefunct = true;

      // EXERCISE
      satellite.setIsDefunct(false);

      // VERIFY
      assertEquals(satellite.isDefunct, false);
      assertEquals(satellite.angularVelocity, 0.0);

      // TEARDOWN
   }

   /*********************************************
    * name:    SATELLITE UPDATE NOT DEFUNCT FACES EARTH
    * input:   satellite at position=(26560000, 0), velocity=(0, 3873.44547),
    *rotation=π/2, dt=48s output:  rotation.radians≈3.134 (maintains
    *earth-facing)
    *********************************************/
   void satellite_update_notDefunctFacesEarth()
   {
      // SETUP
      Satellite satellite;
      satellite.isDefunct = false;
      satellite.position.x = GPS_ORBIT_RADIUS_METERS;
      satellite.position.y = 0.0;
      satellite.rotation.radians = M_PI_2; // 90 degrees
      satellite.velocity.dx = 0.0;
      satellite.velocity.dy = 3873.44547; // sqrt(MU / GPS_ORBIT_RADIUS_METERS)
      const double dt = SIM_SECONDS_PER_FRAME;

      // EXERCISE
      satellite.update(dt);

      // VERIFY
      assertEqualsTolerance(satellite.rotation.radians, 3.134, 0.01);

      // TEARDOWN
   }

    /*********************************************
    * name:    SATELLITE UPDATE DEFUNCT USES ANGULAR VELOCITY
    * input:   defunct satellite with rotation=1.0 rad, angularVelocity=0.5
    *rad/s, dt=2.0s output:  rotation.radians=2.0 (1.0 + 0.5*2.0)
    *********************************************/
   void satellite_update_defunctUsesAngularVelocity()
   {
      // SETUP
      SatelliteDouble satellite;
      satellite.rotation.radians = 1.0;
      satellite.angularVelocity = 0.5;
      satellite.isDefunct = true;
      const double dt = 2.0;
      const double expectedRotation = 1.0 + 0.5 * dt;

      // EXERCISE
      satellite.update(dt);

      // VERIFY
      assertEqualsTolerance(satellite.rotation.radians, expectedRotation, 0.0001);

      // TEARDOWN
   }

   /*********************************************
    * name:    HUBBLE BREAKUP DEBRIS INHERITS VELOCITY
    * input:   Hubble satellite at default orbit
    * output:  4 debris parts, each with kick magnitude in [5000, 9000] m/s
    *********************************************/
   void hubble_breakup_debrisInheritsVelocity()
   {
      // SETUP
      HubbleDouble hubble;
      const double parentDX = hubble.velocity.dx;
      const double parentDY = hubble.velocity.dy;
      std::vector<std::unique_ptr<Entity>> debris;

      // EXERCISE
      hubble.createBreakupDebris(debris);

      // VERIFY - Hubble creates 4 parts
      assertEquals(debris.size(), 4);

      for (const auto& entity : debris)
      {
         // Calculate the kick vector
         const double kickDX = entity->velocity.dx - parentDX;
         const double kickDY = entity->velocity.dy - parentDY;
         const double kickMagnitude = std::sqrt(kickDX * kickDX +
                                                kickDY * kickDY);
         
         // Verify kick magnitude is 5000-9000 m/s
         assertUnit(kickMagnitude >= 5000.0 - 0.1);
         assertUnit(kickMagnitude <= 9000.0 + 0.1);
      }

      // TEARDOWN
   }

   /*********************************************
    * name:    SATELLITE PART BREAKUP DEBRIS INHERITS PART VELOCITY
    * input:   SatellitePart at position=(1000000, 0), velocity=(8000, 6000), 3
    *fragments output:  3 fragments, each with kick magnitude in [5000, 9000]
    *m/s
    *********************************************/
   void satellitePart_breakup_debrisInheritsPartVelocity()
   {
      // SETUP
      Position position;
      position.x = 1000000.0;
      position.y = 0.0;
      
      Velocity partVelocity;
      partVelocity.dx = 8000.0;
      partVelocity.dy = 6000.0;
      const double partDX = 8000.0;
      const double partDY = 6000.0;
      
      SatellitePart part(position, partVelocity, Angle(), 
                         SatellitePart::GPS_CENTER, 7.0, 3);
      std::vector<std::unique_ptr<Entity>> fragments;

      // EXERCISE
      part.createBreakupDebris(fragments);

      // VERIFY - Should create 3 fragments
      assertEquals(static_cast<int>(fragments.size()), 3);

      for (const auto& entity : fragments)
      {
         // Calculate the kick vector applied to this fragment
         const double kickDX = entity->velocity.dx - partDX;
         const double kickDY = entity->velocity.dy - partDY;
         const double kickMagnitude = std::sqrt(kickDX * kickDX +
                                                kickDY * kickDY);
         
         // The KICK itself should be 5000-9000 m/s
         assertUnit(kickMagnitude >= 5000.0 - 0.1);
         assertUnit(kickMagnitude <= 9000.0 + 0.1);
      }

      // TEARDOWN
   }

   /*********************************************
    * name:    REQUIREMENT EXAMPLE 90 DEGREE KICK
    * input:   parent velocity=(3000, 4000) m/s, kick angle=90°, kick
    *magnitude=[5000, 9000] m/s output:  min result=(8000, 4000) m/s, max
    *result=(12000, 4000) m/s
    *********************************************/
   void requirement_example_90DegreeKick()
   {
      // SETUP - Reproduce the exact scenario from design document
      const double parentDX = 3000.0;
      const double parentDY = 4000.0;
      // parentSpeed = sqrt(3000² + 4000²) = sqrt(25000000) = 5000
      const double parentSpeed = std::sqrt(parentDX * parentDX +
                                           parentDY * parentDY);
   
      // EXERCISE - Manually calculate what WOULD happen with 90° kick
      const double angle90Deg = M_PI / 2.0;
      const double kickMin = 5000.0;
      const double kickMax = 9000.0;
   
      const double minResultDX = parentDX + std::sin(angle90Deg) * kickMin;
      const double minResultDY = parentDY + std::cos(angle90Deg) * kickMin;
   
      const double maxResultDX = parentDX + std::sin(angle90Deg) * kickMax;
      const double maxResultDY = parentDY + std::cos(angle90Deg) * kickMax;
   
      // VERIFY - Confirm the math matches the requirement
      assertEqualsTolerance(minResultDX, 8000.0, 0.1);
      assertEqualsTolerance(minResultDY, 4000.0, 0.1);
   
      assertEqualsTolerance(maxResultDX, 12000.0, 0.1);
      assertEqualsTolerance(maxResultDY, 4000.0, 0.1);

      // TEARDOWN
   }
};
