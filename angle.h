#pragma once

#include <cmath>

class Angle
{
public:
   Angle(double radians = 0.0) : radians(0.0)
   {
      setRadians(radians);
   }

   void setRadians(double radians)
   {
      constexpr double TWO_PI = 6.28318530717958647692;
      this->radians = std::fmod(radians, TWO_PI);
      if (this->radians < 0.0)
         this->radians += TWO_PI;
   }

   double getRadians() const
   {
      return radians;
   }

   void addRadians(double deltaRadians)
   {
      setRadians(radians + deltaRadians);
   }

   operator double() const
   {
      return radians;
   }

private:
   double radians;
};
