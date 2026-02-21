#pragma once

class Velocity
{
public:
   // Members
   double dx;
   double dy;

   // Constructors
   Velocity() : dx(0.0), dy(0.0) {}
   Velocity(double dx, double dy) : dx(dx), dy(dy) {}
   Velocity(const Velocity& v) : dx(v.dx), dy(v.dy) {}

   // operators
   Velocity& operator=(const Velocity& v)
   {
      if (this != &v)
      {
         dx = v.dx;
         dy = v.dy;
      }
      return *this;
   }
   Velocity operator+(const Velocity& v) const
   {
      return Velocity(dx + v.dx, dy + v.dy);
   }
   Velocity& operator+=(const Velocity& v)
   {
      dx += v.dx;
      dy += v.dy;
      return *this;
   }
   Velocity operator-(const Velocity& v) const
   {
      return Velocity(dx - v.dx, dy - v.dy);
   }
   Velocity& operator-=(const Velocity& v)
   {
      dx -= v.dx;
      dy -= v.dy;
      return *this;
   }
   bool operator==(const Velocity& v) const
   {
      return dx == v.dx && dy == v.dy;
   }
   bool operator!=(const Velocity& v) const
   {
      return !(*this == v);
   }
};
