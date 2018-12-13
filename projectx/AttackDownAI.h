#ifndef __CAMPER_AI_H
#define __CAMPER_AI_H

#include "src/Actor.h"
#include "src/direction.h"
#include <cmath>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
/// Description: This AI finds a corner and starts shooting. It's like an 8
//                year old playing call of duty.
///////////////////////////////////////////////////////////////////////////////


class AttackDownAI : public Actor
{
public:

    // The Overwritten Move function.
    virtual direction move(MapData map, PositionData status);

    // The Overwritten attack function.
    virtual direction attack(MapData map, PositionData status);

    // The Overwritten set attribute  function.
    virtual attributes setAttribute(int pointsAvailable, attributes baseStats);

    // The Overwritten spendAP  function.
    virtual int spendAP(MapData map, PositionData status);

    AttackDownAI();
    ~AttackDownAI();

};

#endif
