/***********************************************************************
 * Source File:
 *    Point : The representation of a position on the screen
 * Author:
 *    Lindsey Goode and Porter Williams
 * Summary:
 *    Everything we need to know about a location on the screen.
 ************************************************************************/

#include "satellite.h"
#include "constants.h"
#include <cmath>
/******************************************
 * Satellite : Random Spin Rate
 * Initialize the angular velocity to a random spin rate.
 *****************************************/
void Satellite::setRandomSpinRate()
{
   // Random spin speed in radians/second.
   constexpr double MIN_SPIN_RATE = 0.001;
   constexpr double MAX_SPIN_RATE = 0.010;

   const double percent = static_cast<double>(std::rand()) / static_cast<double>(RAND_MAX);
   const double spinRate = MIN_SPIN_RATE + percent * (MAX_SPIN_RATE - MIN_SPIN_RATE);
   const double direction = (std::rand() % 2 == 0) ? 1.0 : -1.0;
   setAngularVelocity(direction * spinRate);
}

/******************************************
 * Satellite : Initialize Circular Orbit
 * Set this satellite's initial position and velocity for a circular orbit.
 *****************************************/
void Satellite::initializeCircularOrbit(double orbitalRadiusMeters, double phaseRadians)
{
   if (orbitalRadiusMeters <= 0.0)
      return;

   const double speed = std::sqrt(MU / orbitalRadiusMeters);
   const double x = orbitalRadiusMeters * std::cos(phaseRadians);
   const double y = orbitalRadiusMeters * std::sin(phaseRadians);
   const double vx = -speed * std::sin(phaseRadians);
   const double vy = speed * std::cos(phaseRadians);

   Position position;
   position.setMeters(x, y);
   setPosition(position);
   setVelocity(Velocity(vx, vy));
}


/******************************************
 * Satellite : Update
 * Update the satellite's position
 *****************************************/

void Satellite::update(double dt)
{
   Entity::update(dt);
}
