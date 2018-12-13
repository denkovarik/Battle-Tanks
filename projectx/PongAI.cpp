#include "PongAI.h"

PongAI::PongAI(void)
    :LastDir(STAY)
{
    // Tells the move function that we can go anywhere
}

PongAI::~PongAI(void)
{
    // Intentionally Left Empty
}

direction PongAI::move(MapData map, PositionData status)
{
    int y_pos = status.game_y;


    direction retval = DOWN;
    if(y_pos == 0)
        retval = DOWN;
    else if(y_pos == map.height - 1)
        retval = UP;
    else if(LastDir == UP)
        retval = UP;
    else if(LastDir == DOWN)
        retval = DOWN;

    LastDir = retval;

    return retval;

}

direction PongAI::attack(MapData map, PositionData status)
{
    int x_pos = status.game_x;
    int median = map.width / 2;
    direction retval;

    if(x_pos >= median)
        retval = LEFT;
    else
        retval = RIGHT;

    return retval;
}

attributes PongAI::setAttribute(int pointsAvailable, attributes baseStats)
{
    attributes retval;

    retval.tankAP = pointsAvailable;

    return retval;
}

int PongAI::spendAP(MapData map, PositionData status)
{
    if(status.ap == 1)
        return 2; // Attack
    else
        return 1; // Move
}



#ifdef DYNAMIC
extern "C" //required for runtime linking
{
    Actor * maker()
    {
        return new PongAI;
    }
}
#endif
