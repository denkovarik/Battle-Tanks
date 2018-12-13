#ifndef __JUNKO__H
#define __JUNKO__H

#include "src/Actor.h"
#include "src/direction.h"
#include <cmath>
#include <bits/stdc++.h>
#include <ctime>
#include <cstdlib>


struct dirAndNum
{
    int num;
    direction dir;
};



class Junko : public Actor
{
    bool blocked(MapData map, PositionData status, int x, int y);
    
    void updateGlobalMap(MapData map, PositionData status);
    int findLocalSum(int x, int y, MapData map);
    void getNewCoordinates(PositionData status, direction newDirection, int &x, int &y);

    bool canHitAnything(MapData map, int x, int y);
    bool canHitUp(MapData map, int x, int y);
    bool canHitDown(MapData map, int x, int y);
    bool canHitLeft(MapData map, int x, int y);
    bool canHitRight(MapData map, int x, int y);
    bool canHitUpLeft(MapData map, int x, int y);
    bool canHitUpRight(MapData map, int x, int y);
    bool canHitDownLeft(MapData map, int x, int y);
    bool canHitDownRight(MapData map, int x, int y);

    attributes myStats;
    std::vector<int> myMap ;
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

    virtual attributes setAttribute(int pointsAvailable, attributes baseStats);

    virtual int spendAP(MapData map, PositionData status);
   

    Junko();
    ~Junko();
#endif
};

    bool sortNumLowestFirst(dirAndNum d1, dirAndNum d2);
#endif
