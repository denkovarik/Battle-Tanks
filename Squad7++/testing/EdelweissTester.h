#ifndef _TESTER_
#define _TESTER_
#include "../Edelweiss/Edelweiss.h"
#include "catch.hpp"
#include <bits/stdc++.h>

class Tester
{
public:
    Tester();
    ~Tester();

    bool testDanger();
    bool testCanHitAnything(int x, int y, attributes newAtt, std::vector< std::vector<int> > map);
    bool testMakePlan(std::vector<enemyTank> threats, std::vector<spot> &vantagePoints, 
	attributes newStats, int x, int y, std::vector< std::vector<int> > map);
    void testConvertDirectionToPosition(int &x, int &y, direction where);

    friend class Edelweiss;
};

#endif
