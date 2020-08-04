#include "Edelweiss.h"
using namespace std;



/*
   spendAP()
Authors: Dennis Kovarik, Samuel Backes, Adesh Naik
Parameters
    MapData map - struct given by platform holding information about the map.
    PositionData status - struct given by platform holding information about
			the tank's position
Return Value
    returns an int representing the next AP point the tank will spend.

This function is the first function called by the platform. It based on the 
data held in the MapData and PositionData structs, it will decide on the next
AP point to spend. To move on the map, it will return 1, to shoot on the map
it will return 2, to reload, it will return 4. Otherwise to do nothing, it will
return 3;
*/
int Edelweiss::spendAP(MapData map, PositionData status)
{
    int maxVal = 0;
    enemyTank threat;
    bool stop;

    //Allocate space for myMap
    if(myMap.size() == 0)
    {
	myMap.resize(map.height);

	for(int i = 0; i < map.height; i++)
	{
	    myMap[i].resize(map.width, 1);
	}	

    	//Set height and width of map
    	height = map.height;
    	width = map.width;
    }

    //Set our current position
    myX = status.game_x;
    myY = status.game_y;

    //If were enroute to destination but we have arrived
    if(enroute && enrouteX == myX && enrouteY == myY)
    {
	//reset the enroute flags
 	enroute = false;
    }
    

    //If at end of turn
    if(curAp <= 0)
    { 
	//Reset curAp to total AP
	curAp = myStats.tankAP;
	//Mark that no longer engaged
	//Reset all the shot flags so tank will be able to move in that direction
	for(int i = 0; i < 8; i++)
	{
	    shot[i] = false;
	}
    }

    //clear tankList and reset flags
    tankList.clear();
    run = false;
    scanMap(map, status);
    findThreats();

     //Make sure there are known threats before charging
     if(knownThreats.empty())
	charge = false;

     //This block handles when the tank decides to charge another tank
     if(charge)
     {
	counter++;

	//Find known threats that tank remembers stored in myMap
	stop = false;
	for(int i = 0; i < knownThreats.size() && !stop; i++)
	{
	    if(myMap[knownThreats[i].y][knownThreats[i].x] >= 1005)
	    {
		threat = knownThreats[i];
		stop = true;
	    }
	}

	//Shoot the tank if in range and in radar
	if(!byCrate() && inRadar(myX, myY, threat) && inRange(myX, myY, threat) 
	  && canHitAnything(myX, myY)/* && myStats.tankAP > 1*/)
	{
	    return 2;
	}
	//Prevents tank from getting stuck in a loop
	else if(counter % 10 == 0)
	{
	    run = true;
	    return 1;
	}
	//Move closer if more than 1 AP point left
	else if(curAp > 1 && myStats.tankAP > 1)
	{
	    return 1;
	}
	//Move closer if more than 1 AP point left
	else if(myStats.tankAP <= 1)
	{
	    return 1;
	}
	//Shoot at tank on last AP point
	else if(!byCrate() && !inRange(myX, myY, threat) && canHitAnything(myX, myY) 
	  && curAp <= 1)
	{
	    return 2;
	}
	//Reload
	else if(!canHitAnything(myX, myY) && status.ammo <= 0)
	{
	    curAp--;
	    return 4;
	}
	//Retreat if out of ammo and on last AP
	else if(canHitAnything(myX, myY) && curAp <= 1 && myStats.tankAP > 1 
		&& status.ammo <= 0)
	{
	    run = true;
	    return 1;
	}
	//Shoot on last AP point
	else if(!byCrate() && canHitAnything(myX, myY) && curAp <= 1)
	{
	    return 2;
	}
     }


    //This block handles cases when tanks are in sight in line of fire
    if(!tankList.empty() && canHitAnything(myX, myY))
    {
	//Fight or Flight
	if(!byCrate() && shouldFight(status, map))
	{
	    return 2;
	}

	//If no fight then run
	run = true;
    	return 1;
    }
    //Handles when enemy tanks are in sight but not in line of fire
    else if(!tankList.empty() && !canHitAnything(myX, myY))
    {
	//Reload
	if(curAp > 1 && status.ammo <= 0)
	{
	    //reload
	    curAp--;
	    return 4;
	}
	//Reload
	else if(myStats.tankAP <= 1 && status.ammo <= 0)
	{
	    //reload
	    curAp--;
	    return 4;
	}
	//Retreat when out of ammo on last AP
	else if(curAp <= 1 && status.ammo <= 0)
	{
	    run = true;
	    return 1;
	}
	//Move if enought AP points available
	if(curAp > (myStats.tankAP - exploreAP))
	{
	    return 1;
	}
    }
    //Handles when no tanks in sight but have known tank locations on myMap
    else if(tankList.empty() && curAp > (myStats.tankAP - exploreAP) 
	&& canHitAnything(myX, myY) && coveringFire > 0 && !byCrate())
    {	
	    coveringFire--;
	    return 2;
    }
    //Handles no tanks in sight, not in line of fire of enemies, and low on ammo
    else if(curAp > (myStats.tankAP - exploreAP) && tankList.empty() 
	&& !canHitAnything(myX, myY) && status.ammo < myStats.tankAmmo)
    {
	curAp--;
	return 4;
    }
    //No known tanks and plenty of AP points to spend
    else if(curAp > (myStats.tankAP - exploreAP) && tankList.empty() 
	&& !canHitAnything(myX, myY))
    {
	return 1;
    }
    //Shoot if can Hit anything
    else if(canHitAnything(myX, myY) && !byCrate())
    {	
	    return 2;
    }

    //Default to moving 
    engaged = 0;
    return 1;
}

