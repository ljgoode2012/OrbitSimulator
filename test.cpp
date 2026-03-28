/***********************************************************************
 * Source File:
 *    Test : Test runner
 * Author:
 *    Br. Helfrich
 * Summary:
 *    The test runner for all the unit tests
 ************************************************************************/

#include "test.h"
#include "testAcceleration.h"
#include "testAngle.h"
#include "testEntity.h"
#include "testPosition.h"
#include "testSatellite.h"
#include "testShip.h"
#include "testVelocity.h"
#include "testFragment.h"
#include "testProjectile.h"

#ifdef _WIN32
#include <iostream>
#include <windows.h>
using namespace std;
#endif

/*****************************************************************
 * TEST RUNNER
 * Runs all the unit tests
 ****************************************************************/
void testRunner()
{
#ifdef _WIN32
   AllocConsole();
   FILE* stream;
   errno_t err;
   err = freopen_s(&stream, "CONOUT$", "a", stdout);
#endif // _WIN32

   TestPosition().run();
   TestVelocity().run();
   TestAcceleration().run();
   TestAngle().run();
   TestEntity().run();
   TestBreakableEntity().run();
   TestSatellite().run();
   TestShip().run();
   TestTimedEntity().run();
   TestFragment().run();
   TestProjectile().run();
}
