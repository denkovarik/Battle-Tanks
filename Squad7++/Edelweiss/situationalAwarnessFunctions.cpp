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
    //The x and y values of the tank's radar
    int minRadarX = status.game_x - myStats.tankRadar;
    int minRadarY =  status.game_y - myStats.tankRadar;
    int maxRadarX = status.game_x + myStats.tankRadar;
    int maxRadarY =  status.game_y + myStats.tankRadar;

    //Error checking for x and y radar values
    if(minRadarX < 0 )
	minRadarX = 0 ;
    if (minRadarY < 0 )
	minRadarY = 0;
    if(maxRadarX > map.width)
	maxRadarX = map.width;
    if(maxRadarY > map.height)
	maxRadarY = map.height;

    //This block scans the entire map
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

		//Charge if found a camper
	    	if(myMap[y][x] == 1005 && charge != true)
	    	{
		    charge = true;
	    	}
		
		//Mark tank on myMap
		if(myMap[y][x] < 1000)
		    myMap[y][x] = 1000;
		//Mechanism for finding campers
		else if(myMap[y][x] >= 1000 && myMap[y][x] < 1005 && curAp >= myStats.tankAP)
		    myMap[y][x]++;

		coveringFire = myStats.tankHealth;
	    }
 	    //Mark our current position as visited
	    else if(x == status.game_x && y == status.game_y)
	    {
		myMap[y][x] = 0;
		//myMap[y][x] = 1;
	    }
	    //Charge if camper stored on myMap
	    else if(myMap[y][x] >= 1005 && charge != true)
	    {
		charge = true;
	    }
	    //Mechanism for finding campers not currently in sight
	    else if(myMap[y][x] >= 1000 && myMap[y][x] < 1005 && curAp >= myStats.tankAP)
	    {
		myMap[y][x]++;
	    }
 	    //-5 value for water
	    else if(map.obstacleMap[x + y * map.width] == 'W')
	    {
		myMap[y][x] = -5; 
	    }
	    //Negative vaules represents obsticles
	    //-11 value for crates
	    else if(map.obstacleMap[x + y * map.width] == 'C')
	    {   
		myMap[y][x] = -11; 
	    }
	    else if(myMap[y][x] == -11)
	    {
		myMap[y][x] = -10;
	    }
	    else if(myMap[y][x] > 1 && myMap[y][x] < 1000 && map.obstacleMap[x + y * map.width] == 'B') 
	    {
		myMap[y][x] = 0;
	    }
	    //Higher values on myMap for trees
	    else if(myMap[y][x] > 1 && myMap[y][x] < 1000 && map.obstacleMap[x + y * map.width] == 'T') 
	    {
		myMap[y][x] = 700 - ( abs(x - status.game_x) + abs(y - status.game_y) );
	    }
	    //Negative vaules represents obsticles
	    //-10 value for rocks
	    else if(map.obstacleMap[x + y * map.width] > 0)
	    {   
		myMap[y][x] = -10; 
	    }
	    //-4 for enemy bulletss
	    else if(map.map[x + y * map.width] < 0 && myMap[y][x] != -4 
		&& myMap[y][x] < 1000)
	    {
		myMap[y][x] = -3;
	    } 
	    //Mark positions within radar as seen indicated by value of 0
	    else if(map.map[x + y * map.width] == 0 && x >= minRadarX && x <= maxRadarX 
		&& y >= minRadarY && y <= maxRadarY && myMap[y][x] != -4)
	    {
		if(myMap[y][x] >= 1000)
		{
		    charge = false;
		    myMap[y][x] = 999 - ( abs(x - status.game_x) + abs(y - status.game_y) );
		}
		else if(myMap[y][x] < 900)
		{
		    myMap[y][x] = 0;
		}
	    }
	    else if(myMap[y][x] > 0 && myMap[y][x] < 1000 && (x == 1 || x == width - 2 || y == 1 || y == height - 2)) 
	    {
		myMap[y][x] = 700 - ( abs(x - status.game_x) + abs(y - status.game_y) );
	    }
	    else if(myMap[y][x] > 0 && myMap[y][x] < 1000) 
	    {
		myMap[y][x] = 500 - ( abs(x - status.game_x) + abs(y - status.game_y) );
	    }
	    //Used in checking that we don't run ahead of our bullets

	    if(myMap[y][x] == -4)
		myMap[y][x]++;	   
	}
    }

    //Sort enemyTanks which out tank currently sees by distance
    sort( tankList.begin(), tankList.end(), tankSortCriteria );

    return;
}
/*
 shouldFight()
 Authors: Dennis Kovarik, Samuel Backes, Adeshkumar Naik

 Parameters :
 	PositionData status - current position 
        MapData map - Map Data

 Returns:
  	True if we should fight
	False if we should not fight and there is a safe spot

 This function checks for all known enemy tanks and checks if we should fight 
 them by checking if there is spot that we can move to such that
 we can hit something and are not blocked and the threat is at greater distance
 than our current distance.

*/
bool Edelweiss::shouldFight(PositionData status, MapData map)
{
    double dis = 0.0;
    int myX = status.game_x;
    int myY = status.game_y;
    enemyTank threat;
    bool safe;
    bool inEnemyRadar = false;



    //Retreat protocol for when tank is really low on health
    //Tank will retreat if tank lost all but 1 health point and is in radar
      //of an enemy tank
    if(((status.health <= 1 && myStats.tankHealth) || status.ammo <= 0) && curAp <= 1)
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

    if((status.health < myStats.tankHealth || status.ammo <= 0) && curAp <= 1)
    {
	//Looks for any adjacent tiles on map
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

/*
   tankSortCriteria()
Authors: Sam Backes, Dennis Kovarik, Adeshkumar Naik
Parameters:
    enemyTank t1  - First enemy tank
    enemyTank t2  - Second enemy tank
Return Value:
    true if t1 is closer than t2
    false if t2 is closer than t1

This function just checks which of the two 2 enemy tanks passed to is 
closer to our tank
*/

bool tankSortCriteria(enemyTank t1, enemyTank t2)
{
    return t1.dis < t2.dis;
}

/*
   findPlan()
 Authors: Sam Backes, Dennis Kovarik, Adeshkumar Naik
 Parameters:
     MapData map            - The map given by the platform
     PositionData status    - Our current status given by the platform
     enemyTank threat       - enemyTank
     vector<spot> &vantagePoints - Spots to move 
 Return Value:
     true if we have one or more spots that are out of radar and in range
     false if we have none
 This function will calculate best 9 spots in and around the current the
 current spot. If the 

*/
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

    //The BFS search
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

    //Return spots that are both out of radar and in line of fire
    if(!outRadarSpots.empty())
    {
	vantagePoints = outRadarSpots;
	return true;
    }
    //Else just return spots that are in line of fire
    else if(!allRadarSpots.empty())
    {
	vantagePoints = allRadarSpots;
	return true;
    }

    //Return false if no spots were found
    return false;
}

/*
  inRadar(int x, int y, enemyTank threat)
  Authors: Sam Backes, Dennis Kovarik, Adeshkumar Naik
  
  Parameters: 
       int x - X coordinate of the position we are trying to move to
       int y - Y coordinate of the position we are trying to move to
       enemyTank threat - Enemy tank

  Returns
	True if the we are in Radar of a enemyTank
	False if we are not 
  This function computes whether we are in Radar of a enemyTank checking
  for both the x and y coordinates
 */
 
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

/*
  inRange(int x, int y, enemyTank threat)
  Authors: Sam Backes, Dennis Kovarik, Adeshkumar Naik
  
  Parameters: 
       int x - X coordinate of the position we are trying to move to
       int y - Y coordinate of the position we are trying to move to
       enemyTank threat - Enemy tank

  Returns
	True if the we are in Range of a enemyTank
	False if we are not 
  This function computes whether we are in Range of a enemyTank checking
  for both the x and y coordinates
 */
 
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
/*

 danger()

 Authors: Dennis Kovarik
 
 Parameters - None

 Return
   True - if we find a tank ( value on myMap will be above 1000 )

  This function basically returns true if we find a tank on the myMap
  else it returns false

*/

bool Edelweiss::danger()
{
    //Scan myMap to look for known enemy locations
    for(int i = 0; i < myMap.size(); i++)
    {
	for(int j = 0; j < myMap[i].size(); j++)
	{
	    if(myMap[i][j] >= 1000)
	    	return true;
	}
    } 

    return false;
}
/*

  findThreats()

   Authors: Dennis Kovarik, Sam Backes, Adeshkumar Naik

   Parameters : None

   Returns 
     True if knownThreats vector is not empty 
     False if it is empty

  This function scans the myMap to look for enemy threats and if it
  finds it then adds it on to the vector which is sorted based on closed
  ones.

*/
bool Edelweiss::findThreats()
{
    double dis = 0.0;
    enemyTank threat;
    threat.x = 0;
    threat.y = 0;

    knownThreats.clear();

    //Scans myMap to look for known enemy threats
    for(int i = 0; i < myMap.size(); i++)
    {
	for(int j = 0; j < myMap[i].size(); j++)
	{
	    //If found threat, then add it to threat vector
	    if(myMap[i][j] >= 1000)
	    {
		dis = sqrt(pow(myX - j, 2) + pow(myY - i, 2));
		threat.x = j;
		threat.y = i;
		threat.dis = dis; 
		knownThreats.push_back(threat);
	    }
	}
    } 

    //Sort threats based on distance
    sort(knownThreats.begin(), knownThreats.end(), tankSortCriteria);

    if(!knownThreats.empty())
    	return true;

    return false;
}

bool Edelweiss::byCrate()
{
    for(int i = -1; i < 2; i++)
    {
	for(int j = -1; j < 2; j++)
	{
	    if(myY + i >= 0 && myY + i < height && myX + j >= 0 
		&& myX + j < width && myMap[i+myY][j+myX] == -11)
	    {
		return true;
	    }
	}
    }

    return false;
}
