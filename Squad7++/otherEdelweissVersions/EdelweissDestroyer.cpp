#include "Edelweiss.h"
#include <iostream>
#include <algorithm>
using namespace std;



/*
   scanMap()
Authors: Dennis Kovarik, Sam Backes, Adeshkumar Naik 
Parameters
    MapData map - Struct given by platform representing the map
    PositionData status - Struct given by platform representing the tank's 
			  position on the map
Return Value
    none

This function scans the map. If an enemy tank is found on the map, then the 
function will add the (x, y) position of the enemy tank to the 'tankList'
vector. Otherwise, it will assing numerical values to each square on the map
and identify obsticles within the tank's radar. The numerical values assigned 
to each square represents the priority of where the tank should move to. Higher
values indicate a higher priority, and generally higher priorities are assigned 
to unvisited squares close by to the tank. Negative values represent obsticles, 
0 represents visited squares. 
*/
void Edelweiss::scanMap( MapData map, PositionData status )
{
    int minRadarX = status.game_x - myStats.tankRadar, minRadarY =  status.game_y - myStats.tankRadar;
    int maxRadarX = status.game_x + myStats.tankRadar, maxRadarY =  status.game_y + myStats.tankRadar;

    //Error checking
    if(minRadarX < 0 )
	minRadarX = 0 ;
    if (minRadarY < 0 )
	minRadarY = 0;
    if(maxRadarX > map.width)
	maxRadarX = map.width;
    if(maxRadarY > map.height)
	maxRadarY = map.height;

    for(int x= 0; x < map.width; ++x)
    {
	    for(int y = 0; y < map.height; ++y)
	    {
		    if(map.map[x + y * map.width] > 0 &&
		       map.map[x + y * map.width] != status.id)
		    {
			cout << "Tank Pos: (" << x << ", " << y << ")" << endl;
			cout << "Hi Tank" << endl;
			tankList.resize( tankList.size() + 1 );
			tankList.back().x = x;
			tankList.back().y = y;
			tankList.back().dis = sqrt( pow(status.game_x - x, 2) + pow(status.game_y - y, 2) );
			myMap[y][x] = 1000;
			coveringFire = myStats.tankHealth + 3;
		    }

		    //Mark positions within radar as seen indicated by value of 0
		    else if(map.map[x + y * map.width] == 0 && x >= minRadarX && x <= maxRadarX 
			&& y >= minRadarY && y <= maxRadarY && myMap[y][x] > 0)
		    {
			myMap[y][x] = 0;
		    }
		    //Negative vaules represents obsticles
		    else if(map.obstacleMap[x + y * map.width] == 'R' 
			|| map.obstacleMap[x + y * map.width] == 'W'|| map.obstacleMap[x + y * map.width] == 'C')
		    {
			myMap[y][x] = -10 ; 
		    }
		    //Some value marks trees
		    else if (  map.obstacleMap[x + y * map.width] == 'T')
		    {
			//Assign some value to identify trees
			//myMap[y][x] = 801 ;
		    }
		    //0 indicates squares have been visited
		    else if(myMap[y][x] > 0 && myMap[y][x] != 1000) 
		    {
			myMap[y][x] = 500 - ( abs(x - status.game_x) + abs(y - status.game_y) );
		    } 
	    }
    }

    sort( tankList.begin(), tankList.end(), tankSortCriteria );

    return;
}

