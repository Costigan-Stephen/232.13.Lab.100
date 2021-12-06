/***********************************************************************
 * Header:
 *    Test
 * Summary:
 *    Driver to test hash.h
 * Author
 *    Br. Helfrich
 ************************************************************************/

#ifndef DEBUG
#define DEBUG   // Remove this to skip the unit tests
#endif // DEBUG

#include "testSpy.h"       // for the pair unit tests
#include "testPair.h"       // for the pair unit tests
#include "testHash.h"       // for the hash unit tests
#include "testList.h"       // for the list unit tests
int Spy::counters[] = {};

/**********************************************************************
 * MAIN
 * This is just a simple menu to launch a collection of tests
 ***********************************************************************/
int main()
{
#ifdef DEBUG
   // unit tests
   TestSpy().run();
   TestPair().run();
   TestList().run();
   TestHash().run();
#endif // DEBUG
   
   // driver
   return 0;
}
