#include <bits/stdc++.h>

using namespace std:

//This is a struct to represent enemy tanks found on the map
struct enemyTank
{
    int x;
    int y;
    double dis;
};

//Struct used in 'findPath' functio for BFS
struct spot
{
    int x, y;
    bool visited = false;
    int prevx, prevy;
    double dis;

    bool operator==(spot rhs) const
    {
	if(x == rhs.x && y == rhs.y)
	{
	    return true;
	}

	return false;
    }
};

int myX, myY;
int range;
int radar;



bool makePlan(enemyTank threat, std::vector<spot> &vantagePoints);

int main()
{
    enemyTank threat;
    vector<spot> vantagePoints;
    vector< vector<int> > map = { {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, -10, -10, -10, -10, -10}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, -10, -10, -10, -10, -10}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,1000,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0} };

    myX = 11;
    myY = 8;
    radar = 3;
    range = 4;
    threat.x = 14;
    threat.y = 8;
    
    makePlan(threat, vantagePoints);

}

bool makePlan(enemyTank threat, std::vector<spot> &vantagePoints)
{

}
