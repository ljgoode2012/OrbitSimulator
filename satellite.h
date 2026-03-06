/***********************************************************************
 * Header File:
 *    Satellite: The representation of a satellite in space
 * Author:
 *    Lindsey Goode and Porter Williams
 * Summary:
 *    Everything we need to know about a satellite.
 ************************************************************************/

#pragma once

#include "entity.h"
#include "acceleration.h"

/*********************************************
 * Satellite
 * A single satellite object in space
 *********************************************/
class Satellite : public Entity
{
public:
   // Members
   Acceleration acceleration;

   // Constructors
   Satellite() : Entity(), acceleration() {}
   Satellite(const Position& pos, const Velocity& vel)
       : Entity(pos, vel), acceleration() {}
   Satellite(const Position& pos, const Velocity& vel, const Angle& head)
       : Entity(pos, vel, head), acceleration() {}
   Satellite(const Position& pos, const Velocity& vel, const Angle& head, double radius)
       : Entity(pos, vel, head, radius), acceleration() {}

   // Destructor
   virtual ~Satellite() = default;

   // Update the satellite's position and velocity using gravity
   void update(double dt); 
};
