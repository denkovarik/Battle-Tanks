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
    int prevx, prevy;
};


class Edelweiss : public Actor
{
    void scanMap( MapData map, PositionData status );
    bool explore(int &x, int &y, PositionData status);
    bool retreat(int &x, int &y, PositionData status, MapData map);
    void resetMyMap();
    direction findPath(int startx, int starty, int endx, int endy);
    direction determineDirection(spot start, spot next);
    bool blocked(MapData map, PositionData status, int x, int y, bool tank = false);
    bool canHitAnything(MapData map, PositionData status, int x, int y);
    bool canHitUp(MapData map, PositionData status, int x, int y);
    bool canHitDown(MapData map, PositionData status, int x, int y);
    bool canHitLeft(MapData map, PositionData status, int x, int y);
    bool canHitRight(MapData map, PositionData status, int x, int y);
    bool canHitUpLeft(MapData map, PositionData status, int x, int y);
    bool canHitUpRight(MapData map, PositionData status, int x, int y);
    bool canHitDownLeft(MapData map, PositionData status, int x, int y);
    bool canHitDownRight(MapData map, PositionData status, int x, int y);
    bool shouldFight(PositionData status, MapData map);

    attributes myStats;	
    std::vector<enemyTank> tankList;		//Container of enemy tanks
    std::vector<std::vector<int> > myMap;	//2D vector to represent map
    int exploreAP;				//AP for exploring map 
    int curAp;					//AP points spent for given turn
    int myShots = 0;				//Count of shots fired for reloading
    bool run = false;
    bool engaged = false;

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
