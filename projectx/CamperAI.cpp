#include "CamperAI.h"

CamperAI::CamperAI(void)
{
    // Tells the move function that we can go anywhere
}

CamperAI::~CamperAI(void)
{
}

direction CamperAI::move(MapData map, PositionData status)
{
    int x_pos = status.game_x;
    int y_pos = status.game_y;
    int median = map.height / 2;
    int median2 = map.width / 2;
    bool UP = false, DOWN = false, LEFT = false, RIGHT = false;

    direction retval = STAY;

    if((y_pos == 0 && x_pos == 0) ||
       (y_pos == map.height - 1 && x_pos == 0) ||
       (x_pos == map.width - 1 && y_pos == 0) ||
       (x_pos == map.width - 1 && y_pos == map.height - 1) ||
       (corner == true))
        return retval;
    if(y_pos <= median && y_pos - 1 >= 0 
                && (map.obstacleMap[x_pos + ((y_pos-1)*map.width)] == 0 || 
                    map.obstacleMap[x_pos + ((y_pos-1)*map.width)] == 'T' ||
                    map.obstacleMap[x_pos + ((y_pos-1)*map.width)] == 'B'))
        UP = true;
    if(y_pos > median && (y_pos + 1 <= map.height - 1)
                && (map.obstacleMap[x_pos + ((y_pos+1)*map.width)] == 0 || 
                map.obstacleMap[x_pos + ((y_pos+1)*map.width)] == 'T' ||
                map.obstacleMap[x_pos + ((y_pos+1)*map.width)] == 'B'))
                        
        DOWN = true;
    if(x_pos > median2 && (x_pos + 1 <= map.width - 1)
                && (map.obstacleMap[(x_pos + 1) + (y_pos*map.width)] == 0 || 
                    map.obstacleMap[(x_pos + 1) + (y_pos*map.width)] == 'T' ||
                    map.obstacleMap[(x_pos + 1) + (y_pos*map.width)] == 'B'))
        RIGHT = true;
    if(x_pos <= median2 && (x_pos - 1 >= 0)
                && (map.obstacleMap[(x_pos - 1) + (y_pos*map.width)] == 0 || 
                    map.obstacleMap[(x_pos - 1) + (y_pos*map.width)] == 'T' ||
                    map.obstacleMap[(x_pos - 1) + (y_pos*map.width)] == 'B'))
        LEFT = true;
    
    if (!UP && !DOWN && !RIGHT && !LEFT){
            retval = STAY;
            corner = true;
    }
    else if (UP && LEFT)
            retval = direction::UPLEFT;
    else if (UP && RIGHT)
            retval = direction::UPRIGHT;
    else if (UP)
            retval = direction::UP;
    else if (DOWN && LEFT)
            retval = direction::DOWNLEFT;
    else if (DOWN && RIGHT)
            retval = direction::DOWNRIGHT;
    else if (DOWN)
            retval = direction::DOWN;
    else if (LEFT)
            retval = direction::LEFT;
    else if (RIGHT)
            retval = direction::RIGHT;
    return retval;
}

direction CamperAI::attack(MapData map, PositionData status)
{
    int y_pos = status.game_y;
    int x_pos = status.game_x;
    int hmedian = map.height / 2;
    int wmedian = map.width / 2;
    direction retval;

    if(y_pos >= hmedian && x_pos <= wmedian)
    {
        switch (attacks[turn])
        {
        case 1:
            retval = UPRIGHT;
            break;
        case 2:
            retval = RIGHT;
            break;
        default:
            retval = UP;
            break;
        }
    }
    else if(y_pos < hmedian && x_pos <= wmedian)
    {
        switch (attacks[turn])
        {
        case 1:
            retval = DOWNRIGHT;
            break;
        case 2:
            retval = RIGHT;
            break;
        default:
            retval = DOWN;
            break;
        }
    }
    else if(y_pos >= hmedian && x_pos > wmedian)
    {
        switch (attacks[turn])
        {
        case 1:
            retval = UPLEFT;
            break;
        case 2:
            retval = LEFT;
            break;
        default:
            retval = UP;
            break;
        }
    }
    else if(y_pos < hmedian && x_pos > wmedian)
    {
        switch (attacks[turn])
        {
        case 1:
            retval = DOWNLEFT;
            break;
        case 2:
            retval = LEFT;
            break;
        default:
            retval = DOWN;
            break;
        }
    }

    return retval;
}

attributes CamperAI::setAttribute(int pointsAvailable, attributes baseStats)
{
    attributes retval;

    retval.tankHealth = pointsAvailable;

    return retval;
}

int CamperAI::spendAP(MapData map, PositionData status)
{
    if (maxAp == 0)
        maxAp = status.ap;
//Debugging map output
/*printf("Map:\n");
for (int i = 0; i < map.map.size(); i++){
  printf("%d ", map.map[i]);
  if (i % 30 == 29)
          printf("\n");
}
printf("Obstacle:\n");
for (int i = 0; i < map.obstacleMap.size(); i++){
  printf("%d ", map.obstacleMap[i]);
  if (i % 30 == 29)
          printf("\n");
}
printf("Health:\n");
for (int i = 0; i < map.healthMap.size(); i++){
  printf("%d ", map.healthMap[i]);
  if (i % 30 == 29)
          printf("\n");
}*/
    if (maxAp == status.ap){
        turn = 0;
        for (int i = 0; i < 6; i++)
                attacks[i] = rand() % 3;
    }
    else
        turn++;
    if (move(map, status) != STAY)
        return 1;
    return 2; // Attack
}



#ifdef DYNAMIC
extern "C" //required for runtime linking
{
    Actor * maker()
    {
        return new CamperAI;
    }
}
#endif
