/***********************************************************************
 * Source File:
 *    Angle: The representation of an angle
 * Author:
 *    Lindsey Goode, Porter Williams
 * Summary:
 *    Implementation of the Angle class methods
 ************************************************************************/

#include "angle.h"
#include "constants.h"

/******************************************
 * Angle : normalize
 * Normalize radians to 0 to 2*PI
 *****************************************/
double Angle::normalize(double radians)
{
   radians = std::fmod(radians, M_PI * 2.0);
   if (radians < 0.0)
      radians += M_PI * 2.0;
   return radians;
}

/******************************************
 * Angle : assignment operator
 * Assign from another angle
 *****************************************/
Angle& Angle::operator=(const Angle& a)
{
   if (this != &a)
      radians = a.radians;
   return *this;
}
