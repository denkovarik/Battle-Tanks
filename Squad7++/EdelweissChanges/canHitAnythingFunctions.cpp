#include "Edelweiss.h"
using namespace std;



/*
   canHitAnything
Authors: Sam Backes
Parameters:
   int x               - The X position we are checking
   int y               - The Y position we are checking
Return Values:
   true if (as far as we know) a tank firing in a direction from position (x,y) would be able to hit something
   false if (as far as we know) a tank firing in ANY direction from position (x,y) would not be able to hit ANY enemy tank

This function is just a huge function call to the other canHit functions. Basically it's just to check if we could hit anything 
if we were to fire. You can also check positions that are not your own to determine if a spot is safe. Although, since RADAR is
limited, the results from such a request may or may not be 100% accurate.
*/
bool Edelweiss::canHitAnything(int x, int y)
{
	if(canHitUpLeft(x, y)
	|| canHitUp(x, y)
	|| canHitUpRight(x, y)
	|| canHitLeft(x, y)
	|| canHitRight(x, y)
	|| canHitDownLeft(x, y)
	|| canHitDown(x, y)
	|| canHitDownRight(x, y))
		return true;
	return false;
}

/*
   canHitUp
Authors: Sam Backes
Parameters:
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
*/
bool Edelweiss::canHitUp(int x, int y)
{
    //For every space that's in range and not off the map
    for(int i = 1; y - i >= 0; i++)
    {
	if(y - i > -1)
	{
            //If we run into a obstacle first, return false
	    if(blocked(x, y - i))
                return false;

            //If we find a tank first, return true
	    if(myMap[y - i][x] >= 1000 && !(x == myX && myY == y - i))
	    {
                return true;
	    }
        }
    }
    //Ran off the map or out of range before finding a tank or obstacle
    return false;	
}

/*
   canHitDown
Authors: Sam Backes
Parameters:
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
*/
bool Edelweiss::canHitDown(int x, int y)
{
    //For every space that's in range and not off the map
    for(int i = 1; y + i < height; i++)
    {
	if(y + i < height)
	{
	    //If we run into a obstacle first, return false
            if(blocked(x, y + i))
            	return false;

	    //If we find a tank first, return true
	    if((myMap[y + i][x] >= 1000) && !(x == myX && myY == y + i))
	    {
            	return true;
            }
	}
    }

    //Ran off the map or out of range before finding a tank or obstacle
    return false;	
}

/*
   canHitLeft
Authors: Sam Backes
Parameters:
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
*/
bool Edelweiss::canHitLeft(int x, int y)
{
    //For every space that's in range and not off the map
    for(int i = 1; x - i >= 0; i++)
    {
	if(x - i > -1)
	{
            //If we run into a obstacle first, return false
	    if(blocked(x - i, y))
                return false;

            //If we find a tank first, return true
	    if((myMap[y][x - i] >= 1000) && !(x - i == myX && myY == y))
	    {
        	return true;
	    }
        }
    }

    //Ran off the map or out of range before finding a tank or obstacle
    return false;
}

/*
   canHitRight
Authors: Sam Backes
Parameters:
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
*/
bool Edelweiss::canHitRight(int x, int y)
{
    //For every space that's in range and not off the map
    for(int i = 1; x + i < width; i++)
    {
	if(x + i < width)
	{
            //If we run into a obstacle first, return false
	    if(blocked(x + i, y))
 	    {
                return false;
	    }

            //If we find a tank first, return true
	    if((myMap[y][x + i] >= 1000) && !(x + i == myX && myY == y))
	    {
        	return true;
	    }
        }
    }

    //Ran off the map or out of range before finding a tank or obstacle
    return false;
}

/*
   canHitUpLeft
Authors: Sam Backes
Parameters:
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
*/
bool Edelweiss::canHitUpLeft(int x, int y)
{
    //For every space that's in range and not off the map
    for(int i = 1; y - i >= 0 && x - i >= 0; i++)
    {
	if(y - i > -1 && x - i > -1)
	{
            //If we run into a obstacle first, return false
	    if(blocked(x - i, y - i))
                return false;

            //If we find a tank first, return true
	    if((myMap[y - i][x - i] >= 1000) && !(x - i == myX && myY == y - i))
	    {
        	return true;
	    }
        }
    }

    //Ran off the map or out of range before finding a tank or obstacle
    return false;
}

/*
   canHitUpRight
Authors: Sam Backes
Parameters:
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
*/
bool Edelweiss::canHitUpRight(int x, int y)
{
    //For every space that's in range and not off the map
    for(int i = 1; y - i >= 0 && x + i < width; i++)
    {
	if(y - i > -1 && x + i < width)
	{
            //If we run into a obstacle first, return false
            if(blocked(x + i, y - i))
            	return false;

            //If we find a tank first, return true
	    if((myMap[y - i][x + i] >= 1000) && !(x + i == myX && myY == y - i))
	    {
                return true;
	    }
        }
    }

    //Ran off the map or out of range before finding a tank or obstacle
    return false;
}

/*
   canHitDownLeft
Authors: Sam Backes
Parameters:
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
*/
bool Edelweiss::canHitDownLeft(int x, int y)
{
    //For every space that's in range and not off the map
    for(int i = 1; x + i < height && y - i >= 0; i++)
    {
	if(y + i < height && x - i > -1)
	{
            //If we run into a obstacle first, return false
	    if(blocked(x - i, y + i))
       		return false;
	
       	    //If we find a tank first, return true
	    if((myMap[y + i][x - i] >= 1000) && !(x - i == myX && myY == y + i))
	    {
		return true;
	    }
        }
    }

    //Ran off the map or out of range before finding a tank or obstacle
    return false;
}

/*
   canHitDownRight
Authors: Sam Backes
Parameters:
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
*/
bool Edelweiss::canHitDownRight(int x, int y)
{
    //For every space that's in range and not off the map
    for(int i = 1; y + i < height && x + i < width; i++)
    {
	if(y + i < height && x + i < width)
	{
            //If we run into a obstacle first, return false
	    if(blocked(x + i, y + i))
		return false;
	
	    //If we find a tank first, return true
	    if((myMap[y + i][x + i] >= 1000) && !(x + i == myX && myY == y + i))
	    {
		return true;
	    }
	}
    }

    //Ran off the map or out of range before finding a tank or obstacle
    return false;	
}


