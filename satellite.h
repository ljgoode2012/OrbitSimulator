#pragma once

#include "position.h"
#include "velocity.h"
#include "acceleration.h"

class Satellite
{
public:
   // Members
   Position position;
   Velocity velocity;
   Acceleration acceleration;

   // Constructors
   Satellite()
      : position(), velocity(), acceleration() {}

   Satellite(const Position& pos, const Velocity& vel, const Acceleration& acc = Acceleration())
      : position(pos), velocity(vel), acceleration(acc) {}

   // Update the satellite's position and velocity using basic kinematics
   void update(double dt);
};
