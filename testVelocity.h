/***********************************************************************
 * Header File:
 *    Test Velocity : Test the Velocity class
 * Author:
 *    Lindsey Goode and Porter Williams
 * Summary:
 *    All the unit tests for Velocity
 ************************************************************************/

#pragma once

#include "acceleration.h"
#include "angle.h"
#include "constants.h"
#include "unitTest.h"
#include "velocity.h"

/*********************************************
 * TEST VELOCITY
 * Unit tests for Velocity
 *********************************************/
class TestVelocity : public UnitTest
{
public:
   void run()
   {
      // Constructors
      constructor_default();
      constructor_nonDefault();
      constructor_copy();

      // Getters
      getDX();
      getDY();
      getSpeed_zero();
      getSpeed_value();
      getAngle();

      // Setters
      setDX();
      setDY();
      set_up();
      set_down();
      set_right();
      set_left();
      set_diagonal();

      // Add methods
      addDX_zero();
      addDX_value();
      addDY_zero();
      addDY_value();
      add_velocity_zeroZero();
      add_velocity_valueZero();
      add_velocity_zeroValue();
      add_velocity_valueValue();
      add_acceleration_zero();
      add_acceleration_value();

      // Other methods
      reverse();

      report("Velocity");
   }

private:
   /*****************************************************************
    *****************************************************************
    * CONSTRUCTOR
    *****************************************************************
    *****************************************************************/

   /*********************************************
    * name:    DEFAULT CONSTRUCTOR
    * input:   nothing
    * output:  dx=0.0, dy=0.0
    *********************************************/
   void constructor_default()
   { // setup
      // exercise
      Velocity v;

      // verify
      assertEquals(v.getDX(), 0.0);
      assertEquals(v.getDY(), 0.0);
   } // teardown

   /*********************************************
    * name:    NON-DEFAULT CONSTRUCTOR
    * input:   2.3, 4.5
    * output:  dx=2.3, dy=4.5
    *********************************************/
   void constructor_nonDefault()
   { // setup
      double dx = 2.3;
      double dy = 4.5;
      // exercise
      Velocity v(dx, dy);

      // verify
      assertEquals(v.getDX(), 2.3);
      assertEquals(v.getDY(), 4.5);
      assertEquals(dx, 2.3);
      assertEquals(dy, 4.5);
   } // teardown

   /*********************************************
    * name:    COPY CONSTRUCTOR
    * input:   v(2.3, 4.5)
    * output:  v2(2.3, 4.5)
    *********************************************/
   void constructor_copy()
   { // setup
      Velocity v(2.3, 4.5);
      // exercise
      Velocity v2(v);

      // verify
      assertEquals(v2.getDX(), 2.3);
      assertEquals(v2.getDY(), 4.5);
      assertEquals(v.getDX(), 2.3);
      assertEquals(v.getDY(), 4.5);
   } // teardown

   /*****************************************************************
    *****************************************************************
    * GETTERS
    *****************************************************************
    *****************************************************************/

   /*********************************************
    * name:    GET DX
    * input:   (2.3, 4.5)
    * output:  2.3
    *********************************************/
   void getDX()
   { // setup
      Velocity v(2.3, 4.5);
      double dx = 99.9;

      // exercise
      dx = v.getDX();

      // verify
      assertEquals(v.getDX(), 2.3);
      assertEquals(v.getDY(), 4.5);
      assertEquals(dx, 2.3);
   } // teardown

   /*********************************************
    * name:    GET DY
    * input:   (2.3, 4.5)
    * output:  4.5
    *********************************************/
   void getDY()
   { // setup
      Velocity v(2.3, 4.5);
      double dy = 99.9;

      // exercise
      dy = v.getDY();

      // verify
      assertEquals(v.getDX(), 2.3);
      assertEquals(v.getDY(), 4.5);
      assertEquals(dy, 4.5);
   } // teardown

   /*********************************************
    * name:    GET SPEED ZERO
    * input:   (0.0, 0.0)
    * output:  0.0
    *********************************************/
   void getSpeed_zero()
   { // setup
      Velocity v(0.0, 0.0);

      // exercise
      double speed = v.getSpeed();

      // verify
      assertEquals(speed, 0.0);
   } // teardown

   /*********************************************
    * name:    GET SPEED VALUE
    * input:   (3.0, 4.0)
    * output:  5.0
    *********************************************/
   void getSpeed_value()
   { // setup
      Velocity v(3.0, 4.0);

      // exercise
      double speed = v.getSpeed();

      // verify
      assertEquals(speed, 5.0);
   } // teardown

   /*********************************************
    * name:    GET ANGLE
    * input:   (1.0, 0.0)
    * output:  angle with proper direction
    *********************************************/
   void getAngle()
   { // setup
      Velocity v(1.0, 0.0);

      // exercise
      Angle angle = v.getAngle();

      // verify
      assertEquals(angle.getDx(), 1.0);
      assertEquals(angle.getDy(), 0.0);
   } // teardown

   /*****************************************************************
    *****************************************************************
    * SETTERS
    *****************************************************************
    *****************************************************************/

