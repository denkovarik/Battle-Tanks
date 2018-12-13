#ifndef __EDELWEISS__H
#define __EDELWEISS__H

#include "src/Actor.h"
#include "src/direction.h"
#include <cmath>
#include <bits/stdc++.h>
#include <ctime>
#include <cstdlib>


//This is a struct to represent enemy tanks found on the map
struct enemyTank
{
    int x;
    int y;
    double dis;
};

//Struct used in 'findPath' functio for BFS
struct spot
{
    int x, y;
    bool visited = false;
    int prevx, prevy;		//x and y pos of previous spot for BFS function
    double dis;			//Distance
    int separation = 0;		//Degree of separation from start node for BFS

    bool operator==(spot rhs) const
    {
	if(x == rhs.x && y == rhs.y)
	{
	    return true;
	}

	return false;
    }
};


class Edelweiss : public Actor
{
    void scanMap( MapData map, PositionData status );
    bool explore(int &x, int &y, PositionData status);
    bool retreat(int &x, int &y, PositionData status, MapData map);
    void resetMyMap();
    direction findPath(int startx, int starty, int endx, int endy);
    direction determineDirection(spot start, spot next);
    bool blocked(int x, int y, bool tank = false);
    void convertDirectionToPosition(int &x, int &y, direction where);
    bool canHitAnything(int x, int y);
    bool canHitUp(int x, int y);
    bool canHitDown(int x, int y);
    bool canHitLeft(int x, int y);
    bool canHitRight(int x, int y);
    bool canHitUpLeft(int x, int y);
    bool canHitUpRight(int x, int y);
    bool canHitDownLeft(int x, int y);
    bool canHitDownRight(int x, int y);    
    bool inRadar(int x, int y, enemyTank threat);
    bool inRange(int x, int y, enemyTank threat);
    bool findPlan(std::vector<enemyTank> threats, std::vector<spot> &vantagePoints);
    bool inPos(int x, int y, int enemyX, int enemyY);
    bool shouldFight(PositionData status, MapData map);
    bool danger();
    std::vector<enemyTank> findThreats();

    int coveringFire = 0;			//Indicates how long to shoot
    attributes myStats;				//The Tank attributes
    std::vector<enemyTank> tankList;		//Container of enemy tanks
    std::vector<std::vector<int> > myMap;	//2D vector to represent map
    int exploreAP;				//AP for exploring map 
    int curAp = 0;					//AP points spent for given turn
    int myShots = 0;				//Count of shots fired for reloading
    bool run = false;				//Flag to tell tank to retreat
    int engaged = 0;				//Flag indicates in combat
    bool enroute = false;			//Flag indicating tank is enroute of pos
    int enrouteX;				//Destination xpos
    int enrouteY;				//Destination ypos
    int myX;					//Current ypos
    int myY;					//Current xpos
    int height;					//Map height
    int width;					//Map width
    bool charge = false;
    int counter = 0;

    //If tank shoots then moves in the direction of that shot in the same turn,
      //then the tank will shoot itself. These flags prevent that
    //up=0, upRight=1, right=2, downRight=3, down=4, downLeft=5, left=6, upLeft=7
    bool shot[9] = { false };

    friend class Tester;

public:
#ifndef testing
    virtual int spendAP(MapData map, PositionData status);

    /**
     * calculates a move, and returns the data describing its move
     * @return MoveData the desired move to be made, if it is possible then the gamefield will do the move
     */
    virtual direction move(MapData map, PositionData status);

    /**
     * calculates an attack and returns the data describing its attack
     * @return Attack data the desired attack to be made, if possible the gamefield will do the move
     */
    virtual direction attack(MapData map, PositionData status);

    virtual attributes setAttribute(int pointsAvailable, attributes baseStats);
 
    Edelweiss();
    ~Edelweiss();
#endif
};

    //Helper function for STL sort function to sort enemy tanks based on distance
    bool tankSortCriteria( enemyTank t1, enemyTank t2 );
#endif
