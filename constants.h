#pragma once

/*************************************************************
 * Mathematical Constants
 *************************************************************/
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 (M_PI / 2.0)
#endif

constexpr double TWO_PI = M_PI * 2.0;

/*************************************************************
 * Physics Constants
 *************************************************************/
// Earth's gravitational parameter (G * M) in m^3/s^2
constexpr double MU = 3.986004418e14;

// Length of one sidereal day (seconds)
constexpr double SIDEREAL_DAY_SECONDS = 86164.0905;

// Earth's rotation rate (radians/second)
constexpr double EARTH_ROTATION_RATE = TWO_PI / SIDEREAL_DAY_SECONDS;

/*************************************************************
 * Simulation Constants
 *************************************************************/
// Frames per second in the simulation
constexpr double FRAMES_PER_SECOND = 30.0;

// Frames per 1 simulated day (1 simulated day = 60 seconds of run time)
constexpr double FRAMES_PER_SIM_DAY = FRAMES_PER_SECOND * 60.0; // 1800.0

// Dilated seconds per frame (86400 seconds in a day)
constexpr double SIM_SECONDS_PER_FRAME = 86400.0 / FRAMES_PER_SIM_DAY; // 48.0

// Number of stars to generate
constexpr int NUM_STARS = 200;

// Number of GPS satellites to spawn
constexpr int GPS_SATELLITE_COUNT = 10;

/*************************************************************
 * Orbital Constants - Satellite Orbits
 *************************************************************/
// Geostationary orbit radius (~42,164 km)
constexpr double GEO_RADIUS_METERS = 42164000.0;

// Hubble orbit radius (same as GEO)
constexpr double HUBBLE_ORBIT_RADIUS_METERS = GEO_RADIUS_METERS;

// Starlink orbit radius (~13,020 km)
constexpr double STARLINK_ORBIT_RADIUS_METERS = 13020000.0;

// Crew Dragon orbit radius (~8,000 km)
constexpr double CREW_DRAGON_ORBIT_RADIUS_METERS = 8000000.0;

// GPS orbit radius (~26,560 km)
constexpr double GPS_ORBIT_RADIUS_METERS = 26560000.0;

// Sputnik orbit radius (artificially pushed out for visibility)
constexpr double SPUTNIK_ORBIT_RADIUS_METERS = 50000000.0;

/*************************************************************
 * Default Phase Angles (Starting Positions)
 *************************************************************/
constexpr double HUBBLE_DEFAULT_PHASE_RADIANS = -M_PI_2;     // -π/2
constexpr double STARLINK_DEFAULT_PHASE_RADIANS = -M_PI_2;   // -π/2
constexpr double CREW_DRAGON_DEFAULT_PHASE_RADIANS = M_PI_2; // π/2
constexpr double GPS_DEFAULT_PHASE_RADIANS = 0.0;
constexpr double SPUTNIK_DEFAULT_PHASE_RADIANS = 5.340707511102648;

/*************************************************************
 * Collision Radii (in pixels)
 *************************************************************/
constexpr double FRAGMENT_COLLISION_RADIUS_PIXELS = 2.0;
constexpr double SATELLITE_COLLISION_RADIUS_PIXELS = 6.0;
constexpr double HUBBLE_COLLISION_RADIUS_PIXELS = 10.0;
constexpr double STARLINK_COLLISION_RADIUS_PIXELS = 6.0;
constexpr double CREW_DRAGON_COLLISION_RADIUS_PIXELS = 7.0;
constexpr double GPS_COLLISION_RADIUS_PIXELS = 12.0;
constexpr double SPUTNIK_COLLISION_RADIUS_PIXELS = 4.0;
constexpr double SHIP_COLLISION_RADIUS_PIXELS = 10.0;
constexpr double PROJECTILE_COLLISION_RADIUS_PIXELS = 1.0;
constexpr double EARTH_COLLISION_RADIUS_PIXELS = 50.0;

/*************************************************************
 * Satellite Part Collision Radii (in pixels)
 *************************************************************/
constexpr double HUBBLE_TELESCOPE_RADIUS_PIXELS = 10.0;
constexpr double HUBBLE_COMPUTER_RADIUS_PIXELS = 7.0;
constexpr double HUBBLE_LEFT_RADIUS_PIXELS = 8.0;
constexpr double HUBBLE_RIGHT_RADIUS_PIXELS = 8.0;