/*
   advance()
Authors: Dennis Kovarik
Parameters
    int x - int passed by reference representing x value of next move
    int y - int passed by reference representing y value of next move
Return Value
    returns true if max value > 0 is found, otherwise it will return false

This function scans the myMap 2d vector that represents what the tank sees on
the map. It will look for the highest value placed on this map and return the 
x and y values of that position by reference.
*/
bool Edelweiss::explore(int &x, int &y, PositionData status)
{
    int maxVal = -1;

    //Make sure there is enough ap to explore
    if(curAp - (myStats.tankAP - exploreAP) <= 0)
	return false;

    //First loop checks that entire map is not explored
    do
    {
	//Looks for highest value on myMap
     	for(int i = 0; i < myMap.size(); i++)
    	{
	    for(int j = 0; j < myMap[i].size(); j++)
	    {
	    	if(myMap[i][j] > maxVal)
	    	{
		    maxVal = myMap[i][j];
		    y = i;
		    x = j;
	    	}
	    	cout << myMap[i][j] << " ";
	    }
	    cout << endl;
        }

	cout << endl;
	
	//If maxVal is <= 0, that means whole map explored
	//Reset myMap to reexplore map
	if(maxVal <= 0)
	    resetMyMap();

    }while(maxVal <= 0);

    return true;
}

/*
   retreat()
Authors: Dennis Kovarik
Parameters
    int x - int passed by reference representing x value of next move
    int y - int passed by reference representing  value of next move
Return Value
    returns true if max value > 0 is found, otherwise it will return false

This function scans the myMap 2d vector that represents what the tank sees on
the map. It will look for the highest value placed on this map and return the 
x and y values of that position by reference.
*/
bool Edelweiss::retreat(int &x, int &y, PositionData status, MapData map)
{
    spot newSpot;
    vector<spot> safeSpots;
    bool safe;
    enemyTank threat;
    double dis;
    int myX = status.game_x;
    int myY = status.game_y;
    x = status.game_x;
    y = status.game_y;
 
    //Check each adjacent tile
    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
	{
	    safe = true;
	
	    //Account for all know tanks
	    for(int t = 0; t < tankList.size() && safe; t++)
	    {
		threat = tankList[t];	    
		dis = sqrt( pow(myX + j - threat.x, 2) + pow(myY + i - threat.y, 2) );

		//if((/*canHitAnything(map, status, myX + j, myY + i) &&*/ blocked(map, status, myX + j, myY + i, true) && threat.dis >= dis))
		if(blocked(map, status, myX + j, myY + i, true))
		{
		    safe = false;
		} 
		else if(threat.dis >= dis)
		{
		    safe = false;
		}
		else if(canHitAnything(map, status, myX + j, myY + i))
		{
		    safe = false;
		}
	    }

	    if((i != 0 || j != 0) && safe)
	    {
		cout << endl;
 		newSpot.x = myX + j;
		newSpot.y = myY + i;
		safeSpots.push_back(newSpot);
	    }
	 }
    }

    random_shuffle(safeSpots.begin(), safeSpots.end());

    if(safeSpots.size() > 0)
    {
	x = safeSpots.front().x;
 	y = safeSpots.front().y;
	return true;
    }

    return false;
}

/*
   resetMyMap()
Authors: Dennis Kovarik
Parameters
    none
Return Value
    none

This function will reinitialize every nonzero value in myMap to 1.
*/
void Edelweiss::resetMyMap()
{
    for(int i = 0; i < myMap.size(); i++)
    {
	for(int j = 0; j < myMap[i].size(); j++)
	{
	    if(myMap[i][j] >= 0)
	    {
		myMap[i][j] = 1;
	    }
	}
    }
}

