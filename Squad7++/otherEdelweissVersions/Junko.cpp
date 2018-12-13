#include "Junko.h"
#include <iostream>
#include <algorithm>
using namespace std;

Junko::Junko() {}
Junko::~Junko() 
{
    myMap.clear();
}

bool Junko::blocked(MapData map, PositionData status, int x, int y)
{
    //Top/left side of map
    if(x < 0 || y < 0)
	return true;

    //Bottom/right side of map
    else if(x >= map.width || y >= map.height)
	return true;

    //Obstacles
    else if(map.obstacleMap[x + y * map.width] == 'R')
	return true; 
    else if(map.obstacleMap[x + y * map.width] == 'W')
	return true;
    else if(map.obstacleMap[x + y * map.width] == 'C')
	return true;

    //Enemies
    else if(map.map[x + y * map.width] > 0)
	return true;

    //Bullets
    else if(map.map[x + y * map.width] < 0)
	return true;
    //If nothing is in our way
    else
	return false;
}


void Junko::updateGlobalMap(MapData map, PositionData status )
{
    for(int i = 0; i < map.width; i++)
    {
	for(int j = 0; j < map.height; j++)
	{
	    if(i > status.game_x + myStats.tankRadar || i < status.game_x - myStats.tankRadar ||
	       j > status.game_y + myStats.tankRadar || j < status.game_y - myStats.tankRadar)
	    {
		--myMap[i + j * map.width];
	    }
	    else
	    {
		myMap[i + j * map.width] = 10;
	    }
	}
    }	
}



int Junko::findLocalSum(int x, int y, MapData map)
{
    int sum = 0; 

    for(int i = -myStats.tankRadar; i < myStats.tankRadar; i++)
	if(i + x >= 0 && i + x < map.width)
	    for(int j = -myStats.tankRadar; j <= myStats.tankRadar; j++)
		if(j + y >= 0 && j + y < map.height)
		    sum += myMap[i + j * map.width];

    return sum;
}


direction Junko::move(MapData map, PositionData status)
{
    //Contains the sum of each direction along with said direction
    vector<dirAndNum> sums(8);
    int x, y;

    //Updates the map so we have current information
    updateGlobalMap(map, status);

    //Set up the vector with the direction and sum
    sums[0].num = findLocalSum(status.game_x - 1, status.game_y - 1, map);
    sums[0].dir = UPLEFT;

    sums[1].num = findLocalSum(status.game_x, status.game_y - 1, map);
    sums[1].dir = UP;

    sums[2].num = findLocalSum(status.game_x + 1, status.game_y - 1, map);
    sums[2].dir = UPRIGHT;

    sums[3].num = findLocalSum(status.game_x - 1, status.game_y, map);
    sums[3].dir = LEFT;

    sums[4].num = findLocalSum(status.game_x + 1, status.game_y, map);
    sums[4].dir = RIGHT;

    sums[5].num = findLocalSum(status.game_x - 1, status.game_y + 1, map);
    sums[5].dir = DOWNLEFT;

    sums[6].num = findLocalSum(status.game_x, status.game_y + 1, map);
    sums[6].dir = DOWN;

    sums[7].num = findLocalSum(status.game_x + 1, status.game_y + 1, map);
    sums[7].dir = DOWNRIGHT;

    //Sort the vector with the lowest sum first
    sort(sums.begin(), sums.end(), sortNumLowestFirst);

    for(int i = 0; i < 8; i++)
    {
	getNewCoordinates(status, sums[i].dir, x, y);
	if(!blocked(map, status, x, y))
	    return sums[i].dir;
    }

    //If, for some reason, we're spawned in a box
    return STAY;
}

bool sortNumLowestFirst(dirAndNum d1, dirAndNum d2)
{
    return d1.dir < d2.dir;
}

void Junko::getNewCoordinates(PositionData status, direction newDirection, int&x, int &y)
{
    if( newDirection == UP )
    {
        x = status.game_x;
        y = status.game_y - 1;
    }
    else if( newDirection == UPRIGHT )
    {
        x = status.game_x + 1;
        y = status.game_y - 1;
    }
    else if( newDirection == RIGHT )
    {
        x = status.game_x + 1;
        y = status.game_y;
    }
    else if( newDirection == DOWNRIGHT )
    {
        x = status.game_x + 1;
        y = status.game_y + 1;
    }
    else if( newDirection == DOWN )
    {
        x = status.game_x;
        y = status.game_y + 1;
    }
    else if( newDirection == DOWNLEFT )
    {
        x = status.game_x - 1;
        y = status.game_y + 1;
    }
    else if( newDirection == LEFT )
    {
        x = status.game_x - 1;
        y = status.game_y;
    }
    else if( newDirection == UPLEFT )
    {
        x = status.game_x - 1;
        y = status.game_y - 1;
    }
}


direction Junko::attack(MapData map, PositionData status)
{
    direction ret;

    if(canHitUpLeft(map, status.game_x, status.game_y))
	ret = UPLEFT;
    else if(canHitUp(map, status.game_x, status.game_y))
	ret = UP;
    else if(canHitUpRight(map, status.game_x, status.game_y))
	ret = UPRIGHT;
    else if(canHitLeft(map, status.game_x, status.game_y))
	ret = LEFT;
    else if(canHitRight(map, status.game_x, status.game_y))
	ret = RIGHT;
    else if(canHitDownLeft(map, status.game_x, status.game_y))
	ret = DOWNLEFT;
    else if(canHitDown(map, status.game_x, status.game_y))
	ret = DOWN;
    else if(canHitDownRight(map, status.game_x, status.game_y))
	ret = DOWNRIGHT;
    else
	ret = STAY;

    return ret;
}

