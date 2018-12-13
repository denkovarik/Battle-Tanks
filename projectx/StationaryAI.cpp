#include "StationaryAI.h"

StationaryAI::StationaryAI(void)
{
    // Tells the move function that we can go anywhere
}

StationaryAI::~StationaryAI(void)
{
}

direction StationaryAI::move(MapData map, PositionData status)
{
    direction retval;
    retval = STAY;
    return retval;
}

direction StationaryAI::attack(MapData map, PositionData status)
{
    direction retval;
    retval = STAY;
    return retval;
}

attributes StationaryAI::setAttribute(int pointsAvailable, attributes baseStats)
{
    attributes retval;

    retval.tankHealth = pointsAvailable;

    return retval;
}

int StationaryAI::spendAP(MapData map, PositionData status)
{
    return 3;
}



#ifdef DYNAMIC
extern "C" //required for runtime linking
{
    Actor * maker()
    {
        return new StationaryAI;
    }
}
#endif
