#include "Edelweiss.h"
#include <iostream>
#include <algorithm>
using namespace std;

Edelweiss::Edelweiss() {}

Edelweiss::~Edelweiss() 
{
    tankList.clear();
    myMap.clear();
}

bool Edelweiss::blocked(MapData map, PositionData status, int x, int y)
{
    //Top/left side of map
    if(x < 0 || y < 0)
	return true;

    //Bottom/right side of map
    else if(x >= map.width || y >= map.height)
	return true;

    //Obstacles
    else if(map.obstacleMap[x + y * map.width] == ('R' ) 
		|| map.obstacleMap[x + y * map.width] == ('C' ) 
		|| map.obstacleMap[x + y * map.width] == ('W' ))
	return true;

    //Enemies
    else if(map.map[x + y * map.width] > 0)
	return true;

    //Bullets
    else if(map.map[x + y * map.width] < 0)
	return true;

    else
	return false;

}

void Edelweiss::getNewCoordinates(PositionData status, direction newDirection, int &x, int &y)
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

int Edelweiss::areaExplore(int x, int y, PositionData status, MapData map)
{
    int zeros = 0; 

    //for(i; i < map.height && i <= y + status.radar; i++)
    for(int i = 0; i < map.height; i++)
    {
	//for(j; j < map.width && j <= x + status.radar; j++) 
	for(int j = 0; j < map.width; j++)
	{
	    if(myMap[i+j*map.width] <= 0)
	    {
		zeros++;
	    }
	} 
    }   
    return zeros;
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

void Edelweiss::updateMap(MapData map, PositionData status )
{
    for( int i = 0 ; i < map.height; i++)
    {
      	for (int j = 0; j < map.width; j++) 	
	{
	   // if( abs(i - status.game_y) <= status.radar && abs(j - status.game_x) - j <= status.radar )
	    if( abs(i - status.game_y) <= 2 && abs(j - status.game_x) - j <= 2 )
	    {
		myMap[i+j*map.width] = 10;
	    }
	    else if(myMap[i+j*map.width] > 0)
	    {
		myMap[i+j*map.width]--;
	    }

	}
    }


}

direction Edelweiss::move(MapData map, PositionData status)
{
    //Return value
    direction ret = STAY;
    int currentMax = 0;
    int directionOfMax = 0;
    int maxZeros[8];
    int x, y;
    vector<direction> moves;

    moves.push_back(UP);
    moves.push_back(UPRIGHT);
    moves.push_back(RIGHT);
    moves.push_back(DOWNRIGHT);
    moves.push_back(DOWN);
    moves.push_back(DOWNLEFT);
    moves.push_back(LEFT);
    moves.push_back(UPLEFT);

    random_shuffle( moves.begin(), moves.end() );

    for(int i = 0; i < moves.size(); i++)
    {
	getNewCoordinates(status, moves[i], x, y);
        if(!blocked(map, status, x, y))
	{
	    cout << "Current: " << status.game_x << " " << status.game_y << endl;
	    cout << "    New: " << x << " " << y << endl << endl;
	    return moves[i];
        }
  	x = status.game_x;
	y = status.game_y;
	cout << "Current: " << status.game_x << " " << status.game_y << endl;
	cout << "    New: " << x << " " << y << endl << endl;
    }

    return STAY;
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

    tankAttributes.tankHealth += pointsAvailable;

    return tankAttributes;
}


int Edelweiss::spendAP(MapData map, PositionData status)
{
    if(myMap.size() == 0)
    {
	myMap.resize((map.height - 1) + (map.width - 1) * map.width, 0);	
    }

    tankList.clear();
    findEnemyTanks(map, status);
    updateMap(map,status);

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
