#pragma once

class Acceleration
{
public:
   // Members
   double ddx;
   double ddy;

   // Constructors
   Acceleration() : ddx(0.0), ddy(0.0) {}
   Acceleration(double ddx, double ddy) : ddx(ddx), ddy(ddy) {}
   Acceleration(const Acceleration& a) : ddx(a.ddx), ddy(a.ddy) {}

   // operators
   Acceleration& operator=(const Acceleration& a)
   {
      if (this != &a)
      {
         ddx = a.ddx;
         ddy = a.ddy;
      }
      return *this;
   }
   Acceleration operator+(const Acceleration& a) const
   {
      return Acceleration(ddx + a.ddx, ddy + a.ddy);
   }
   Acceleration& operator+=(const Acceleration& a)
   {
      ddx += a.ddx;
      ddy += a.ddy;
      return *this;
   }
   Acceleration operator-(const Acceleration& a) const
   {
      return Acceleration(ddx - a.ddx, ddy - a.ddy);
   }
   Acceleration& operator-=(const Acceleration& a)
   {
      ddx -= a.ddx;
      ddy -= a.ddy;
      return *this;
   }
   bool operator==(const Acceleration& a) const
   {
      return ddx == a.ddx && ddy == a.ddy;
   }
   bool operator!=(const Acceleration& a) const
   {
      return !(*this == a);
   }
};
