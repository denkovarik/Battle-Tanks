#include "Edelweiss.h"
using namespace std;



/*
   canHitAnything
Authors: Sam Backes
Parameters:
   MapData map         - The map given by the platform
   PositionData status - Our current status given by the platform
   int x               - The X position we are checking
   int y               - The Y position we are checking
Return Values:
   true if (as far as we know) a tank firing in a direction from position (x,y) would be able to hit something
   false if (as far as we know) a tank firing in ANY direction from position (x,y) would not be able to hit ANY enemy tank

This function is just a huge function call to the other canHit functions. Basically it's just to check if we could hit anything 
if we were to fire. You can also check positions that are not your own to determine if a spot is safe. Although, since RADAR is
limited, the results from such a request may or may not be 100% accurate.
*/
bool Edelweiss::canHitAnything(MapData map, PositionData status, int x, int y)
{
	if(canHitUpLeft(map, status, x, y)
	|| canHitUp(map, status, x, y)
	|| canHitUpRight(map, status, x, y)
	|| canHitLeft(map, status, x, y)
	|| canHitRight(map, status, x, y)
	|| canHitDownLeft(map, status, x, y)
	|| canHitDown(map, status, x, y)
	|| canHitDownRight(map, status, x, y))
		return true;
	return false;
}

/*
   canHitUp
Authors: Sam Backes
Parameters:
   MapData map         - The map data given to us by the platform 
   PositionData status - Our current status given to us by the platform
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
Yes, this is a much better way of doing this than 8 different functions, but we're on a time limit here.
*/
bool Edelweiss::canHitUp(MapData map, PositionData status, int x, int y)
{	
    int myX = status.game_x;
    int myY = status.game_y;

    //For every space that's in range and not off the map
    for(int i = 1; i < myStats.tankRange + 1; i++)
    {
	if(y - i > -1)
	{
            //If we run into a obstacle first, return false
	    if(blocked(map, status, x, y - i))
                return false;

            //If we find a tank first, return true
	    //else if((map.map[x + (y - i) * map.width] > 0 || myMap[y-i][x] == 1000) 
		//&& !(x == myX && myY == y - i))
	    else if((myMap[y-i][x] == 1000) && !(x == myX && myY == y - i))
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
   MapData map         - The map data given to us by the platform
   PositionData status - Our current status given to us by the platform
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
Yes, this is a much better way of doing this than 8 different functions, but we're on a time limit here.
*/
bool Edelweiss::canHitDown(MapData map, PositionData status, int x, int y)
{
    int myX = status.game_x;
    int myY = status.game_y;

    //For every space that's in range and not off the map
    for(int i = 1; i < myStats.tankRange + 1; i++)
    {
	if(y + i < map.height)
	{
	    //If we run into a obstacle first, return false
            if(blocked(map, status, x, y + i))
            	return false;

	    //If we find a tank first, return true
          //  else if((map.map[x + (y + i) * map.width] > 0 || myMap[y+i][x] == 1000)
	//	&& !(x == myX && myY == y + i))
	    else if((myMap[y+i][x] == 1000) && !(x == myX && myY == y - i))
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
   MapData map         - The map data given to us by the platform
   PositionData status - Our current status given to us by the platform
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
Yes, this is a much better way of doing this than 8 different functions, but we're on a time limit here.
*/
bool Edelweiss::canHitLeft(MapData map, PositionData status, int x, int y)
{
    int myX = status.game_x;
    int myY = status.game_y;

    //For every space that's in range and not off the map
    for(int i = 1; i < myStats.tankRange + 1; i++)
    {
	if(x - i > -1)
	{
            //If we run into a obstacle first, return false
	    if(blocked(map, status, x - i, y))
                return false;

            //If we find a tank first, return true
	    //else if((map.map[(x - i) + y * map.width] > 0 || myMap[y][x-i] == 1000)
	//	&& !(x - i == myX && myY == y))
	    else if((myMap[y][x-i] == 1000) && !(x == myX && myY == y - i))
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
   MapData map         - The map data given to us by the platform
   PositionData status - Our current status given to us by the platform
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
Yes, this is a much better way of doing this than 8 different functions, but we're on a time limit here.
*/
bool Edelweiss::canHitRight(MapData map, PositionData status, int x, int y)
{
    int myX = status.game_x;
    int myY = status.game_y; 

    //For every space that's in range and not off the map
    for(int i = 1; i < myStats.tankRange + 1; i++)
    {
	if(x + i < map.width)
	{
            //If we run into a obstacle first, return false
	    if(blocked(map, status, x + i, + y))
                return false;

            //If we find a tank first, return true
	    //else if((map.map[(x + i) + y * map.width] > 0 || myMap[y][x+i] == 1000)
	//	&& !(x + 1 == myX && myY == y))
	    else if((myMap[y][x+1] == 1000) && !(x == myX && myY == y - i))
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
   MapData map         - The map data given to us by the platform
   PositionData status - Our current status given to us by the platform
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
Yes, this is a much better way of doing this than 8 different functions, but we're on a time limit here.
*/
bool Edelweiss::canHitUpLeft(MapData map, PositionData status, int x, int y)
{
    int myX = status.game_x;
    int myY = status.game_y;

    //For every space that's in range and not off the map
    for(int i = 1; i < myStats.tankRange + 1; i++)
    {
	if(y - i > -1 && x -i > -1)
	{
            //If we run into a obstacle first, return false
	    if(blocked(map, status, x - i, y - i))
                return false;

            //If we find a tank first, return true
	    //else if((map.map[(x - i) + (y - i) * map.width] > 0 || myMap[y-i][x-i] == 1000)
	//	&& !(x - i == myX && myY == y - i))
	    else if((myMap[y-i][x-i] == 1000) && !(x == myX && myY == y - i))
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
   MapData map         - The map data given to us by the platform
   PositionData status - Our current status given to us by the platform
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
Yes, this is a much better way of doing this than 8 different functions, but we're on a time limit here.
*/
bool Edelweiss::canHitUpRight(MapData map, PositionData status, int x, int y)
{
    int myX = status.game_x;
    int myY = status.game_y;

    //For every space that's in range and not off the map
    for(int i = 1; i < myStats.tankRange + 1; i++)
    {
	if(y - i > -1 && x + i < map.width)
	{
            //If we run into a obstacle first, return false
            if(blocked(map, status, x + i, y - i))
            	return false;

            //If we find a tank first, return true
            //else if((map.map[(x + i) + (y - i) * map.width] > 0 || myMap[y-i][x+i] == 1000)
	//	&& !(x + i == myX && myY == y - i))
	    else if((myMap[y-i][x+i] == 1000) && !(x == myX && myY == y - i))
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
   MapData map         - The map data given to us by the platform
   PositionData status - Our current status given to us by the platform
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
Yes, this is a much better way of doing this than 8 different functions, but we're on a time limit here.
*/
bool Edelweiss::canHitDownLeft(MapData map, PositionData status, int x, int y)
{
    int myX = status.game_x;
    int myY = status.game_y;

    //For every space that's in range and not off the map
    for(int i = 1; i < myStats.tankRange + 1; i++)
    {
	if(y + i < map.height && x - i > -1)
	{
            //If we run into a obstacle first, return false
	    if(blocked(map, status, x - i, y + i))
       		return false;
	
       	    //If we find a tank first, return true
            //else if((map.map[(x - i) + (y + i) * map.width] > 0 || myMap[y+i][x-i] == 1000)
	//	&& !(x - i == myX && myY == y + i))
	    else if((myMap[y+i][x-i] == 1000) && !(x == myX && myY == y - i))
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
   MapData map         - The map data given to us by the platform
   PositionData status - Our current status given to us by the platform
   int x               - The x position we want to check
   int y               - The y position we want to check
Return Value:
   true if firing in that direction would result in hitting an enemy tank
   false if firing in that direction would not result in hitting an enemy tank

This is just a simple function that checks if firing in a set direction would even do anything.
Yes, this is a much better way of doing this than 8 different functions, but we're on a time limit here.
*/
bool Edelweiss::canHitDownRight(MapData map, PositionData status, int x, int y)
{
    int myX = status.game_x;
    int myY = status.game_y;

    //For every space that's in range and not off the map
    for(int i = 1; i < myStats.tankRange + 1; i++)
    {
	if(y + i < map.height && x + i < map.width)
	{
            //If we run into a obstacle first, return false
	    if(blocked(map, status, x + i, y + i))
		return false;
	
	    //If we find a tank first, return true
	    //else if((map.map[(x + i) + (y + i) * map.width] > 0 || myMap[y+i][x+i] == 1000)
	//	&& !(x + i == myX && y == myY + i))
	    else if((myMap[y+i][x+i] == 1000) && !(x == myX && myY == y - i))
	    {
		return true;
	    }
	}
    }

    //Ran off the map or out of range before finding a tank or obstacle
    return false;	
}


