/***********************************************************************
 * Header File:
 *    Entity: The representation of a physical object in space
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Each entity has a position, velocity, and heading angle, and can update itself based on
 * time and acceleration due to gravity. This is the base class for all physical
 * objects in the simulation.
 ************************************************************************/

#pragma once

#include "position.h"
#include "velocity.h"
#include "angle.h"

/*********************************************
 * Entity
 * A single physical object in space with a position, velocity, and heading
 * angle
 *********************************************/
class Entity
{
public:
   // Members
   Position position;
   Velocity velocity;
   Angle heading;
   double radiusInPixels;

   // Constructors
   Entity() : position(), velocity(), heading(), radiusInPixels(0.0) {}

   Entity(const Position& pos, const Velocity& vel)
       : position(pos), velocity(vel), heading(), radiusInPixels(0.0)
   {
   }

   Entity(const Position& pos, const Velocity& vel, const Angle& head)
       : position(pos), velocity(vel), heading(head), radiusInPixels(0.0)
   {
   }

   Entity(const Position& pos, const Velocity& vel, const Angle& head, double radius)
       : position(pos), velocity(vel), heading(head), radiusInPixels(radius)
   {
   }

   // Destructor
   virtual ~Entity() = default;

   // Update the entity's position and velocity using basic kinematics
   void update(double dt);
};