/*
   findPath()
Authors: Dennis Kovarik
Parameters
    int startx - int representing the starting x pos of tank
    int starty - int representing the starting y pos of tank
    int endx   - int representing the target x pos of tank
    int endy   - int representing the target y pos of tank
Return Value
    returns the next direction the tank should move to in order to get to the
	    the target destination.

Based on the starting and ending coordinates passed into this function, this
function performs a breadth-first search on the map to find the shortest path
from the starting coordinates to the ending coordinates. When the BFS is done,
this function will return the next direction along the shortest path from the 
starting coordinates to the ending coordinates. If there is no way to get to
the ending coordinates from the starting pos, then this function will make 
that position as an obsticle and return STAY.
*/
direction Edelweiss::findPath(int startx, int starty, int endx, int endy)
{
    queue<spot> next;
    vector< vector<spot> > explored;
    spot newSpot, cur, prev;
    int width, height;



    //Make graph for BFS
    explored.resize(myMap.size() + 1);
    height = myMap.size();
    
    if(height > 0)
	width = myMap[0].size();

    for(int i = 0; i < height; i++)
    {
	explored[i].resize(width + 1);
    }


    if(height <= 0 || width <= 0)
    {
	return STAY;
    }

    //Error checking
    if(startx < 0 || startx >= width || endx < 0 || endy > height)
    {
	return STAY;
    } 


    //make start node
    newSpot.x = startx;
    newSpot.y = starty;
    newSpot.prevx = startx;
    newSpot.prevy = starty;
    next.push(newSpot);

    while(!next.empty())
    {
	cur = next.front();
       	next.pop();
	cur.visited = true;
	//explored[cur.y][cur.x] = cur;
	
	//See if we reached target
	if(cur.x == endx && cur.y == endy)
	{
	    //Retrace Steps
	    while(cur.x != startx || cur.y != starty)
	    {
		prev = cur;
		cur = explored[cur.prevy][cur.prevx];
	    }

	    explored.clear();
	    return determineDirection(cur, prev);
	}

	//If not, continue BFS
	newSpot = cur;
	newSpot.prevx = cur.x;
	newSpot.prevy = cur.y;
	newSpot.visited = true;

	//Sim moving one more step in each direction
	for(int i = -1; i < 2; i++)
	{
	    for(int j = -1; j < 2; j++)
	    {
	        newSpot.y = cur.y + i;
	        newSpot.x = cur.x + j;

	     	if( newSpot.y >= 0 && newSpot.y < height && newSpot.x >= 0 
		   && newSpot.x < width && myMap[newSpot.y][newSpot.x] >= 0 
		   && !explored[newSpot.y][newSpot.x].visited)
	    	{
		    explored[newSpot.y][newSpot.x] = newSpot;
	 	    next.push(newSpot);
	      	}
	    }
	}
	
    }

    //If no way there, mark pos as obsticle and return STAY
    myMap[endy][endx] = -10;

    return STAY;
}

/*
   findPath()
Authors: Dennis Kovarik, Sam Backes, Adeshkumar Naik 
Parameters
    spot start - spot struct representing the starting position of tank
    spot next - spot struct representing an adjacent square on the map
Return Value
    returns the direction from the 'start' spot to the 'next' spot on the map..

Two spot structs are passed into this function. The spot struct 'start' 
represents the current position of the tank while the spot struct 'next'
represents the next position on the map that the tank will move to. This 
function returns the direction from the starting position to the next position
that the tank will move to on the map.
*/
direction Edelweiss::determineDirection(spot start, spot next)
{
    bool up, right;
    bool noHorizontal = false;
    bool noVertical = false;

    //Determine Horizontal Direction
    if(start.x - next.x > 0)
    	right = false;
    else if(start.x - next.x < 0)
	right = true;
    else
	noHorizontal = true;

    //Determine Vertical Direction
    if(start.y - next.y > 0)
	up = true;
    else if(start.y - next.y < 0)
	up = false;
    else
	noVertical = true;

    //Return verital or horizontal directions
    if(noHorizontal)
    {
	if(up)
	    return UP;
	else
	    return DOWN;
    }
    else if(noVertical)
    {
	if(right)
	    return RIGHT;
	else
	    return LEFT;
    }

    //Return diangle directions
    if(up && right)
    	return UPRIGHT;
    else if(up && !right)
	return UPLEFT;
    else if(!up && right)
	return DOWNRIGHT;
    else
	return DOWNLEFT;



    return STAY;
}


