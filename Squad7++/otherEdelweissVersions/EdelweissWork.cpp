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
    else if(map.obstacleMap[x + y * map.width] == (R||W||C))
	return true;

    //Enemies
    else if(map.map[x + y * map.width] > 0)
	return true;

    //Bullets
    else if(map.map[x + y * map.width] < 0)
	return true;

    else
	return false;

/*
    if(where == UPLEFT)
    {
	if(status.game_y == 0 || status.game_x == 0)
		return true;
	else if(map.map[(status.game_x - 1) + (status.game_y - 1) * map.width] != -status.id)
    }
    
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
*/
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


direction Edelweiss::move(MapData map, PositionData status)
{
    //Return value
    direction ret = STAY;
    int currentMax = 0;
    int directionOfMax = 0;
    int maxZeros[8];
    vector<int> maxDir;
 
    //Only accessing front tank for now to should be updated later
    //If enemy is further horizontally or vertically
    if(!tankList.empty())
	{
	    if(tankList.size() > 0 && (std::abs(status.game_x - tankList.front().x) > std::abs(status.game_y - tankList.front().y)))
    	    {
	    	//Are they to the left or right?
            	if(status.game_x > tankList.front().x)
			    return moveLeft(map, status);
	    	else
			    return moveRight(map, status);
    	    }
    	    else
    	    {
            	//Are they above or below?
	    	if(tankList.size() > 0 && status.game_y > tankList.front().y)
			    return moveUp(map, status);
	    	else
			    return moveDown(map, status);
    	    }
	}

    maxZeros[0] = areaExplore(status.game_x - 1, status.game_y - 1, status, map);
    maxZeros[1] = areaExplore(status.game_x, status.game_y - 1, status, map);
    maxZeros[2] = areaExplore(status.game_x + 1, status.game_y - 1, status, map);
    maxZeros[3] = areaExplore(status.game_x - 1, status.game_y, status, map);
    maxZeros[4] = areaExplore(status.game_x + 1, status.game_y, status, map);
    maxZeros[5] = areaExplore(status.game_x - 1, status.game_y + 1, status, map);
    maxZeros[6] = areaExplore(status.game_x, status.game_y + 1, status, map);
    maxZeros[7] = areaExplore(status.game_x + 1, status.game_y + 1, status, map);

    for(int i = 0; i < 8; i++)
	{
	    cout<<"maxZeros[" << i << "] == " << maxZeros[i] << endl;
	    if(maxZeros[i] > currentMax)
	    {
		currentMax = maxZeros[i];
		directionOfMax = i;
	    }
	}
	
    for(int i = 0; i < 8; i++)
	{
	    if(maxZeros[i] == currentMax)
	    {
		maxDir.push_back( i );
	    }
	}


    if(maxDir.size() > 0)
    {
    	random_shuffle(maxDir.begin(), maxDir.end());
        directionOfMax = maxDir[0];
    }

    maxDir.clear();

    switch(directionOfMax) {
	case 0: 
		cout << "UpLeft" << endl;
		//return moveUpLeft(map, status);
		return moveUp(map, status);
		break;
	case 1: 
		cout << "up" << endl;
		return moveUp(map, status);
                break;
	case 2: 
		return moveUp(map, status);
		//return moveUpRight(map, status);
		break ;
	case 3: 
		return moveLeft(map, status);
		break;
	case 4: 
		return moveRight(map, status);
                break;
	case 5: 
		cout << "DownLeft" << endl;	
		return moveUp(map, status);
		//return moveDownLeft(map,status);
		break;
	case 6: 
		return moveDown(map,status);
		break;
	case 7: 
		cout << "moveDownRight" << endl;	
		return moveUp(map, status);
		//return moveDownRight(map,status);
		break;
	default: 
		cout << "default" << endl;
		return moveUp(map,status);
		break;
	};
     
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
	vector<direction> dirs;
	direction myDirection;
	int i;

	if(!blocked(map, status, status.game_x, status.game_y + 1))
		return DOWN;


	else if(!tankList.empty())
	{
		return (status.game_x > tankList.front().x) 
			? moveLeft(map, status) : moveRight(map, status);
	}
	else
	{
		dirs.push_back( UP );
		dirs.push_back( RIGHT );
		dirs.push_back( LEFT );

		for(i = 0; i < dirs.size(); i++)
		{
		    if(!blocked(map, status, dirs[i]))
		    {
			myDirection = dirs[i];
			dirs.clear();
			return myDirection;
		    }
		}
	}
}

direction Edelweiss::moveRight(MapData map, PositionData status)
{
	vector<direction> dirs;
	direction myDirection;
	int i;

	if(!blocked(map, status, RIGHT))
		return RIGHT;

	else if(!tankList.empty())
	{
		return (status.game_x > tankList.front().x) 
			? moveLeft(map, status) : moveRight(map, status);
	}
	else
	{
		dirs.push_back( UP );
		dirs.push_back( DOWN );
		dirs.push_back( LEFT );

		random_shuffle(dirs.begin(), dirs.end());

		for(i = 0; i < dirs.size(); i++)
		{
		    if(!blocked(map, status, dirs[i]))
		    {
			myDirection = dirs[i];
			dirs.clear();
			return myDirection;
		    }
		}

	}
}

direction Edelweiss::moveUp(MapData map, PositionData status)
{
	vector<int> dirs;
	direction myDirection;
	int intDirection;
	int i;
	bool stop = false;

        if(!blocked(map, status, UP))
                return UP;

	else if(!tankList.empty())
	{
		return (status.game_x > tankList.front().x) 
			? moveLeft(map, status) : moveRight(map, status);
	}
	else
	{
		//return RIGHT;
		dirs.push_back( 1 );
		dirs.push_back( 2 );
		dirs.push_back( 3 );

		random_shuffle(dirs.begin(), dirs.end());

		for(i = 0; i < dirs.size(); i++)
		{
		    switch(dirs[i])
		    {
		    case 1: 
			myDirection = RIGHT;
			break;
		    case 2: 
			myDirection = DOWN;
			break;
		    case 3:
			myDirection = LEFT; 
			break;
		    default: 
			myDirection = RIGHT;
	 	    };

		    if(!blocked(map, status, myDirection))
		    {
			dirs.clear();
			return myDirection;
		    }
		}

	}
}

direction Edelweiss::moveLeft(MapData map, PositionData status)
{
	vector<direction> dirs;
	direction myDirection;
	int i;

	if(!blocked(map, status, LEFT))
		return LEFT;

	else if(!tankList.empty())
	{
		return (status.game_x > tankList.front().x) 
			? moveLeft(map, status) : moveRight(map, status);
	}
	else
	{
		dirs.push_back( UP );
		dirs.push_back( RIGHT );
		dirs.push_back( DOWN );

		for(i = 0; i < dirs.size(); i++)
		{
		    if(!blocked(map, status, dirs[i]))
		    {
			myDirection = dirs[i];
			dirs.clear();
			return myDirection;
		    }
		}

	}
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