   /*********************************************
    * name:    SET DX
    * input:   v=(−99.9, −88.8) setDX(11.1)
    * output:  v=(11.1, −88.8)
    *********************************************/
   void setDX()
   { // setup
      Velocity v(-99.9, -88.8);

      // exercise
      v.setDX(11.1);

      // verify
      assertEquals(v.getDX(), 11.1);
      assertEquals(v.getDY(), -88.8);
   } // teardown

   /*********************************************
    * name:    SET DY
    * input:   v=(−99.9, −88.8) setDY(11.1)
    * output:  v=(−99.9, 11.1)
    *********************************************/
   void setDY()
   { // setup
      Velocity v(-99.9, -88.8);

      // exercise
      v.setDY(11.1);

      // verify
      assertEquals(v.getDX(), -99.9);
      assertEquals(v.getDY(), 11.1);
   } // teardown

   /*********************************************
    * name:    SET UP
    * input:   v=(−99.9, −88.8) angle=0 rad, magnitude=3.3
    * output:  v=(0.0, 3.3)
    *********************************************/
   void set_up()
   { // setup
      Velocity v(-99.9, -88.8);
      Angle angle;
      angle.setRadians(0.0); // up
      double magnitude = 3.3;

      // exercise
      v.set(angle, magnitude);

      // verify
      assertEquals(v.getDX(), 0.0); // 0.0 = 3.3 sin(0)
      assertEquals(v.getDY(), 3.3); // 3.3 = 3.3 cos(0)
   } // teardown

   /*********************************************
    * name:    SET DOWN
    * input:   v=(−99.9, −88.8) angle=π rad, magnitude=3.3
    * output:  v=(0.0, −3.3)
    *********************************************/
   void set_down()
   { // setup
      Velocity v(-99.9, -88.8);
      Angle angle;
      angle.setRadians(M_PI); // down
      double magnitude = 3.3;

      // exercise
      v.set(angle, magnitude);

      // verify
      assertEquals(v.getDX(), 0.0);  // 0.0 = 3.3 sin(π)
      assertEquals(v.getDY(), -3.3); // -3.3 = 3.3 cos(π)
   } // teardown

   /*********************************************
    * name:    SET RIGHT
    * input:   v=(−99.9, −88.8) angle=π/2 rad, magnitude=3.3
    * output:  v=(3.3, 0.0)
    *********************************************/
   void set_right()
   { // setup
      Velocity v(-99.9, -88.8);
      Angle angle;
      angle.setRadians(M_PI_2); // right
      double magnitude = 3.3;

      // exercise
      v.set(angle, magnitude);

      // verify
      assertEquals(v.getDX(), 3.3); // 3.3 = 3.3 sin(π/2)
      assertEquals(v.getDY(), 0.0); // 0.0 = 3.3 cos(π/2)
   } // teardown

   /*********************************************
    * name:    SET LEFT
    * input:   v=(−99.9, −88.8) angle=3π/2 rad, magnitude=3.3
    * output:  v=(−3.3, 0.0)
    *********************************************/
   void set_left()
   { // setup
      Velocity v(-99.9, -88.8);
      Angle angle;
      angle.setRadians(M_PI_2 + M_PI); // left
      double magnitude = 3.3;

      // exercise
      v.set(angle, magnitude);

      // verify
      assertEquals(v.getDX(), -3.3); // -3.3 = 3.3 sin(3π/2)
      assertEquals(v.getDY(), 0.0);  // 0.0 = 3.3 cos(3π/2)
   } // teardown

   /*********************************************
    * name:    SET DIAGONAL
    * input:   v=(−99.9, −88.8) angle=1.0 rad, magnitude=1.0
    * output:  v=(0.8414, 0.5403)
    *********************************************/
   void set_diagonal()
   { // setup
      Velocity v(-99.9, -88.8);
      Angle angle;
      angle.setRadians(1.0);
      double magnitude = 1.0;

      // exercise
      v.set(angle, magnitude);

      // verify
      assertEquals(v.getDX(), 0.8414); // 0.8414 = 1.0 sin(1.0)
      assertEquals(v.getDY(), 0.5403); // 0.5403 = 1.0 cos(1.0)
   } // teardown

   /*****************************************************************
    *****************************************************************
    * ADD METHODS
    *****************************************************************
    *****************************************************************/

   /*********************************************
    * name:    ADD DX ZERO
    * input:   v=(2.3, 4.5) addDX(0.0)
    * output:  v=(2.3, 4.5)
    *********************************************/
   void addDX_zero()
   { // setup
      Velocity v(2.3, 4.5);

      // exercise
      v.addDX(0.0);

      // verify
      assertEquals(v.getDX(), 2.3);
      assertEquals(v.getDY(), 4.5);
   } // teardown

   /*********************************************
    * name:    ADD DX VALUE
    * input:   v=(2.3, 4.5) addDX(4.1)
    * output:  v=(6.4, 4.5)
    *********************************************/
   void addDX_value()
   { // setup
      Velocity v(2.3, 4.5);

      // exercise
      v.addDX(4.1);

      // verify
      assertEquals(v.getDX(), 6.4);
      assertEquals(v.getDY(), 4.5);
   } // teardown