/*
   move()
Authors: Dennis Kovarik, Samuel Backes, Adesh Naik
Parameters
    MapData map - struct given by platform holding information about the map.
    PositionData status - struct given by platform holding information about
			the tank's position
Return Value
    returns an enum of type 'direction' indicating the direction of the tank's 
    next move.

This function is called by the platform when our tank decides to move on the 
map. Based on the data stored in the MapData struct, PositionData structs, and 
the tank's representation of the map stored it the myMap 2D vector, it will
decide on the tank's next move on the map. It will return an enum of indicating
the tank's next move.
*/
direction Edelweiss::move(MapData map, PositionData status)
{
    //Return value
    direction ret = STAY;
    vector<spot> vantagePoints; 
    spot startSpot;
    int x, y;
    enemyTank threat;
    bool stop;
    vector<direction> randomDirection = {UP, UPRIGHT, RIGHT, DOWNRIGHT, DOWN, 
					DOWNLEFT, LEFT, UPLEFT};


    //If Handles when tanks are in sight
    if(!tankList.empty())
    {
	//Case for when tank decides to retreat
	if(run)
	{
	    retreat(x, y, status, map);
    	    ret = findPath(status.game_x, status.game_y, x, y);
	}
	//Case for when tank decides to stand and fight
	else if(!run)
	{
	    x = tankList.front().x;
	    y = tankList.front().y;
	    threat = tankList.front();
	    
	    //Find campers that are in sight
	    stop = false;
	    for(int i = 0; i < tankList.size() && !stop; i++)
	    {
	    	if(myMap[tankList[i].y][tankList[i].x] == 1010)
	    	{
		    threat = tankList[i];
		    stop = true;
	    	}
	    }

	    //charge if found camper
	    if(charge)
	    {
		ret = findPath(myX, myY, threat.x, threat.y);
	    }
	    //This block will handle tank moving towards the closest threat in sight
	    else if(canHitAnything(myX, myY) && myMap[tankList.front().y][tankList.front().x] >= 1005 
		&& status.health > 1)
	    {
		ret = findPath(myX, myY, tankList.front().x, tankList.front().y);
	
		x = myX;
		y = myY;
		convertDirectionToPosition(x, y, ret);

		//Retreat if low on AP and in line of fire
		if(curAp <= 1 && canHitAnything(x, y))
		{
		    retreat(x, y, status, map);
		    ret = findPath(myX, myY, x, y);
		}
	    }
	    //This block will allow tank to decide on where to best move in order to a
	     //ttack an enemy tank
	    else if(!enroute && findPlan(tankList, vantagePoints))
	    {
		enroute = true;
		enrouteX = vantagePoints.front().x;
		enrouteY = vantagePoints.front().y;
		random_shuffle(vantagePoints.begin(), vantagePoints.end());
		startSpot.x = status.game_x;
		startSpot.y = status.game_y;
		
	        ret = findPath(status.game_x, status.game_y, vantagePoints.front().x, 
			vantagePoints.front().y);

		x = myX;
		y = myY;
		convertDirectionToPosition(x, y, ret);

		//Retreat if low on AP and in line of fire
		if(curAp <= 1 && canHitAnything(x, y))
		{
		    retreat(x, y, status, map);
		    ret = findPath(myX, myY, x, y); 
		}
	    }
	    //If a destination has already been determined, then this block will allow
	     //tank to continue moving to that destination
	    else if(enroute)
	    {
	        ret = findPath(status.game_x, status.game_y, enrouteX, enrouteY);

		x = myX;
		y = myY;
		convertDirectionToPosition(x, y, ret);

		//If low on AP and not in line of Fire, then don't move anymore
		if(!canHitAnything(myX, myY) && curAp <= 1)
		{
		    ret = STAY;
		} 
		//Retreat if low on AP and in line of fire
		else if(canHitAnything(myX, myY) && canHitAnything(x, y) && curAp <= 1)
		{
		    retreat(x, y, status, map);
		    ret = findPath(myX, myY, x, y);
		} 
	    }
	    //If no decision has been made, just move towards the tank
	    else
	    {
	    	ret = findPath(status.game_x, status.game_y, tankList.front().x, tankList.front().y);
	    }
	}
    }
    //This Block handles if tank does not see enemies but has known enemy locations
    else if(!knownThreats.empty())
    {
	//Finds campers
	stop = false;
	for(int i = 0; i < knownThreats.size() && !stop; i++)
	{
	    if(myMap[knownThreats[i].y][knownThreats[i].x] == 1005)
	    {
		threat = knownThreats[i];
		stop = true;
	    }
	}
	
	//Charge enemy camper if found
	if(charge)
	{
	    ret = findPath(myX, myY, threat.x, threat.y);
	}
	//This block allows tank to move into position to attack an enemy tank
	else if(!enroute && findPlan(knownThreats, vantagePoints))
	{
	    enroute = true;
	    enrouteX = vantagePoints.front().x;
	    enrouteY = vantagePoints.front().y;
	    random_shuffle(vantagePoints.begin(), vantagePoints.end());
	    startSpot.x = status.game_x;
	    startSpot.y = status.game_y;
		
	    ret = findPath(status.game_x, status.game_y, vantagePoints.front().x, 
		vantagePoints.front().y);

	    x = myX;
	    y = myY;
	    convertDirectionToPosition(x, y, ret);

	    //Retreat if low on AP
	    if(curAp <= 1 && canHitAnything(x, y))
	    {
	        retreat(x, y, status, map);
	        ret = findPath(myX, myY, x, y); 
	    }
	}
	//This block handles if tank is currently enroute to a position
	else if(enroute)
	{
	    ret = findPath(status.game_x, status.game_y, enrouteX, enrouteY);

	    x = myX;
	    y = myY;
	    convertDirectionToPosition(x, y, ret);

	    //If low on AP and not in line of fire of enemies, then stay
	    if(!canHitAnything(myX, myY) && curAp <= 1)
	    {
	        ret = STAY;
	    } 
	    //If low on AP and in line of fire of enemy tank, then retreat
	    else if(canHitAnything(myX, myY) && canHitAnything(x, y) && curAp <= 1)
	    {
	        retreat(x, y, status, map);
	        ret = findPath(myX, myY, x, y);
	    } 
	}
    }
    //If no threat exits, then explore map
    else if(curAp > (myStats.tankAP - exploreAP) && explore(x, y, status))
    {
	ret = findPath(status.game_x, status.game_y, x, y);
    }
    else
    {
	ret = STAY;
    }

    //Prevents tank from being indecisive
    if(curAp > 1 && ret == STAY)
    {
	random_shuffle(randomDirection.begin(), randomDirection.end());
	ret = randomDirection.front();	
    }

    //Make sure the direction of movement is not in way of bullet
    x = myX;
    y = myY;

    convertDirectionToPosition(x, y, ret);
	
    if(shot[ret] || blocked(x, y, true))
    {
	x = myX;
	y = myY;
	retreat(x, y, status, map);
    	ret = findPath(status.game_x, status.game_y, x, y);
    }

    curAp--;
    return ret;
}