/*
	This function will just check if a tank were to fire in *any* direction, would it hit something this turn.
*/
bool Junko::canHitAnything(MapData map, int x, int y)
{
	bool ret = false;
	if(canHitUpLeft(map, x, y)
	|| canHitUp(map, x, y)
	|| canHitUpRight(map, x, y)
	|| canHitLeft(map, x, y)
	|| canHitRight(map, x, y)
	|| canHitDownLeft(map, x, y)
	|| canHitDown(map, x, y)
	|| canHitDownRight(map, x, y))
		return true;
	return false;
}

bool Junko::canHitUp(MapData map, int x, int y)
{	
        //For every space that's in range and not off the map
        for(int i = 1; i < 7; i++)
        {
		if(y - i > -1)
		{
                	//If we run into a obstacle first, return false
	                if(map.obstacleMap[x + (y - i) * map.width])
        	                return false;

                	//If we find a tank first, return true
	                else if( map.map[x + (y - i) * map.width] > 0)
        	                return true;
        	}
	}
        //Ran off the map or out of range before finding a tank or obstacle
        return false;	
}

bool Junko::canHitDown(MapData map, int x, int y)
{
        //For every space that's in range and not off the map
        for(int i = 1; i < 7; i++)
        {
		if(y + i < map.height)
		{
	                //If we run into a obstacle first, return false
        	        if(map.obstacleMap[x + (y + i) * map.width])
                	        return false;

	                //If we find a tank first, return true
        	        else if( map.map[x + (y + i) * map.width] > 0)
                 	        return true;
        	}
	}
        //Ran off the map or out of range before finding a tank or obstacle
        return false;	
}

bool Junko::canHitLeft(MapData map, int x, int y)
{
        //For every space that's in range and not off the map
        for(int i = 1; i < 7; i++)
        {
		if(x - i > -1)
		{
                	//If we run into a obstacle first, return false
	                if(map.obstacleMap[(x - i) + y * map.width])
        	                return false;

                	//If we find a tank first, return true
	                else if( map.map[(x - i) + y * map.width] > 0)
        	                return true;
        	}
	}
        //Ran off the map or out of range before finding a tank or obstacle
        return false;
}

bool Junko::canHitRight(MapData map, int x, int y)
{
        //For every space that's in range and not off the map
        for(int i = 1; i < 7; i++)
        {
		if(x + i < map.width)
		{
                	//If we run into a obstacle first, return false
	                if(map.obstacleMap[(x + i) + y * map.width])
        	                return false;

                	//If we find a tank first, return true
	                else if( map.map[(x + i) + y * map.width] > 0)
        	                return true;
        	}
	}
        //Ran off the map or out of range before finding a tank or obstacle
        return false;
}

bool Junko::canHitUpLeft(MapData map, int x, int y)
{
        //For every space that's in range and not off the map
        for(int i = 1; i < 7; i++)
        {
		if(y - i > -1 && x -i > -1)
		{
                	//If we run into a obstacle first, return false
	                if(map.obstacleMap[(x - i) + (y - i) * map.width])
        	                return false;

                	//If we find a tank first, return true
	                else if( map.map[(x - i) + (y - i) * map.width] > 0)
        	                return true;
        	}
	}
        //Ran off the map or out of range before finding a tank or obstacle
        return false;
}

bool Junko::canHitUpRight(MapData map, int x, int y)
{
        //For every space that's in range and not off the map
        for(int i = 1; i < 7; i++)
        {
		if(y - i > -1 && x + i < map.width)
		{
                	//If we run into a obstacle first, return false
                	if(map.obstacleMap[(x + i) + (y - i) * map.width])
                        	return false;

                	//If we find a tank first, return true
                	else if( map.map[(x + i) + (y - i) * map.width] > 0)
                        	return true;
        	}
	}
        //Ran off the map or out of range before finding a tank or obstacle
        return false;
}

bool Junko::canHitDownLeft(MapData map, int x, int y)
{
        //For every space that's in range and not off the map
        for(int i = 1; i < 7; i++)
        {
		if(y + i < map.height && x - i > -1)
		{
                	//If we run into a obstacle first, return false
	                if(map.obstacleMap[(x - i) + (y + i) * map.width])
        	                return false;
	
        	        //If we find a tank first, return true
                	else if( map.map[(x - i) + (y + i) * map.width] > 0)
			      	return true;
        	}
	}
        //Ran off the map or out of range before finding a tank or obstacle
        return false;
}

bool Junko::canHitDownRight(MapData map, int x, int y)
{
	//For every space that's in range and not off the map
	for(int i = 1; i < 7; i++)
	{
		if(y + i < map.height && x + i < map.width)
		{
			//If we run into a obstacle first, return false
			if(map.obstacleMap[(x + i) + (y + i) * map.width])
				return false;
	
			//If we find a tank first, return true
			else if( map.map[(x + i) + (y + i) * map.width] > 0)
				return true;
		}
	}
	//Ran off the map or out of range before finding a tank or obstacle
	return false;	
}


/**
 * @author David Donahue
 * @par Description:
 * Returns the attributes for the tank
 * @param[in] pointsAvailable - available points for the distribution
 * @return attributes of the tank
 */
attributes Junko::setAttribute(int pointsAvailable, attributes baseStats)
{
    attributes tankAttributes;
   
    myStats = baseStats; 

    tankAttributes.tankHealth += pointsAvailable;

    return tankAttributes;
}


int Junko::spendAP(MapData map, PositionData status)
{
    if(myMap.size() == 0)
    {
	myMap.resize((map.height - 1) + (map.width - 1) * map.width, 0);	
    }

    if(canHitAnything(map, status.game_x, status.game_y))
	return 2;

    return 1;
}

#ifdef DYNAMIC
extern "C" //required for runtime linking
{
    Actor * maker()
    {
        return new Junko;
    }
}
#endif
