#ifndef __EDELWEISS__H
#define __EDELWEISS__H

#include "src/Actor.h"
#include "src/direction.h"
#include <cmath>
#include <bits/stdc++.h>



//This is a struct to represent enemy tanks found on the map
struct enemyTank
{
    int x;
    int y;
    double dis;
};



class Edelweiss : public Actor
{
    bool blocked(MapData map, PositionData status, direction where);
    direction moveDown(MapData map, PositionData status);
    direction moveRight(MapData map, PositionData status);
    direction moveUp(MapData map, PositionData status);
    direction moveLeft(MapData map, PositionData status);
    bool canHitAnything(MapData map, int x, int y);
    bool canHitUp(MapData map, int x, int y);
    bool canHitDown(MapData map, int x, int y);
    bool canHitLeft(MapData map, int x, int y);
    bool canHitRight(MapData map, int x, int y);
    bool canHitUpLeft(MapData map, int x, int y);
    bool canHitUpRight(MapData map, int x, int y);
    bool canHitDownLeft(MapData map, int x, int y);
    bool canHitDownRight(MapData map, int x, int y);
    std::vector<enemyTank> tankList;					//Container of enemy tanks

public:
#ifndef testing

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

    virtual attributes setAttribute(int pointsAvailable);

    virtual int spendAP(MapData map, PositionData status);
    /**
     * calculates how many moves it will take to reach a given coordinate
     * @param[in] x1 - starting point x coordinate
     * @param[in] y1 - starting point y coordinate
     * @param[in] x2 - end point x coordinate
     * @param[in] y2 - end point y coordinate
     * @return distance to the target
     */
   
     //Find enemy tanks on the map and store them in tankList vector
     void findEnemyTanks( MapData map, PositionData status );

    Edelweiss();
    ~Edelweiss();
#endif
};

    //Helper function for STL sort function to sort enemy tanks based on distance
    bool tankSortCriteria( enemyTank t1, enemyTank t2 );
#endif
