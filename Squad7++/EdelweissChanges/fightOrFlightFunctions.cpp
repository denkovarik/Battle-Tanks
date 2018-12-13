#include "Edelweiss.h"
using namespace std;



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
		    if((canHitAnything(myX + j, myY + i) && !blocked(myX + j, myY + i, true)) 
			|| threat.dis >= dis)
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


