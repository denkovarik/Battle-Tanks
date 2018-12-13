#include "Edelweiss.h"
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
	    //Mark tanks once found
	    if(map.map[x + y * map.width] > 0 &&
	       map.map[x + y * map.width] != status.id)
	    {
		tankList.resize( tankList.size() + 1 );
		tankList.back().x = x;
		tankList.back().y = y;
		tankList.back().dis = sqrt( pow(status.game_x - x, 2) 
					+ pow(status.game_y - y, 2) );

	    	if(myMap[y][x] == 1010 && charge != true)
	    	{
		    charge = true;
	    	}

		if(myMap[y][x] < 1000)
		    myMap[y][x] = 1000;
		else if(myMap[y][x] >= 1000 && myMap[y][x] < 1010 && curAp >= myStats.tankAP)
		    myMap[y][x]++;

		coveringFire = 2;
	    }
	    //Used in checking that we don't run ahead of our bullets
	    else if(myMap[y][x] == -4)
	    {
		myMap[y][x]++;	   
	    }
	    else if(map.map[x + y * map.width] == status.id)
	    {
		myMap[y][x] = 1;
	    }
	    else if(myMap[y][x] == 1010 && charge != true)
	    {
		charge = true;
	    }
	    else if(myMap[y][x] >= 1000 && myMap[y][x] < 1010 && curAp >= myStats.tankAP)
	    {
		myMap[y][x]++;
	    }
	    //Negative vaules represents obsticles
	    //-10 value for obsticles
	    else if(map.obstacleMap[x + y * map.width] == 'R' 
		|| map.obstacleMap[x + y * map.width] == 'C')
	    {   
		myMap[y][x] = -10 ; 
	    }
 	    //-5 value for water
	    else if(map.obstacleMap[x + y * map.width] == 'W')
	    {
		myMap[y][x] = -5; 
	    }
	    //-4 for enemy bulletss
	    else if(map.map[x + y * map.width] < 0 && myMap[y][x] != -4)
	    {
		myMap[y][x] = -3;
	    } 
 	    //Mark our current position as visited
	    else if(x == status.game_x && y == status.game_y)
	    {
		myMap[y][x] = 0;
	    }
	    //Mark positions within radar as seen indicated by value of 0
	    else if(map.map[x + y * map.width] == 0 && x >= minRadarX && x <= maxRadarX 
		&& y >= minRadarY && y <= maxRadarY && myMap[y][x] != -4)
	    {
		if(myMap[y][x] == 1010)
		    charge = false;
		myMap[y][x] = 0;
	    }
	    //Some value marks trees
	    else if (  map.obstacleMap[x + y * map.width] == 'T')
	    {
		//Assign some value to identify trees
		//myMap[y][x] = 801 ;
	    }
	    //0 indicates squares have been visited
	    else if(myMap[y][x] > 0 && myMap[y][x] < 1000) 
	    {
		myMap[y][x] = 500 - ( abs(x - status.game_x) + abs(y - status.game_y) );
	    }
	}
    }

    //Sort enemyTanks which out tank currently sees by distance
    sort( tankList.begin(), tankList.end(), tankSortCriteria );

    return;
}

bool Edelweiss::shouldFight(PositionData status, MapData map)
{
    double dis = 0.0;
    int myX = status.game_x;
    int myY = status.game_y;
    enemyTank threat;
    bool safe;
    bool inEnemyRadar = false;



    //If we only have 2 AP and past turn 20, then charge
    if(status.health < myStats.tankHealth && myStats.tankAP <= 2 && counter / myStats.tankAP <= 25)
    {
	charge = true;
	return true;
    }
    //Retreat protocol for when tank is really low on health
    //Tank will retreat if tank lost all but 1 health point and is in radar
      //of an enemy tank
    else if((status.health < myStats.tankHealth || status.ammo <= 0) 
	&& curAp <= 1)
    {
	//Look for a safe spot to run to
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
    else if(counter / myStats.tankAP <= 15 && curAp <= 1)
    {
	//Look for a safe spot to run to
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

bool tankSortCriteria(enemyTank t1, enemyTank t2)
{
    return t1.dis < t2.dis;
}

bool Edelweiss::findPlan(std::vector<enemyTank> threats, 
	std::vector<spot> &vantagePoints)
{
    vector<spot> outRadarSpots;		//holds all spots out of radar of threat
    vector<spot> allRadarSpots;		//holds all spots in radar for threat
    queue<spot> next;			//queue for bfs search
    vector< vector<spot> > explored;	//visited vector for BFS
    spot newSpot, cur;
    bool done = false;
    int sep = 0;
    int minSep;
    bool outRadar;

    outRadarSpots.clear();
    allRadarSpots.clear();

    //If we are already in position, then return false
      //There is no point trying to move to our current spot
    if(canHitAnything(myX, myY))
    {
	return false;
    }

    vantagePoints.clear();

    //Make graph for BFS
    explored.clear();
    explored.resize(height + 1);
    
    for(int i = 0; i < height + 1; i++)
    {
	explored[i].resize(width + 1);
    }



    //make start node
    newSpot.x = myX;
    newSpot.y = myY;
    newSpot.separation = sep;
    next.push(newSpot);

    while(!next.empty())
    {
	cur = next.front();
       	next.pop();
	cur.visited = true;

	if(cur.y < height && cur.y >= 0 && cur.x < width && cur.x >= 0)
    	{
	    explored[cur.y][cur.x] = cur;
	
	    //See if we reached target
	    if(!(cur.x == myX && cur.y == myY) && canHitAnything(cur.x, cur.y))
	    {
		if(!done)
		{
		    minSep = cur.separation;
		}

		//Check if this spot is out of radar of all tanks
		outRadar = true;

		for(int i = 0; i < threats.size(); i++)
		{
		    if(inRadar(cur.x, cur.y, threats[i]))
		    {
			outRadar = false;
		    }
		}

	    	if(cur.separation == minSep && outRadar)
	    	{
		    outRadarSpots.push_back(cur);
	    	}
	    	else if(minSep == cur.separation)
	    	{
		    allRadarSpots.push_back(cur);
	    	}

	    	done = true;
	    }	
	}

	//set next node
	sep++;
	newSpot = cur;
	newSpot.visited = true;
	newSpot.separation = sep;

	//Sim moving one more step in each direction
	for(int i = -1; i < 2 && !done; i++)
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

    if(!outRadarSpots.empty())
    {
	vantagePoints = outRadarSpots;
	return true;
    }
    else if(!allRadarSpots.empty())
    {
	vantagePoints = allRadarSpots;
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

bool Edelweiss::inRange(int x, int y, enemyTank threat)
{
    if(abs(x - threat.x) <= myStats.tankRange)
    {
	if(abs(y - threat.y) <= myStats.tankRange)
	{
	    return true;
	}
    }
    return false;
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

vector<enemyTank> Edelweiss::findThreats()
{
    double dis = 0.0;
    enemyTank threat;
    vector<enemyTank> threats;

    threat.x = 0;
    threat.y = 0;

    for(int i = 0; i < myMap.size(); i++)
    {
	for(int j = 0; j < myMap[i].size(); j++)
	{
	    if(myMap[i][j] >= 1000)
	    {
		dis = sqrt(pow(myX - j, 2) + pow(myY - i, 2));
		threat.x = j;
		threat.y = i;
		threat.dis = dis; 
		threats.push_back(threat);
	    }
	}
    } 

    return threats;
}
