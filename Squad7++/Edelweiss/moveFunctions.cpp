#include "Edelweiss.h"
using namespace std;



/*
   explore()
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
    spot newspot;
    vector<spot> move;
    move.clear();

    //First loop checks that entire map is not explored
    do
    {
	//Looks for highest value on myMap
     	for(int i = 0; i < myMap.size(); i++)
    	{
	    for(int j = 0; j < myMap[i].size(); j++)
	    {
	    	if(myMap[i][j] >= maxVal)
	    	{
	            if(myMap[i][j] == maxVal)
		    {
			newspot.y = i; 
			newspot.x = j;
			move.push_back(newspot);
		    }
		   else{
		        move.clear();			
			newspot.y = i; 
			newspot.x = j;
			move.push_back(newspot);
		   }
		    maxVal = myMap[i][j];
	    	}
	    }
        }
	
	//If maxVal is <= 0, that means whole map explored
	//Reset myMap to reexplore map
	if(maxVal <= 0)
	    resetMyMap();

    }while(maxVal <= 0);

    //random_shuffle(move.begin(),move.end());
    y = move[0].y ;
    x = move[0].x ;
    return true;
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

    if(startx == endx && starty == endy)
	return STAY;

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
	explored[cur.y][cur.x] = cur;
	
	//See if we reached target
	if(cur.x == endx && cur.y == endy)
	{
	    //Retrace Steps
	    while((cur.x != startx || cur.y != starty) || !(cur.x == cur.prevx && cur.y == cur.prevy))
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
    if(endy >= 0 && endy < height && endx >= 0 && endx < width)
    	myMap[endy][endx] = -10;
    enroute = false;
    
    return STAY;
}

/*
   determinDirection()
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


    //Error Checking
    if(start == next)
	return STAY;

    if(start.x < 0 || (myMap.size() > 0 &&start.x >= myMap[0].size()))
    {
	return STAY;
    }

    if(start.x < 0 || (myMap.size() > 0 &&start.x >= myMap[0].size()))
    {
	return STAY;
    }

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
bool Edelweiss::blocked(int x, int y, bool tank)
{
    //Top/left side of map
    if(x < 0 || y < 0)
	return true;

    //Bottom/right side of map
    else if(x >= width || y >= height)
	return true;

    //Obstacles for tank
    else if(tank && myMap[y][x] == -5)
	return true;

    //Obstacles
    else if(myMap[y][x] < -5)
	return true;

   //Ramming enemy tank
   else if(tank && myMap[y][x] == 1000)
	return true;

    //Bullets
    else if(myMap[y][x] == -3 || myMap[y][x] == -4)
	return true;

    
    return false;
}

/*

  convertDirectionToPosition()

  Authors: Dennis Kovarik, Sam Backes and Adeshkumar Naik

  Parameters: int x - X position
              int y - Y position 
              direction where - Direction to move

  Returns
       none

  In this function we calculate the distance of x and y which are passed
  are passed by reference. 

*/

void Edelweiss::convertDirectionToPosition(int &x, int &y, direction where)
{
    if(where == UP)
    {
	y = y - 1;
	return;
    }
    if(where == DOWN)
    {
	y = y + 1;
	return;
    }
    if(where == RIGHT)
    {
	x = x + 1;
	return;
    }
    if(where == LEFT)
    {
	x = x - 1;
	return;
    }
    if(where == UPRIGHT)
    {
	y = y - 1;
	x = x + 1;
	return;
    }
    if(where == UPLEFT)
    {
	y = y - 1;
	x = x - 1;
	return;
    }
    if(where == DOWNRIGHT)
    {
	y = y + 1;
	x = x + 1;
	return;
    }
    if(where == DOWNLEFT)
    {
	y = y + 1;
	x = x - 1;
	return;
    }

    return;
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
    vector<spot> outRadarSafeSpots;
    vector<enemyTank> knownThreats;
    bool safe;
    enemyTank threat;
    double dis;
    x = status.game_x;
    y = status.game_y;
    bool outRadar;

    findThreats();
 
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

		if(blocked(myX + j, myY + i, true))
		{
		    safe = false;
		} 
		else if(threat.dis >= dis)
		{
		    safe = false;
		}
		else if(canHitAnything(myX + j, myY + i))
		{
		    safe = false;
		}
	    }

	    if((i != 0 || j != 0) && safe)
	    {
 		newSpot.x = myX + j;
		newSpot.y = myY + i;
		outRadar = true;

		for(int i = 0; i < knownThreats.size(); i++)
		{
		    if(inRadar(newSpot.x, newSpot.y, knownThreats[i]))
		    {	
			outRadar = false;
		    }
		}

		if(outRadar)
		{
		    outRadarSafeSpots.push_back(newSpot);	
		}
		else
		{
		    safeSpots.push_back(newSpot);
		}
	    }
	 }
    }

    if(!outRadarSafeSpots.empty())
    {
    	random_shuffle(outRadarSafeSpots.begin(), outRadarSafeSpots.end());
	x = outRadarSafeSpots.front().x;
 	y = outRadarSafeSpots.front().y;
	return true;
    }
    else if(safeSpots.size() > 0)
    {
      	random_shuffle(safeSpots.begin(), safeSpots.end());
	x = safeSpots.front().x;
 	y = safeSpots.front().y;
	return true;
    }

    return false;
}