/*
   blocked
Authors: Sam Backes, Dennis Kovarik
Parameters:
   MapData map         - The map given by the platform
   PositionData status - Our current status given by the platform
   int x               - The X position we're checking
   int y               - The Y position we're checking
   bool tank           - True if we're checking a tank, false if a projectile. False by default
Return Value:
   true if the position (x,y) would be blocked if a tank and/or projectile tried to move through it
   false if the position (x,Y) would not be blocked if a tank and/or projectile tried to move through it

This functions just checks if a spot is blocked either by going off the map, obstacles are in the way, 
or if other bullets are in the way. If you pass in true for 'tank' it will also check if a tank would
be blocked and not just a projectile.
*/
bool Edelweiss::blocked(MapData map, PositionData status, int x, int y, bool tank)
{
    //Top/left side of map
    if(x < 0 || y < 0)
	return true;

    //Bottom/right side of map
    else if(x >= map.width || y >= map.height)
	return true;

    //Obstacles for tank
    else if(tank && (map.obstacleMap[x + y * map.width] == 'W'))
	return true;

    //Obstacles
    else if((map.obstacleMap[x + y * map.width] == 'R') || (map.obstacleMap[x + y * map.width] == 'C'))
	return true;

   //Ramming enemy tank
   else if(tank && (map.map[x + y * map.width] > 0))
	return true;

    //Bullets
    else if(x != status.game_x && y != status.game_y && map.map[x + y * map.width] < 0)
	return true;

    else
	return false;

}

bool tankSortCriteria(enemyTank t1, enemyTank t2)
{
    return t1.dis < t2.dis;
}

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
	    else if((map.map[x + (y - i) * map.width] > 0 || myMap[y-i][x] == 1000) 
		&& !(x == myX && myY == y - i))
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
            else if((map.map[x + (y + i) * map.width] > 0 || myMap[y+i][x] == 1000)
		&& !(x == myX && myY == y + i))
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
	    else if((map.map[(x - i) + y * map.width] > 0 || myMap[y][x-i] == 1000)
		&& !(x - i == myX && myY == y))
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
	    else if((map.map[(x + i) + y * map.width] > 0 || myMap[y][x+i] == 1000)
		&& !(x + 1 == myX && myY == y))
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
	    else if((map.map[(x - i) + (y - i) * map.width] > 0 || myMap[y-i][x-i] == 1000)
		&& !(x - i == myX && myY == y - i))
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
            else if((map.map[(x + i) + (y - i) * map.width] > 0 || myMap[y-i][x+i] == 1000)
		&& !(x + i == myX && myY == y - i))
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
            else if((map.map[(x - i) + (y + i) * map.width] > 0 || myMap[y+i][x-i] == 1000)
		&& !(x - i == myX && myY == y + i))
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
	    else if((map.map[(x + i) + (y + i) * map.width] > 0 || myMap[y+i][x+i] == 1000)
		&& !(x + i == myX && y == myY + i))
	    {
		return true;
	    }
	}
    }

    //Ran off the map or out of range before finding a tank or obstacle
    return false;	
}


bool Edelweiss::inPos(int x, int y, int enemyX, int enemyY)
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


bool Edelweiss::makePlan(MapData map, PositionData status, enemyTank threat, vector<spot> &vantagePoints)
{
    int x = status.game_x;
    int y = status.game_y;
    double dist;
    spot newSpot;
    vector<spot> inRangeOutRadar, outRangeOutRadar, outRangeInRadar;

    vantagePoints.clear();

    for(int i = -1; i < 2; i++)
    {
	for(int j = -1; j < 2; j++)
	{
	    dist = sqrt(pow(x + j - threat.x, 2) + pow(y + i - threat.y, 2));

	    if(!blocked(map, status, x + j, y + i, true) &&  !inRadar(x + j, y + i, threat) && canHitAnything(map,status, x + j, y + i))
	    {
		newSpot.x = x + j;
		newSpot.y = y + i;
		newSpot.dis = dist;
		inRangeOutRadar.push_back(newSpot);
		cout << "vantage point: (" << x + j << ", " << y + i << ")" << endl;
		cout << "threat: " << threat.x << " " << threat.y << endl;
	    }
	}
    }

    if(!inRangeOutRadar.empty())
    {
	cout << "InRangeOutRadar\n";
	vantagePoints.clear();
	vantagePoints = inRangeOutRadar;
	return true;
    }
    
    return false;


}