/*
   attack()
Authors: Samuel Backes
Parameters
    MapData map - struct given by platform holding information about the map.
    PositionData status - struct given by platform holding information about
			the tank's position
Return Value
    returns an enum of type 'direction' representing the direction the tank 
    will shoot

This function is called by the platform when the tank decides to shoot. It will
call the canHitAnything function to see if it can hit anything in any direction.
It will then return the first direction in which it can hit something.
*/
direction Edelweiss::attack(MapData map, PositionData status)
{
    direction ret;
    int x, y;

    //See if we can hit something in any direction
    if(canHitUpLeft(status.game_x, status.game_y))
	ret = UPLEFT;
    else if(canHitUp(status.game_x, status.game_y))
	ret = UP;
    else if(canHitUpRight(status.game_x, status.game_y))
	ret = UPRIGHT;
    else if(canHitLeft(status.game_x, status.game_y))
	ret = LEFT;
    else if(canHitRight(status.game_x, status.game_y))
	ret = RIGHT;
    else if(canHitDownLeft(status.game_x, status.game_y))
	ret = DOWNLEFT;
    else if(canHitDown(status.game_x, status.game_y))
	ret = DOWN;
    else if(canHitDownRight(status.game_x, status.game_y))
	ret = DOWNRIGHT;
    else
	ret = STAY;

    shot[ret] = true;

    //Count shots
    if(ret != STAY)
	myShots++;

    //Mark 1 position ahead of direction of fire so tank doesn't run into it
    convertDirectionToPosition(x, y, ret);

    if(x >= 0 && x < width && y >= 0 && y < height)
    {
	//Our tank bullets will be marked -3 and -2 on myMap
	//Enemy bullets will be marked -4
	myMap[y][x] = -4;
    }
     
    curAp--;
    return ret;
}

