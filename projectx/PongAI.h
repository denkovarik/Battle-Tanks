#ifndef __PONG_AI_H
#define __PONG_AI_H

#include "src/Actor.h"
#include "src/direction.h"
#include <cmath>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
/// Description: This AI acts like a paddle from pong. It will move vertically
//               and shoot each turn
///////////////////////////////////////////////////////////////////////////////


class PongAI : public Actor
{
    // The direction moved from last turn
    direction LastDir;

public:

    // The Overwritten Move function.
    virtual direction move(MapData map, PositionData status);

    // The Overwritten attack function.
    virtual direction attack(MapData map, PositionData status);

    // The Overwritten set attribute  function.
    virtual attributes setAttribute(int pointsAvailable, attributes baseStats);

    // The Overwritten spendAP  function.
    virtual int spendAP(MapData map, PositionData status);

    PongAI();
    ~PongAI();
};

#endif
