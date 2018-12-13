#include "Edelweiss.h"
#include <iostream>
#include <algorithm>
using namespace std;

Edelweiss::Edelweiss() {}
Edelweiss::~Edelweiss() {}

bool Edelweiss::blocked(MapData map, PositionData status, direction where)
{
    //If tank wants to move up, check if the edge of the map is there or an obstacle is in the way
    if(where == UP)
    {
	if(status.game_y == 0)
		return true;
	
	//Also check if you would be moving into an enemy or bullet
	else if(map.map[status.game_x + (status.game_y - 1) * map.width] != 0 &&
			map.map[status.game_x + (status.game_y - 1) * map.width] != -status.id)
		return true;		
        
	else
		return(map.obstacleMap[status.game_x + (status.game_y - 1) * map.width]);
    }
    //If tank wants to move right, check if the edge of the map is there or an obstacle is in the way
    else if(where == RIGHT)
    {
	if(status.game_x == map.width - 1)
		return true;
	
	//Also check if you would be moving into an enemy or bullet
        else if(map.map[(status.game_x + 1) + status.game_y * map.width] != 0 &&
	                map.map[(status.game_x + 1) + status.game_y * map.width] != -status.id)
	        return true;
	
	else
		return(map.obstacleMap[(status.game_x + 1) + status.game_y * map.width]);
    }
    //If tank wants to move down, check if the edge of the map is there or an obstacle is in the way
    else if(where == DOWN)
    {
	if(status.game_y == map.height - 1)
		return true;
	
	//Also check if you would be moving into an enemy or bullet
	else if(map.map[status.game_x + (status.game_y + 1) * map.width] != 0 &&
			map.map[status.game_x + (status.game_y + 1) * map.width] != -status.id)
	        return true;
    	
	else	
		return(map.obstacleMap[status.game_x + (status.game_y + 1) * map.width]);

    }
    //If tank wants to move left, check if the edge of the map is there or an obstacle is in the way
    else if(where == LEFT)
    {
	if(status.game_x == 0)
		return true;

	//Also check if you would be moving into an enemy or bullet
	else if(map.map[(status.game_x - 1) + status.game_y * map.width] != 0 &&
			map.map[(status.game_x - 1) + status.game_y * map.width] != -status.id)
                return true;
	
	else
		return(map.obstacleMap[(status.game_x - 1) + status.game_y * map.width]);
    }
    //If you put in a wrong direction
    else
	    return false;
}


direction Edelweiss::move(MapData map, PositionData status)
{
    //Return value
    direction ret = STAY;

    //First clear old data then gain new location data
    tankList.clear();
    findEnemyTanks( map, status );

    //Only accessing front tank for now to should be updated later
    //If enemy is further horizontally or vertically
    if(tankList.size() > 0 && (std::abs(status.game_x - tankList.front().x) 
	> std::abs(status.game_y - tankList.front().y)))
    {
	    //Are they to the left or right?
            if(status.game_x > tankList.front().x)
		    ret = moveLeft(map, status);
	    else
		    ret = moveRight(map, status);
    }
    else
    {
            //Are they above or below?
	    if(tankList.size() > 0 && status.game_y > tankList.front().y)
		    ret = moveUp(map, status);
	    else
		    ret = moveDown(map, status);
    }
     
    return ret;
}


void Edelweiss::findEnemyTanks( MapData map, PositionData status )
{
    for(int x = 0; x < map.width; ++x)
    {
	    for(int y = 0; y < map.height; ++y)
	    {
		    if(map.map[x + y * map.width] > 0 &&
		       map.map[x + y * map.width] != status.id)
		    {
			tankList.resize( tankList.size() + 1 );
			tankList.back().x = x;
			tankList.back().y = y;
			tankList.back().dis = sqrt( pow(status.game_x - x, 2) + pow(status.game_y - y, 2) );
		    }
	    }
    }

    sort( tankList.begin(), tankList.end(), tankSortCriteria );
    return;
}

//NOT TESTED
bool tankSortCriteria( enemyTank t1, enemyTank t2 )
{
    return t1.dis < t2.dis;
}


direction Edelweiss::moveDown(MapData map, PositionData status)
{
	if(!blocked(map, status, DOWN))
		return DOWN;

	//Side step either left or right
	else if(!tankList.empty())
	{
		return (status.game_x > tankList.front().x) 
			? moveLeft(map, status) : moveRight(map, status);
	}
	else
	{
		return UP;
	}
}

direction Edelweiss::moveRight(MapData map, PositionData status)
{
	if(!blocked(map, status, RIGHT))
		return RIGHT;

	//Side step either up or down
        else
                return (status.game_y > tankList.front().y) 
			? moveUp(map, status) : moveDown(map, status);
}

direction Edelweiss::moveUp(MapData map, PositionData status)
{
        if(!blocked(map, status, UP))
                return UP;

	//Side step either left or right
        else
                return (status.game_x > tankList.front().x) 
			? moveLeft(map, status) : moveRight(map, status);
}

direction Edelweiss::moveLeft(MapData map, PositionData status)
{
	if(!blocked(map, status, LEFT))
		return LEFT;

	//Side step either up or down
        else
	        return (status.game_y > tankList.front().y) 
			 ? moveUp(map, status) : moveDown(map, status);
}


direction Edelweiss::attack(MapData map, PositionData status)
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
bool Edelweiss::canHitAnything(MapData map, int x, int y)
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

bool Edelweiss::canHitUp(MapData map, int x, int y)
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

bool Edelweiss::canHitDown(MapData map, int x, int y)
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

bool Edelweiss::canHitLeft(MapData map, int x, int y)
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

bool Edelweiss::canHitRight(MapData map, int x, int y)
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

bool Edelweiss::canHitUpLeft(MapData map, int x, int y)
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

bool Edelweiss::canHitUpRight(MapData map, int x, int y)
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

bool Edelweiss::canHitDownLeft(MapData map, int x, int y)
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

bool Edelweiss::canHitDownRight(MapData map, int x, int y)
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


attributes Edelweiss::setAttribute(int pointsAvailable)
{
   attributes tankAttributes;

   tankAttributes.tankDamage += pointsAvailable;

   return tankAttributes;
}


int Edelweiss::spendAP(MapData map, PositionData status)
{
    tankList.clear();
    findEnemyTanks(map, status);

    if(canHitAnything(map, status.game_x, status.game_y))
	return 2;

    return 1;
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
