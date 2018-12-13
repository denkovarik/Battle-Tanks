#include "Edelweiss.h"
using namespace std;



int Edelweiss::spendAP(MapData map, PositionData status)
{
    int maxVal = 0;
    vector<enemyTank> knownThreats;
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
    cout << "Cur AP: " << curAp << endl;
    cout << "engaged: " << engaged << endl;
    cout << "MyPos: (" << myX << ", " << myY << ")"  << endl;
    cout << "covering fire: " << coveringFire << endl;

    //What tank sees on myMap
    for(int i = 0; i < myMap.size(); i++)
    {
        for(int j = 0; j < myMap[i].size(); j++)
        {
            if(myMap[i][j] > maxVal)
	    {
	        maxVal = myMap[i][j];
	    }
	    cout << myMap[i][j] << " ";
	}
	cout << endl;
    }
    cout << endl;


     if(charge)
     {
	counter++;
	cout << "Charge was set" << endl;
	stop = false;
	for(int i = 0; i < knownThreats.size() && !stop; i++)
	{	    
	    if(myMap[knownThreats[i].y][knownThreats[i].x] == 1010)
	    {
		threat = knownThreats[i];
		stop = true;
	    }
	}

	if(!inRadar(myX, myY, threat) && !tankList.empty() 
		&& !(threat.x == tankList.front().x 
		&& threat.y == tankList.front().y))
	{
	    charge = false;
	    run = true;
	    return 1;
	}
	if(counter % 10 == 0)
	{
	    run = true;
	    return 1;
	}
	else if(curAp == myStats.tankAP && myStats.tankAP == myStats.tankAP)
	{
	    return 1;
	}
	else if(inRange(myX, myY, threat) && canHitAnything(myX, myY))
	{
	    cout << myX << " " << myY << endl;
	    cout << threat.x << " " << threat.y << endl;
	    cout << "charge and shoot" << endl;
	    return 2;
	}
	else if(!inRange(myX, myY, threat) && canHitAnything(myX, myY))
	{
	    cout << "charge and shoot" << endl;
	    return 2;
	}
	else if(myStats.tankAP > 1 && curAp > 1 && canHitAnything(myX, myY))
	{
	    return 1;
	}
	else if(myStats.tankAP <= 1 && !inRange(myX, myY, threat))
	{
	    return 1;
	}
	else if(status.ammo <= 0 && curAp > 1)
	{
	    curAp--;
	    return 4;
	}
	else if(status.ammo <= 0)
	{
	    run = true;
	    return 1;
	}
	else if(!canHitAnything(myX, myY) && status.ammo <= 0)
	{
	    curAp--;
	    return 4;
	}
	else if(canHitAnything(myX, myY) && curAp <= 1)
	{
	    cout << "go out with a bang" << endl;
	    return 2;
	}
     }


    //Fight of Flight
    if(!tankList.empty() && canHitAnything(myX, myY))
    {
	cout << "Hi" << endl;
	if(shouldFight(status, map))
	{
	    cout << "Shoot Tank" << endl;
	    return 2;
	}

	//If no fight then run
	run = true;
	cout << "Flee you fools" << endl;
	cout << "Seriously run!" << endl;
    	return 1;
    }
    else if(!tankList.empty() && !canHitAnything(myX, myY))
    {
	if(curAp > 1 && status.ammo <= 0)
	{
	    //reload
	    curAp--;
	    return 4;
	}
	if(curAp > (myStats.tankAP - exploreAP))
	    return 1;
    }
    else if(tankList.empty() && canHitAnything(myX, myY) && coveringFire > 0)
    {	
	    coveringFire--;
	    return 2;
    }
    else if(curAp > (myStats.tankAP - exploreAP) && tankList.empty() 
	&& !canHitAnything(myX, myY) && status.ammo < myStats.tankAmmo)
    {
	curAp--;
	cout << "reload" << endl;
	return 4;
    }
    else if(curAp > (myStats.tankAP - exploreAP) && tankList.empty() 
	&& !canHitAnything(myX, myY))
    {
	return 1;
    }

    engaged = 0;
    return 1;
}

