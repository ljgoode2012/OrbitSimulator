/***********************************************************************
 * Header File:
 *    Acceleration: The representation of an acceleration (or deceleration)
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Everything we need to know about an acceleration (or deceleration)
 ************************************************************************/

#pragma once

#include "angle.h"

class TestAcceleration;

/*********************************************
 * Acceleration
 * An acceleration is just two doubles representing
 * the change in velocity in the x and y directions
 *********************************************/

class Acceleration
{
public:
   friend class TestAcceleration;

   // Constructors
   Acceleration() : ddx(0.0), ddy(0.0) {}
   Acceleration(double ddx, double ddy) : ddx(ddx), ddy(ddy) {}
   Acceleration(const Acceleration& a) : ddx(a.ddx), ddy(a.ddy) {}
   Acceleration(const Angle& a, double magnitude) { set(a, magnitude); }

   // Getters
   double getDDX() const { return ddx; }
   double getDDY() const { return ddy; }

   // Setters
   void setDDX(double ddx) { this->ddx = ddx; }
   void setDDY(double ddy) { this->ddy = ddy; }
   void set(const Angle& a, double magnitude);

   // Methods
   void addDDX(double ddx) { this->ddx += ddx; }
   void addDDY(double ddy) { this->ddy += ddy; }
   void add(const Acceleration& rhs);

   // Operators
   Acceleration& operator=(const Acceleration& a);

private:
   double ddx;
   double ddy;
};