constexpr double STARLINK_BODY_RADIUS_PIXELS = 2.0;
constexpr double STARLINK_ARRAY_RADIUS_PIXELS = 4.0;

constexpr double CREW_DRAGON_CENTER_RADIUS_PIXELS = 6.0;
constexpr double CREW_DRAGON_LEFT_RADIUS_PIXELS = 6.0;
constexpr double CREW_DRAGON_RIGHT_RADIUS_PIXELS = 6.0;

constexpr double GPS_CENTER_RADIUS_PIXELS = 7.0;
constexpr double GPS_LEFT_RADIUS_PIXELS = 8.0;
constexpr double GPS_RIGHT_RADIUS_PIXELS = 8.0;

/*************************************************************
 * Fragment Counts on Satellite Breakup
 *************************************************************/
constexpr int HUBBLE_TELESCOPE_FRAGMENTS = 3;
constexpr int HUBBLE_COMPUTER_FRAGMENTS = 2;
constexpr int HUBBLE_LEFT_FRAGMENTS = 2;
constexpr int HUBBLE_RIGHT_FRAGMENTS = 2;

constexpr int STARLINK_BODY_FRAGMENTS = 3;
constexpr int STARLINK_ARRAY_FRAGMENTS = 3;
constexpr int STARLINK_EXTRA_FRAGMENTS = 2;

constexpr int CREW_DRAGON_CENTER_FRAGMENTS = 4;
constexpr int CREW_DRAGON_LEFT_FRAGMENTS = 2;
constexpr int CREW_DRAGON_RIGHT_FRAGMENTS = 2;
constexpr int CREW_DRAGON_EXTRA_FRAGMENTS = 2;

constexpr int GPS_CENTER_FRAGMENTS = 3;
constexpr int GPS_LEFT_FRAGMENTS = 3;
constexpr int GPS_RIGHT_FRAGMENTS = 3;
constexpr int GPS_EXTRA_FRAGMENTS = 2;

constexpr int SPUTNIK_FRAGMENTS = 4;

/*************************************************************
 * Collision Immunity
 *************************************************************/
constexpr int COLLISION_IMMUNITY_FRAMES = 3;

/*************************************************************
 * Satellite Rotation and Spin
 *************************************************************/
// Offset to align satellite with Earth-facing direction
constexpr double SATELLITE_COUNTERCLOCKWISE_OFFSET_RADIANS = -M_PI_2;

// Defunct satellite spin rate range (radians/second)
constexpr double MIN_SATELLITE_SPIN_RATE = 0.001;
constexpr double MAX_SATELLITE_SPIN_RATE = 0.010;

/*************************************************************
 * Debris Constants
 *************************************************************/
// Velocity "kick" applied to debris when created (meters/second)
// Spec says 5000-9000, but reduced for better visual appearance
constexpr double KICK_MIN_METERS_PER_SECOND = 5000;
constexpr double KICK_MAX_METERS_PER_SECOND = 9000;

// Fragment lifetime range (in frames)
constexpr double FRAGMENT_MIN_LIFETIME_FRAMES = 50.0;
constexpr double FRAGMENT_MAX_LIFETIME_FRAMES = 100.0;

// Distance to spawn debris from parent (in pixels)
constexpr double SPAWN_OFFSET_PIXELS = 4.0;

// Fragment spin rate range (radians/second)
constexpr double FRAGMENT_MIN_SPIN_RADIANS_PER_SECOND = 0.4;
constexpr double FRAGMENT_MAX_SPIN_RADIANS_PER_SECOND = 1.2;

/*************************************************************
 * Ship Constants
 *************************************************************/
// How much the ship turns per key press (radians)
constexpr double SHIP_TURN_ANGLE_RADIANS = 0.1;

// Ship thrust acceleration (meters/second²)
constexpr double SHIP_THRUST_ACCELERATION_METERS_PER_SECOND_SQUARED = 2.0;

/*************************************************************
 * Projectile Constants
 *************************************************************/
// Additional speed added to projectile relative to ship (meters/second)
constexpr double PROJECTILE_SPEED_BOOST_METERS_PER_SECOND = 9000.0;

// Distance to spawn projectile from ship (in pixels)
constexpr double PROJECTILE_SPAWN_OFFSET_PIXELS = 19.0;

// How many game update cycles a projectile lasts
constexpr int PROJECTILE_LIFETIME_GAME_UNITS = 70;