/*
   setAttribute()
Authors: Samuel Backes
Parameters
    int pointsAvailable - Int representing the number of points available to 
			  spend on attributes.
    attributes baseStats - Struct given by platform representing the tank's 
			   base stats given at beginning of game such as health
			   and AP points per turn
Return Value
    returns attrubute struct representing the tank's attrubutes for the game.

This function is called at the begining of the game by the platform in order to
let the tank decide on what it wants to spend its attrubute points on. This 
function will add points to an attrubute struck based on the number of points
it is given. It will also determine the number of AP points per turn to spend
on exploring the map based on the number of AP points given.
*/
attributes Edelweiss::setAttribute(int pointsAvailable, attributes baseStats)
{
    attributes tankAttributes; 
   
    //Set stats
    myStats = baseStats; 

    curAp = myStats.tankAP;

    //Reduced AP points for when we see no tanks
    exploreAP = myStats.tankAP * (3.0 / 4);

    if(exploreAP < 1)
    {
	exploreAP = 1;
    }

    tankAttributes.tankDamage += pointsAvailable;

    return tankAttributes;
}

/*
   Edelweiss constructor
Authors: Samuel Backes
Parameters
    none
Return Value
    none
This is the Edelweiss constructor that constructs the Edelweiss tank object.
*/
Edelweiss::Edelweiss() 
{}

/*
   Edelweiss destructor
Authors: Samuel Backes
Parameters
    none
Return Value
    none
This is the Edelweiss destructor that destroys and frees up the memory that was
allocated by the constructor and the program during runtime. It will clear the 
tankList and myMap vectors that were allocated during runtime.
*/
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

