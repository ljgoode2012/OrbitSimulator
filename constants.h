#pragma once

/*************************************************************
 * Mathematical Constants
 *************************************************************/
constexpr double M_PI = 3.14159265358979323846;
constexpr double M_PI_2 = M_PI / 2.0;

/*************************************************************
 * Physics Constants
 *************************************************************/
// Earth's gravitational parameter (G * M) in m^3/s^2
constexpr double MU = 3.986004418e14;

// Geostationary orbit radius in meters (~42,164 km)
constexpr double GEO_RADIUS_METERS = 42164000.0;

/*************************************************************
 * Simulation Constants
 *************************************************************/
// Frames per second in the simulation
constexpr double FRAMES_PER_SECOND = 30.0;

// Frames per 1 simulated day (1 simulated day = 60 seconds of run time)
constexpr double FRAMES_PER_SIM_DAY = FRAMES_PER_SECOND * 60.0; //1800.0

// Dilated seconds per frame (86400 seconds in a day)
constexpr double SIM_SECONDS_PER_FRAME = 86400.0 / FRAMES_PER_SIM_DAY; // 48.0

// Number of stars to generate
constexpr int NUM_STARS = 200;
