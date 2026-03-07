/***********************************************************************
 * Header File:
 *    Angle: The representation of an angle
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Everything we need to know about an angle, which is just a single double
 ************************************************************************/

#pragma once

#include "constants.h"
#include <cmath>
#include <iostream>

class TestVelocity;
class TestAcceleration;
class TestAngle;

/*********************************************
 * Angle
 * A single angle based on radians
 *********************************************/

class Angle
{
public:
   friend TestVelocity;
   friend TestAcceleration;
   friend TestAngle;

   // Constructors
   Angle() : radians(0.0) {}
   Angle(const Angle& rhs) : radians(rhs.radians) {}
   Angle(double degrees) : radians(0.0) { setDegrees(degrees); }

   // Getters
   double getDegrees() const { return radians * 180.0 / M_PI; }
   double getRadians() const { return radians; }

   double getDx() const { return sin(radians); }
   double getDy() const { return cos(radians); }

   bool isRight() const { return radians < M_PI; }
   bool isLeft() const { return radians > M_PI; }

   // Setters
   void setDegrees(double degrees) { radians = normalize(convertToRadians(degrees)); }
   void setRadians(double radians) { this->radians = normalize(radians); }
   void setNaturalRadians(double radians);

   void setUp() { radians = 0.0; }
   void setDown() { radians = M_PI; }
   void setRight() { radians = M_PI * 0.5; }
   void setLeft() { radians = M_PI * 1.5; }

   void reverse() { radians = normalize(radians + M_PI); }

   void setDxDy(double dx, double dy) { radians = normalize(atan2(dx, dy)); }

   Angle& add(double delta) { radians = normalize(radians + delta); return *this; }

   // Operators
   Angle& operator=(const Angle& a)
   {
      if (this != &a)
      {
         radians = a.radians;
      }
      return *this;
   }
   Angle operator+(const Angle& a) const { return Angle(radians + a.radians); }
   Angle& operator+=(const Angle& a) { radians += a.radians; return *this; }
   Angle operator-(const Angle& a) const { return Angle(radians - a.radians); }
   Angle& operator-=(const Angle& a) { radians -= a.radians; return *this; }
   bool operator==(const Angle& a) const { return radians == a.radians; }
   bool operator!=(const Angle& a) const { return !(*this == a); }

private:
   double radians;

   double normalize(double radians) const;

   double convertToRadians(double degrees) const
   {
      return degrees / 360.0 * (M_PI * 2.0);
   }

   double convertToDegrees(double radians) const
   {
      return radians / (M_PI * 2.0) * 360.0;
   }
};

inline std::ostream& operator<<(std::ostream& out, const Angle& rhs)
{
   out << rhs.getDegrees() << " degrees";
   return out;
}
