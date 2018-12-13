#ifndef __SIMPLE_AI_H
#define __SIMPLE_AI_H

#include "src/Actor.h"
#include "src/direction.h"
#include "src/MapData.h"
#include <cmath>

class SimpleAI : public Actor
{
    int calcDist(int x1, int y1, int x2, int y2);

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
    /**
     * calculates how many moves it will take to reach a given coordinate
     * @param[in] x1 - starting point x coordinate
     * @param[in] y1 - starting point y coordinate
     * @param[in] x2 - end point x coordinate
     * @param[in] y2 - end point y coordinate
     * @return distance to the target
     */
   
    attributes myStats; 
  
    bool spend = false;
    MapData * tempMap;

    SimpleAI();
    ~SimpleAI();
#endif
};

#endif
