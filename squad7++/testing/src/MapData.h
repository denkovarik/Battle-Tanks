/**
 * @file
 * @brief
 * struct that contains the map and the dimensions of the field.
 */

#ifndef __MAP_DATA_H
#define __MAP_DATA_H
#include <vector>
#include <ostream>
#include <algorithm>
/*******************************************************************//**
 * @author David Donahue
 *
 * @struct MapData
 * @par Description:
 * The map ADT for the platform
 *
 *
 **********************************************************************/
struct MapData
{
    /** row-major-order list the ID's of anything on the tiles on the map (tanks, projectiles, and obstacles) */
    std::vector<int> map;
    /*!< row-major-order map of obstacles, true means that an obstacle is present */
    std::vector<bool> obstacleMap;
    int width;  /*!< The width value of the map */
    int height; /*!< The height value of the map */

    MapData(int w, int h);
    MapData();
};
/* overload << to display to console */
std::ostream & operator<<(std::ostream& os, const MapData& md);

#endif
