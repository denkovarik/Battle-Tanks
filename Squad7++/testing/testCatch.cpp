#define CATCH_CONFIG_MAIN
#ifndef __UTILTESTING
#define __UTILTESTING
#include "testCatch.h"
#endif

using namespace std;




/******************************************************************************
*  Testing if inPos function returns correct values
*    (This is the test case purpse)
*
*  Author(s): Dennis Kovarik
*  Parameters:  (These are parameters the test uses in all sections)
*    	x - int representing x pos of tank
*	y - int representing y pos of tank
*      	enemyX - int representing x pos of enemy tank
*	enemyY - int representing y pos of enemy tank
*  Return Value:
*    Returns the number of failed section tests.
*
*  This test case test if the 'inPos()' function accurately returns if a tank is
*  both out of range but in line with another tank.
*                                                   
********************************************************************************/