direction Edelweiss::move(MapData map, PositionData status)
{
    //Return value
    direction ret = STAY;
    vector<spot> vantagePoints; 
    spot startSpot;
    int x, y;
    enemyTank threat;
    vector<enemyTank> knownThreats;
    bool stop;
    knownThreats = findThreats();
    cout << "Move" << endl;

    //If found tank, then attack
    if(!tankList.empty())
    {
	if(run)
	{
	    retreat(x, y, status, map);
    	    ret = findPath(status.game_x, status.game_y, x, y);
	    cout << "flee" << endl;
	}
	else if(!run)
	{
	    cout << "I See Him" << endl;
	    x = tankList.front().x;
	    y = tankList.front().y;
	    threat = tankList.front();

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
	    else if(canHitAnything(myX, myY) && myMap[tankList.front().y][tankList.front().x] >= 1005 && status.health > 1)
	    {
		ret = findPath(myX, myY, tankList.front().x, tankList.front().y);
	
		x = myX;
		y = myY;
		convertDirectionToPosition(x, y, ret);

		if(curAp <= 1 && canHitAnything(x, y))
		{
		    retreat(x, y, status, map);
		    ret = findPath(myX, myY, x, y);
		}
	    }
	    else if(!enroute && findPlan(tankList, vantagePoints))
	    {
		cout << "starting route to (" << enrouteX << ", " << enrouteY << ")" << endl;
		enroute = true;
		enrouteX = vantagePoints.front().x;
		enrouteY = vantagePoints.front().y;
		random_shuffle(vantagePoints.begin(), vantagePoints.end());
	        cout << "vantagePoint: (" << vantagePoints.front().x << " " << vantagePoints.front().y << ")" << endl;
		startSpot.x = status.game_x;
		startSpot.y = status.game_y;
		
	        ret = findPath(status.game_x, status.game_y, vantagePoints.front().x, vantagePoints.front().y);

		x = myX;
		y = myY;
		convertDirectionToPosition(x, y, ret);

		if(curAp <= 1 && canHitAnything(x, y))
		{
		    retreat(x, y, status, map);
		    ret = findPath(myX, myY, x, y); 
		}
	    }
	    else if(enroute)
	    {
		cout << "enroute to (" << enrouteX << ", " << enrouteY << ")" << endl;
	        ret = findPath(status.game_x, status.game_y, enrouteX, enrouteY);

		x = myX;
		y = myY;
		convertDirectionToPosition(x, y, ret);

		if(!canHitAnything(myX, myY) && curAp <= 1)
		{
		    ret = STAY;
		} 
		else if(canHitAnything(myX, myY) && canHitAnything(x, y) && curAp <= 1)
		{
		    retreat(x, y, status, map);
		    ret = findPath(myX, myY, x, y);
		} 
	    }
	    else
	    {
	    	ret = findPath(status.game_x, status.game_y, tankList.front().x, tankList.front().y);
	    }
	}
    }
    //If we remember where an enemy is out of radar
    else if(!knownThreats.empty())
    {
	cout << "made it" << endl;
	//Look for campers
	stop = false;
	for(int i = 0; i < knownThreats.size() && !stop && charge; i++)
	{
	    if(myMap[knownThreats[i].y][knownThreats[i].x] == 1010)
	    {
		threat = knownThreats[i];
		stop = true;
	    }
	}
	//charge if we have a camper
	if(charge)
	{
	    //charge
	    ret = findPath(myX, myY, threat.x, threat.y);
	}
	//Look for a place to shoot him out of radar
	else if(findPlan(knownThreats, vantagePoints))
	{
	    cout << "Vantage" << endl;
	    ret = findPath(status.game_x, status.game_y, vantagePoints.front().x, 
		vantagePoints.front().y);
	}
	//else to move towards him
	else
	{
	    ret = findPath(status.game_x, status.game_y, knownThreats.front().x, 
		knownThreats.front().y);
	}

    	
    }
    //If no threat exits, then explore map
    else if(curAp > (myStats.tankAP - exploreAP) && explore(x, y, status))
    {
	cout << "Explore (" << x << ", " << y << ")" << endl;
    	ret = findPath(status.game_x, status.game_y, x, y);
    }
    else
    {
	ret = STAY;
    }

    //Make sure the direction of movement is not in way of bullet
    x = myX;
    y = myY;

    convertDirectionToPosition(x, y, ret);
	
    if(shot[ret] || blocked(x, y, true))
    {
	cout << "That seems like a bad idea" << endl;
	x = myX;
	y = myY;
	retreat(x, y, status, map);
    	ret = findPath(status.game_x, status.game_y, x, y);
    }

    curAp--;
    return ret;
}

direction Edelweiss::attack(MapData map, PositionData status)
{
    cout << "Fire" << endl;
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
    x = myX;
    y = myY;
    convertDirectionToPosition(x, y, ret);
    cout << x << " " << y << endl;

    if(x >= 0 && x < width && y >= 0 && y < height)
    {
	//Our tank bullets will be marked -3 and -2 on myMap
	//Enemy bullets will be marked -4
	cout << "shot marked" << endl;
	myMap[y][x] = -4;
    }
     
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
   
    //Set stats
    myStats = baseStats; 

    curAp = myStats.tankAP;

    //Reduced AP points for when we see no tanks
    exploreAP = myStats.tankAP * (2.0 / 3);

    if(exploreAP < 1)
    {
	exploreAP = 1;
    }

    tankAttributes.tankDamage += pointsAvailable;

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