bool Edelweiss::inRadar(int x, int y, enemyTank threat)
{

    if(abs(x - threat.x) <= myStats.tankRadar)
    {
	if(abs(y - threat.y) <= myStats.tankRadar)
	{
	    return true;
	}
    }
    return false;
}


bool Edelweiss::shouldFight(PositionData status, MapData map)
{
    double dis = 0.0;
    int myX = status.game_x;
    int myY = status.game_y;
    enemyTank threat;
    bool safe;

    //Consider fleeing if low health or low ap
    //if(status.health <= 1 && myStats.tankHealth > 1)
    if((myStats.tankHealth > status.health && curAp < myStats.tankAP) || myStats.tankRadar < myStats.tankRange)
    {
	for(int i = -1; i < 2; i++)
	{
	    for(int j = -1; j < 2; j++)
	    {
		safe = true;
		
		//Account for all know tanks
		for(int t = 0; t < tankList.size() && safe; t++)
		{
		    threat = tankList[t];	    
		    dis = sqrt( pow(myX + j - threat.x, 2) + pow(myY + i - threat.y, 2) );

		    //if(canHitAnything(map, status, myX + j, myY + i) || threat.dis >= dis)
		    if((canHitAnything(map, status, myX + j, myY + i) && !blocked(map, status, myX + j, myY + i, true)) || threat.dis >= dis)
		    {
			safe = false;
		    }
		}

		if(safe)
		{
		    //Flee you fools
		    return false;
		}
	    }
	}
    }
    
    //Born in a world of strife!
    //Against the odds!
    //We choose to fight! 
    return true;
}

bool Edelweiss::danger()
{
    for(int i = 0; i < myMap.size(); i++)
    {
	for(int j = 0; j < myMap[i].size(); j++)
	{
	    if(myMap[i][j] == 1000)
	    	return true;
	}
    } 

    return false;
}

enemyTank Edelweiss::findClosestThreat(PositionData status)
{
    double dis = 0.0;
    double minDis = 100000.0;
    enemyTank threat;
    int myX = status.game_x;
    int myY = status.game_y;

    threat.x = 0;
    threat.y = 0;
    threat.dis = minDis;

    for(int i = 0; i < myMap.size(); i++)
    {
	for(int j = 0; j < myMap[i].size(); j++)
	{
	    if(myMap[i][j] == 1000)
	    {
		dis = sqrt(pow(myX - j, 2) + pow(myY - i, 2));

		if(dis < threat.dis)
		{
		    threat.x = j;
		    threat.y = i;
		    threat.dis = dis;
		} 
	    }
	}
    } 

    return threat;
}

int Edelweiss::spendAP(MapData map, PositionData status)
{
    //Allocate space for myMap
    if(myMap.size() == 0)
    {
	myMap.resize(map.height);

	for(int i = 0; i < map.height; i++)
	{
	    myMap[i].resize(map.width, 1);
	}	
    }

    //Keep track of our ap points
    if(curAp <= 0)
    {
	curAp = myStats.tankAP;
	engaged = false;
    }


    tankList.clear();
    run = false;
    scanMap(map, status);
    cout << "MyPos: (" << status.game_x << ", " << status.game_y << ")"  << endl;
    cout << "MyMap: " << myMap[0][0] << endl;

    //Fight of Flight
    if(!tankList.empty() && canHitAnything(map, status, status.game_x, 
	status.game_y))
    {
	if(shouldFight(status, map))
	{
	    cout << "Shoot Tank" << endl;
	    engaged = true;
	    return 2;
	}
	else if(engaged)
	{
	    return 2;
	}

	//If no fight then run
	run = true;
	cout << "Flee you fools" << endl;
	coveringFire = myStats.tankHealth + 3;
    	return 1;
    }
    else if(coveringFire && canHitAnything(map, status, status.game_x, 
	status.game_y))
    {
	coveringFire--;
	return 2;
    }

    //Reload
    if(curAp - (myStats.tankAP - exploreAP) <= 0 && myShots > 0)
    {
	cout << "Reload" << endl;
	myShots = 0;
	return 4;
    }

    return 1;
}

