//
// Created by jlee on 9/8/18.
//

/** @file */

#ifndef SLACKERS_PLATFORM_POSITIONDATA_H
#define SLACKERS_PLATFORM_POSITIONDATA_H

#include "direction.h"
#include "string"

/***************************************************************************//**
* @struct PositionData
* @author Jacob Lee
* @par Description
* The position data for each actor
*******************************************************************************/
struct PositionData
{
  std::string name; /*!<The name */
  int id; /*!<The id number */
  int health; /*!<The health */
  int game_x; /*!<The x coordinate */
  int game_y; /*!<The y coordinate */
  int ap; /*!< remaining action points*/
  int ammo; /*!< Remaining Ammo chambered */
  PositionData() {}
  PositionData(int i, int h, int x, int y, int a = 2, int am = 0) : id(i), health(h), game_x(x), game_y(y), ap(a), ammo(am) {}
};
#endif //SLACKERS_PLATFORM_POSITIONDATA_H
