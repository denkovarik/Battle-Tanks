#include "catch.hpp"
#include "../Edelweiss/Edelweiss.h"
#include "EdelweissTester.h"
#include <iostream>

using namespace std;

//Testing the danger function
TEST_CASE("Testing the destroy function")
{
    Tester testTank;

    CHECK(testTank.testDanger() == true);
}

TEST_CASE("Testing canHitAnything functions")
{
    Tester testTank;
    attributes testAtt;
    testAtt.tankRange = 4;
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

    SECTION("No line of fire and obsticle in way. Expect false")
    {
    	CHECK(testTank.testCanHitAnything(16, 2, testAtt, map) == false);
    }
    SECTION("No line of fire. Expect false")
    {
    	CHECK(testTank.testCanHitAnything(12, 0, testAtt, map) == false);
    }
    SECTION("Blocked by obsticle. Expect false")
    {
    	CHECK(testTank.testCanHitAnything(14, 2, testAtt, map) == false);
    }
    SECTION("In line of fire but out of range. Expect true.")
    {
    	CHECK(testTank.testCanHitAnything(0, 8, testAtt, map) == false);
    }
}

TEST_CASE("Testing makePlan() function")
{
    Tester testTank1;
    enemyTank threat;
    vector<enemyTank> knownThreats;
    attributes testAtt;
    vector<spot> vantagePoints;
    vector< vector<int> > map = { {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,1000,   0,   0,   0,   0,   0},
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, -10,   0, -10, -10, -10, -10, -10}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, -10,   0,   0,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, -10, -10, -10, -10, -10, -10, -10}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,1000,   0,   0,   0,   0}, 
				  {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0} };

    threat.x = 13;
    threat.y = 0;
    knownThreats.push_back(threat);
    threat.x = 14;
    threat.y = 8;
    knownThreats.push_back(threat);

    SECTION("Sees a tank and can hit it")
    {
 	testAtt.tankRange = 4;
	testAtt.tankRadar = 3;
    	CHECK(testTank1.testMakePlan(knownThreats, vantagePoints, testAtt, 11, 8, map) == false);
    }
    SECTION("Sees a tank and but can't hit it")
    {
 	testAtt.tankRange = 4;
	testAtt.tankRadar = 3;

    	CHECK(testTank1.testMakePlan(knownThreats, vantagePoints, testAtt, 11, 7, map) == true);

	CHECK(vantagePoints.size() == 1);

    }
    SECTION("myX == 11, myY == 7, threat.x == 14, threat.y == 8 Expect true")
    {
 	testAtt.tankRange = 4;
	testAtt.tankRadar = 5;
	threat.x = 14;
	threat.y = 8;

    	CHECK(testTank1.testMakePlan(knownThreats, vantagePoints, testAtt, 11, 7, map) == true);

	CHECK(vantagePoints.size() == 4);

    }
    SECTION("myX == 19, myY == 2, threat.x == 12, threat.y == 0 Expect true")
    {
 	testAtt.tankRange = 4;
	testAtt.tankRadar = 5;
	threat.x = 12;
	threat.y = 0;

    	CHECK(testTank1.testMakePlan(knownThreats, vantagePoints, testAtt, 19, 2, map) == true);

	CHECK(vantagePoints.size() == 2);

    }
}

TEST_CASE("Testing convertDirectionToPosition() function")
{
    int x, y;
    Tester testTank1;

    SECTION("x = 2, y = 2, direction UP. Expect x = 2, y = 1")
    {
	x = 2;
	y = 2;
        testTank1.testConvertDirectionToPosition(x, y, UP);
        CHECK(x == 2);
 	CHECK(y == 1);
    }
    SECTION("x = 2, y = 2, direction DOWN. Expect x = 2, y = 3")
    {
	x = 2;
	y = 2;
        testTank1.testConvertDirectionToPosition(x, y, DOWN);
        CHECK(x == 2);
 	CHECK(y == 3);
    }
    SECTION("x = 2, y = 2, direction RIGHT. Expect x = 3, y = 2")
    {
	x = 2;
	y = 2;
        testTank1.testConvertDirectionToPosition(x, y, RIGHT);
        CHECK(x == 3);
 	CHECK(y == 2);
    }
    SECTION("x = 2, y = 2, direction LEFT. Expect x = 1, y = 2")
    {
	x = 2;
	y = 2;
        testTank1.testConvertDirectionToPosition(x, y, LEFT);
        CHECK(x == 1);
 	CHECK(y == 2);
    }
    SECTION("x = 2, y = 2, direction UPRIGHT. Expect x = 3, y = 1")
    {
	x = 2;
	y = 2;
        testTank1.testConvertDirectionToPosition(x, y, UPRIGHT);
        CHECK(x == 3);
 	CHECK(y == 1);
    }
    SECTION("x = 2, y = 2, direction UPLEFT. Expect x = 1, y = 1")
    {
	x = 2;
	y = 2;
        testTank1.testConvertDirectionToPosition(x, y, UPLEFT);
        CHECK(x == 1);
 	CHECK(y == 1);
    }
    SECTION("x = 2, y = 2, direction DOWNRIGHT. Expect x = 3, y = 3")
    {
	x = 2;
	y = 2;
        testTank1.testConvertDirectionToPosition(x, y, DOWNRIGHT);
        CHECK(x == 3);
 	CHECK(y == 3);
    }
    SECTION("x = 2, y = 2, direction DOWNLEFT. Expect x = 1, y = 3")
    {
	x = 2;
	y = 2;
        testTank1.testConvertDirectionToPosition(x, y, DOWNLEFT);
        CHECK(x == 1);
 	CHECK(y == 3);
    }
}