   /*********************************************
    * name:    ADD DY ZERO
    * input:   v=(2.3, 4.5) addDY(0.0)
    * output:  v=(2.3, 4.5)
    *********************************************/
   void addDY_zero()
   { // setup
      Velocity v(2.3, 4.5);

      // exercise
      v.addDY(0.0);

      // verify
      assertEquals(v.getDX(), 2.3);
      assertEquals(v.getDY(), 4.5);
   } // teardown

   /*********************************************
    * name:    ADD DY VALUE
    * input:   v=(2.3, 4.5) addDY(4.1)
    * output:  v=(2.3, 8.6)
    *********************************************/
   void addDY_value()
   { // setup
      Velocity v(2.3, 4.5);

      // exercise
      v.addDY(4.1);

      // verify
      assertEquals(v.getDX(), 2.3);
      assertEquals(v.getDY(), 8.6);
   } // teardown

   /*********************************************
    * name:    ADD VELOCITY ZERO TO ZERO
    * input:   vLHS=(0.0, 0.0) vRHS=(0.0, 0.0)
    * output:  vLHS=(0.0, 0.0)
    *********************************************/
   void add_velocity_zeroZero()
   { // setup
      Velocity vLHS(0.0, 0.0);
      Velocity vRHS(0.0, 0.0);

      // exercise
      vLHS.add(vRHS);

      // verify
      assertEquals(vLHS.getDX(), 0.0);
      assertEquals(vLHS.getDY(), 0.0);
   } // teardown

   /*********************************************
    * name:    ADD VELOCITY ZERO TO VALUE
    * input:   vLHS=(1.1, 2.2) vRHS=(0.0, 0.0)
    * output:  vLHS=(1.1, 2.2)
    *********************************************/
   void add_velocity_valueZero()
   { // setup
      Velocity vLHS(1.1, 2.2);
      Velocity vRHS(0.0, 0.0);

      // exercise
      vLHS.add(vRHS);

      // verify
      assertEquals(vLHS.getDX(), 1.1);
      assertEquals(vLHS.getDY(), 2.2);
   } // teardown

   /*********************************************
    * name:    ADD VELOCITY VALUE TO ZERO
    * input:   vLHS=(0.0, 0.0) vRHS=(4.4, 7.7)
    * output:  vLHS=(4.4, 7.7)
    *********************************************/
   void add_velocity_zeroValue()
   { // setup
      Velocity vLHS(0.0, 0.0);
      Velocity vRHS(4.4, 7.7);

      // exercise
      vLHS.add(vRHS);

      // verify
      assertEquals(vLHS.getDX(), 4.4);
      assertEquals(vLHS.getDY(), 7.7);
   } // teardown

   /*********************************************
    * name:    ADD VELOCITY VALUE TO VALUE
    * input:   vLHS=(1.1, 2.2) vRHS=(4.4, 7.7)
    * output:  vLHS=(5.5, 9.9)
    *********************************************/
   void add_velocity_valueValue()
   { // setup
      Velocity vLHS(1.1, 2.2);
      Velocity vRHS(4.4, 7.7);

      // exercise
      vLHS.add(vRHS);

      // verify
      assertEquals(vLHS.getDX(), 5.5); // 1.1 + 4.4 = 5.5
      assertEquals(vLHS.getDY(), 9.9); // 2.2 + 7.7 = 9.9
   } // teardown

   /*********************************************
    * name:    ADD ACCELERATION ZERO
    * input:   v=(2.3, 4.5) a=(0.0, 0.0) time=1.0
    * output:  v=(2.3, 4.5)
    *********************************************/
   void add_acceleration_zero()
   { // setup
      Velocity v(2.3, 4.5);
      Acceleration a(0.0, 0.0);
      double time = 1.0;

      // exercise
      v.add(a, time);

      // verify
      assertEquals(v.getDX(), 2.3);
      assertEquals(v.getDY(), 4.5);
   } // teardown

   /*********************************************
    * name:    ADD ACCELERATION VALUE
    * input:   v=(2.3, 4.5) a=(1.1, 2.2) time=2.0
    * output:  v=(4.5, 8.9)
    *********************************************/
   void add_acceleration_value()
   { // setup
      Velocity v(2.3, 4.5);
      Acceleration a(1.1, 2.2);
      double time = 2.0;

      // exercise
      v.add(a, time);

      // verify
      assertEquals(v.getDX(), 4.5); // 2.3 + 1.1*2.0 = 4.5
      assertEquals(v.getDY(), 8.9); // 4.5 + 2.2*2.0 = 8.9
   } // teardown

   /*****************************************************************
    *****************************************************************
    * OTHER METHODS
    *****************************************************************
    *****************************************************************/

   /*********************************************
    * name:    REVERSE
    * input:   v=(2.3, 4.5)
    * output:  v=(−2.3, −4.5)
    *********************************************/
   void reverse()
   { // setup
      Velocity v(2.3, 4.5);

      // exercise
      v.reverse();

      // verify
      assertEquals(v.getDX(), -2.3);
      assertEquals(v.getDY(), -4.5);
   } // teardown
};
