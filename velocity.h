#pragma once

class Angle;
class Acceleration;
class TestVelocity;

class Velocity
{
public:
   friend class TestVelocity;

   // Constructors
   Velocity() : dx(0.0), dy(0.0) {}
   Velocity(double dx, double dy) : dx(dx), dy(dy) {}
   Velocity(const Velocity& v) : dx(v.dx), dy(v.dy) {}

   // Getters
   double getDX() const { return dx; }
   double getDY() const { return dy; }
   double getSpeed() const;
   Angle getAngle() const;

   // Setters
   void set(const Angle& angle, double magnitude);
   void setDX(double dx) { this->dx = dx; }
   void setDY(double dy) { this->dy = dy; }
   void addDX(double dx) { setDX(getDX() + dx); }
   void addDY(double dy) { setDY(getDY() + dy); }
   void add(const Acceleration& acceleration, double time);
   void add(const Velocity& rhs) { dx += rhs.getDX(); dy += rhs.getDY(); }
   void reverse();

   // Operators
   Velocity& operator=(const Velocity& v) { if (this != &v) { dx = v.dx; dy = v.dy; } return *this; }
   Velocity operator+(const Velocity& v) const { return Velocity(dx + v.dx, dy + v.dy); }
   Velocity& operator+=(const Velocity& v) { dx += v.dx; dy += v.dy; return *this; }
   Velocity operator-(const Velocity& v) const { return Velocity(dx - v.dx, dy - v.dy); }
   Velocity& operator-=(const Velocity& v) { dx -= v.dx; dy -= v.dy; return *this; }
   bool operator==(const Velocity& v) const { return dx == v.dx && dy == v.dy; }
   bool operator!=(const Velocity& v) const { return !(*this == v); }

private:
   double dx;
   double dy;
};
