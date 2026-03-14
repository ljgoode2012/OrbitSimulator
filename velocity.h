#pragma once
#include "acceleration.h"
#include "angle.h"
#include <cmath>

class TestVelocity;

class Velocity
{
public:
   friend class TestVelocity;
   // Members
   double dx;
   double dy;

   // Constructors
   Velocity() : dx(0.0), dy(0.0) {}
   Velocity(double dx, double dy) : dx(dx), dy(dy) {}
   Velocity(const Velocity& v) : dx(v.dx), dy(v.dy) {}

   // Getters
   double getDX() const { return dx; }
   double getDY() const { return dy; }
   double getSpeed() const { return std::sqrt(dx * dx + dy * dy); }
   Angle getAngle() const
   {
      Angle angle;
      angle.setDxDy(dx, dy);
      return angle;
   }

   // Setters
   void set(const Angle& angle, double magnitude)
   {
      dx = angle.getDx() * magnitude;
      dy = angle.getDy() * magnitude;
   }
   void setDX(double dx) { this->dx = dx; }
   void setDY(double dy) { this->dy = dy; }
   void addDX(double dx) { this->dx += dx; }
   void addDY(double dy) { this->dy += dy; }
   void add(const Acceleration& acceleration, double dt)
   {
      dx += acceleration.getDDX() * dt;
      dy += acceleration.getDDY() * dt;
   }
   void add(const Velocity& rhs)
   {
      dx += rhs.dx;
      dy += rhs.dy;
   }
   void reverse()
   {
      dx = -dx;
      dy = -dy;
   }

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
   void update(const Acceleration& acceleration, double dt)
   {
      dx += acceleration.getDDX() * dt;
      dy += acceleration.getDDY() * dt;
   }
};
