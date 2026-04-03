/***********************************************************************
 * Source File:
 *    Acceleration : The representation of acceleration in 2D space
 * Author:
 *    Lindsey Goode and Porter Williams
 * Summary:
 *    Everything we need for a change in speed
 ************************************************************************/

#include "acceleration.h"
#include "angle.h"
#include <cmath>

/******************************************
 * Acceleration : Set
 * Set acceleration from angle and magnitude
 *****************************************/
void Acceleration::set(const Angle& a, double magnitude)
{
   ddx = magnitude * sin(a.getRadians());
   ddy = magnitude * cos(a.getRadians());
}

/******************************************
 * Acceleration : Add
 * Add two acceleration objects together
 *****************************************/
void Acceleration::add(const Acceleration& rhs)
{
   ddx += rhs.getDDX();
   ddy += rhs.getDDY();
}

/******************************************
 * Acceleration : Assignment Operator
 * Assign one acceleration to another
 *****************************************/
Acceleration& Acceleration::operator=(const Acceleration& a)
{
   if (this != &a)
   {
      ddx = a.ddx;
      ddy = a.ddy;
   }
   return *this;
}
