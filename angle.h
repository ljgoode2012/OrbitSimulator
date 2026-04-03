/***********************************************************************
 * Header File:
 *    Angle : The representation of an angle
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Everything we need to know about angles and direction
 ************************************************************************/

#pragma once

#include "constants.h"
#include <cmath>
class TestVelocity;
class TestAcceleration;
class TestAngle;
class TestShip;
class TestEntity;
class TestSatellite;

/*********************************************
 * Angle
 * An angle represents a direction in radians,
 * normalized between 0 and 2π
 *********************************************/
class Angle
{
public:
   friend class TestVelocity;
   friend class TestAcceleration;
   friend class TestAngle;
   friend class TestShip;
   friend class TestEntity;
   friend class TestSatellite;

   Angle(double radians = 0.0) : radians(0.0) { setRadians(radians); }
   Angle(const Angle& rhs) : radians(rhs.radians) {}

   double getDegrees() const { return radians * 180.0 / M_PI; }

   double getRadians() const { return radians; }

   double getDx() const { return std::sin(radians); }

   double getDy() const { return std::cos(radians); }

   bool isRight() const { return radians < M_PI; }

   bool isLeft() const { return radians > M_PI; }

   void setRadians(double radians) { this->radians = normalize(radians); }

   void setDegrees(double degrees) { setRadians(degrees * M_PI / 180.0); }

   void setNaturalRadians(double radians) { this->radians = radians; }

   void setUp() { radians = 0.0; }

   void setDown() { radians = M_PI; }

   void setRight() { radians = M_PI_2; }

   void setLeft() { radians = M_PI + M_PI_2; }

   void reverse() { setRadians(radians + M_PI); }

   void setDxDy(double dx, double dy) { setRadians(std::atan2(dx, dy)); }

   Angle& add(double deltaRadians)
   {
      setRadians(radians + deltaRadians);
      return *this;
   }

   void addRadians(double deltaRadians) { setRadians(radians + deltaRadians); }

   Angle& operator=(const Angle& a);

   operator double() const { return radians; }

private:
   static double normalize(double radians);

   double radians;
};
