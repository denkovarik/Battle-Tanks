#include "functions.h"

using namespace std;

/*
   inPos()
Authors: Dennis Kovarik
Parameters
    int x - int representing x pos of tank
    int y - int representing y pos of tank
    int enemyX - int representing x pos of enemy tank
    int enemyY - int representing y pos of enemy tank
Return Value
    returns true if tank is in line with enemy tank
    returns false if tank not in line with enemy tank or tank on same pos as
	    enemy tank.

Given the x and y values of both this tank and an enemy tank, this function will
determine if this tank is in line with another enemy tank. If this tank is in 
line with an enemy tank, it will return true. If not in line or if both tanks 
occupy the same position, then this function will return false.
*/
bool inPos(int x, int y, int enemyX, int enemyY)
{
    //On same pos
    if(x == enemyX && y == enemyY)
    {
	return false;
    }

    //In line horizontally
    if(abs(x - enemyX) > 0 && (y - enemyY) == 0)
    {
	return true;
    }

    //In line vertically
    if((x - enemyX) == 0 && abs(y - enemyY) > 0)
    {
	return true;
    }

    //In line diagonally
    if(abs(x - enemyX) == abs(y - enemyY))
    {
	return true;
    }

    return false;
}