direction Edelweiss::move(MapData map, PositionData status)
{
    //Return value
    direction ret = STAY;
    direction nextDir;
    vector<spot> vantagePoints; 
    spot startSpot;
    int x, y;
    enemyTank threat;


    //If found tank, then attack
    if(!tankList.empty())
    {
	if(!run)
	{
	    curAp--;

	    x = tankList.front().x;
	    y = tankList.front().y;
	    threat = tankList.front();

	    if(makePlan(map, status, threat, vantagePoints))
	    {
		random_shuffle(vantagePoints.begin(), vantagePoints.end());
		startSpot.x = status.game_x;
		startSpot.y = status.game_y;
	        return findPath(status.game_x, status.game_y, vantagePoints.front().x, vantagePoints.front().y);
	    }
	    return findPath(status.game_x, status.game_y, tankList.front().x, tankList.front().y);
	}

	if(retreat(x, y, status, map))
	{
	    return findPath(status.game_x, status.game_y, x, y);
	}
	
	return STAY;
	
    }
    else if(coveringFire > 0)
    {
	threat = findClosestThreat(status);

	if(makePlan(map, status, threat, vantagePoints))
	{
	    random_shuffle(vantagePoints.begin(), vantagePoints.end());
	    startSpot.x = status.game_x;
	    startSpot.y = status.game_y;
	    return findPath(status.game_x, status.game_y, vantagePoints.front().x, vantagePoints.front().y);
	}

	return findPath(status.game_x, status.game_y, threat.x, threat.y);
    }

    cout << "coveringFire: " << coveringFire << endl;

    //Figure out the next position to go to
    if(explore(x, y, status))
    {
	curAp--;
	cout << "Explore (" << x << ", " << y << ")" << endl;
    	nextDir = findPath(status.game_x, status.game_y, x, y);

	return nextDir;
    }

    curAp--;
    return STAY;
}

direction Edelweiss::attack(MapData map, PositionData status)
{
    direction ret;

    if(canHitUpLeft(map, status, status.game_x, status.game_y))
	ret = UPLEFT;
    else if(canHitUp(map, status, status.game_x, status.game_y))
	ret = UP;
    else if(canHitUpRight(map, status, status.game_x, status.game_y))
	ret = UPRIGHT;
    else if(canHitLeft(map, status, status.game_x, status.game_y))
	ret = LEFT;
    else if(canHitRight(map, status, status.game_x, status.game_y))
	ret = RIGHT;
    else if(canHitDownLeft(map, status, status.game_x, status.game_y))
	ret = DOWNLEFT;
    else if(canHitDown(map, status, status.game_x, status.game_y))
	ret = DOWN;
    else if(canHitDownRight(map, status, status.game_x, status.game_y))
	ret = DOWNRIGHT;
    else
	ret = STAY;

    //Count shots
    if(ret != STAY)
	myShots++;

    curAp--;
    return ret;
}

/**
 * @author David Donahue
 * @par Description:
 * Returns the attributes for the tank
 * @param[in] pointsAvailable - available points for the distribution
 * @return attributes of the tank
 */
attributes Edelweiss::setAttribute(int pointsAvailable, attributes baseStats)
{
    attributes tankAttributes;
   
    myStats = baseStats; 

    curAp = myStats.tankAP;

    //Reduced AP points for when we see no tanks
    exploreAP = myStats.tankAP * (2.0 / 3);

    if(exploreAP < 1)
    {
	exploreAP = 1;
    }

    tankAttributes.tankHealth += pointsAvailable;

    return tankAttributes;
}

//Tank Constructor
Edelweiss::Edelweiss() 
{

}

//Destructor
Edelweiss::~Edelweiss() 
{
    tankList.clear();
    myMap.clear();
}

#ifdef DYNAMIC
extern "C" //required for runtime linking
{
    Actor * maker()
    {
        return new Edelweiss;
    }
}
#endif

