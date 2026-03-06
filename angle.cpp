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
double Angle::normalize(double radians) const
{
   while (radians < 0.0)
      radians += M_PI * 2.0;
   while (radians >= M_PI * 2.0)
      radians -= M_PI * 2.0;
   return radians;
}

/******************************************
 * Angle : setNaturalRadians
 * Set radians without normalization
 *****************************************/
void Angle::setNaturalRadians(double radians)
{
   this->radians = radians;
}
