#include "EdelweissTester.h"

using namespace std;

Tester::Tester() {}
Tester::~Tester() {}

bool Tester::testDanger()
{
    Edelweiss testTank;

    testTank.myMap.resize(10);

    for(int i = 0; i < 10; i++)
    {
	testTank.myMap[i].resize(10);
    }

    testTank.myMap[1][1] = 1000;

    return testTank.danger();
}

bool Tester::testCanHitAnything(int x, int y, attributes newAtt, std::vector< std::vector<int> > map)
{
    Edelweiss testTank;
    testTank.myStats = newAtt;
    testTank.myMap = map;

    return testTank.canHitAnything(x, y);
}

bool Tester::testMakePlan(std::vector<enemyTank> threats, std::vector<spot> &vantagePoints, 
	attributes newStats, int x, int y, std::vector< std::vector<int> > map)
{
    bool flag;
    Edelweiss testTank;
    testTank.myStats = newStats;
    testTank.myMap = map;
    testTank.myX = x;
    testTank.myY = y;
    testTank.height = map.size();
    
    testTank.width = 0;

    if(map.size() > 0)
	testTank.width = map[0].size();

    flag = testTank.findPlan(threats, vantagePoints);

    cout << "vantagePoints" << endl;
    cout << "-----------------------------------------------------------------------" << endl;

    for(int i = 0; i < vantagePoints.size(); i++)
    {
	cout << vantagePoints[i].x << " " << vantagePoints[i].y << endl;
    }

    cout << endl;

    return flag;
}

void Tester::testConvertDirectionToPosition(int &x, int &y, direction where)
{
    Edelweiss testTank;

    return testTank.convertDirectionToPosition(x, y, where);
}
