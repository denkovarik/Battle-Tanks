#include "AttackDownAI.h"

AttackDownAI::AttackDownAI(void)
{
    // Tells the move function that we can go anywhere
}

AttackDownAI::~AttackDownAI(void)
{
}

direction AttackDownAI::move(MapData map, PositionData status)
{
    direction retval = STAY;
    return retval;
}

direction AttackDownAI::attack(MapData map, PositionData status)
{
    direction retval;
    retval = DOWN;
    return retval;
}

attributes AttackDownAI::setAttribute(int pointsAvailable, attributes baseStats)
{
    attributes retval;

    retval.tankAP = pointsAvailable;

    return retval;
}

int AttackDownAI::spendAP(MapData map, PositionData status)
{
    return 2; // Attack
}

#ifdef DYNAMIC
extern "C" //required for runtime linking
{
    Actor * maker()
    {
        return new AttackDownAI;
    }
}
#endif
