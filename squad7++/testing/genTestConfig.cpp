#include <bits/stdc++.h>

using namespace std;

void genRandomMap(vector< vector<char> > &map);
void showMap(vector< vector<char> > map);

int main()
{
    srand(time(NULL));
    vector< vector<char> > map;
    int ex = 0;
    int ey = 0;
    int tx = 0;
    int ty = 0;
    ofstream fout;
    string enemy;
    int num;
    int randNum = rand() % 11;

    fout.open("testConfigs/testConfig.txt");

    if(!fout.is_open())
    {
	cout << "Could not open file writer" << endl;
 	return 1;
    }

    while(ex == tx && ey == ty)
    {
	ex = rand() % 15;
	tx = rand() % 15;
	ey = rand() % 15;
	ty = rand() % 15;
    }

    num = rand() % 2;

    if(num == 0)
	enemy = "Target";
    else if(num == 1)
	enemy = "Mouse";
    else if(num == 2)
	enemy = "Turret";

    genRandomMap(map);

    //showMap(map);

    fout << "WIDTH 15" << endl;
    fout << "HEIGHT 15" << endl;
    fout << "MAP" << endl;

    for(int i = 0; i < map.size(); i++)
    {
	for(int j = 0; j < map[i].size(); j++)
	{
	    fout << map[i][j];
	}
	fout << endl;
    }

    fout << endl;
    fout << "AI " << enemy << " " << tx << " " << ty << " images/Purple/tankP_U.png ";
    fout << "images/Purple/tankP_R.png images/Purple/tankP_D.png images/Purple";
    fout << "/tankP_L.png images/Purple/bulletP.png" << endl;
    fout << "AI Edelweiss " << ex << " " << ey << " images/SkyBlue/tankSB_U.png images";
    fout << "/SkyBlue/tankSB_R.png images/SkyBlue/tankSB_D.png images/SkyBlue";
    fout << "/tankSB_L.png images/SkyBlue/bulletSB.png" << endl;
    fout << endl;

    fout << "#FIELDIMAGE <MAP FILE>" << endl;
    fout << "FIELDIMAGE images/green.png" << endl;
    fout << endl;
    fout << "#OBSTACLE_IMAGE <IMAGE1> [<IMAGE2>]" << endl;
    fout << "OBSTACLE_IMAGE images/tree/tree.png images/rock/rock.png" << endl;
    fout << "TREE_IMAGE images/tree/tree.png images/tree/treeb.png images/tree/treec.png images/tree/treed.png" << endl;
    fout << "BUSH_IMAGE images/bush/bush1.png images/bush/bush2.png images/bush/bush3.png images/bush/bush4.png" << endl;
    fout << "ROCK_IMAGE images/rock/rock.png images/rock/rockb.png images/rock/rockc.png" << endl;
    fout << "WATER_IMAGE images/Water/waterTex.png images/Water/waterTexNS.png images/Water/waterTexES.png images/Water/waterTexSS.png images/Water/waterTexWS.png images/Water/waterTexNES.png images/Water/waterTexSES.png images/Water/waterTexSWS.png images/Water/waterTexNWS.png images/Water/waterTexEWS.png images/Water/waterTexNSS.png images/Water/waterTexNWES.png images/Water/waterTexNESS.png images/Water/waterTexSEWS.png images/Water/waterTexNWSS.png images/Water/waterTexPond.png" << endl;
    fout << endl;
    fout << "#Max Turns <200>" << endl;
    fout << "MAXTURNS 100" << endl;
    fout << endl;
    fout << "#TANK RULES" << endl;
    fout << "#STAT <AMMOUNT>" << endl;
    fout << "#VALID STATS: DAMAGE, HEALTH, RADAR, AP, SPECIAL, RANGE, AMMO" << endl;
    fout << "DAMAGE " << rand() % 8 + 1 << endl;
    fout << "HEALTH "<< rand() % 8 + 1 << endl;
    fout << "RADAR "<< rand() % 8 + 1 << endl;
    fout << "AP "<< rand() % 8 + 1 << endl;
    fout << "SPECIAL "<< rand() % 8 + 1 << endl;
    fout << "RANGE "<< rand() % 8 + 1 << endl;
    fout << "AMMO "<< rand() % 8 + 1 << endl;
    fout << endl;
    fout << "#AI IDLE SPEED: AI_SPEED <750>" << endl;
    fout << "AI_SPEED 10" << endl;
    fout << endl;
    fout << "#ANIMATION SPEED: ANIMATION_SPEED<20>" << endl;
    fout << "ANIMATION_SPEED 1" << endl;
    fout << endl;
    fout << "#BULLET SPEED: BULLET_SPEED<80>" << endl;
    fout << "BULLET_SPEED 1" << endl;
    fout << endl;
    fout << "#TANK SPEED: TANK_SPEED<400>" << endl;
    fout << "TANK_SPEED 1" << endl;
    fout << endl;
    fout << "#DISABLEGUI Disables GUI" << endl;
    fout << "#DISABLEGUI" << endl;

    fout.close();
    
    return 0;
}

void genRandomMap(vector< vector<char> > &map)
{
    int num;
    int randNum = rand() % 8 + 2;

    map.resize(15);

    for(int i = 0; i < map.size(); i++)
    {
	map[i].resize(15);
    }

    for(int i = 0; i < map.size(); i++)
    {
	for(int j = 0; j < map[i].size(); j++)
	{
	    map[i][j] = 'x';

	    if(rand() % randNum  == 0)
	    {
		num = rand() % 6;

		if(num == 0)
		{
		    map[i][j] = 'R';
		}	 	
		else if(num == 1)
		{
		    map[i][j] = 'O';
		}	 	
		else if(num == 2)
		{
		    map[i][j] = 'W';
		}	 	
		else if(num == 3)
		{
		    map[i][j] = 'B';
		}	 	
		else if(num == 4)
		{
		    map[i][j] = 'T';
		}
		else if(num == 5)
		{
		    map[i][j] = '1';
		}	 	
		else
		{
		    map[i][j] = 'x';
		}	 	
	    }
	}
    }

}

void showMap(vector< vector<char> > map)
{
    for(int i = 0; i < map.size(); i++)
    {
	for(int j = 0; j < map[i].size(); j++)
	{
	    cout << map[i][j] << " ";
	}
	cout << endl;
    }
}

/*
WIDTH 15
HEIGHT 15
MAP
xxxxxxxxOxWxxxx
xxxxxxxxxxxCxxO
xxOxxxxWxxxxOxx
xxxxxxxxxxxxxxx
xxxBxxxxxxxxxxx
xxxxxxxxxxWCxRx
xxxxTxxxxxxxxxx
xBxBxxxxxxxxxxx
OxxxxOxxxxxOxxx
xxxxxxxCxxxxBxx
xRBxxxxxxxxWxxx
xBxTxxxxxxxxxxx
xxxxxxxxxxCxxxx
xOxxxxxxBxxxxxx
RxxxxxxRxxxCxxx

AI Target 8 8 images/Purple/tankP_U.png images/Purple/tankP_R.png images/Purple/tankP_D.png images/Purple/tankP_L.png images/Purple/bulletP.png
AI Edelweiss 1 4 images/SkyBlue/tankSB_U.png images/SkyBlue/tankSB_R.png images/SkyBlue/tankSB_D.png images/SkyBlue/tankSB_L.png images/SkyBlue/bulletSB.png


#FIELDIMAGE <MAP FILE>
FIELDIMAGE images/green.png

#OBSTACLE_IMAGE <IMAGE1> [<IMAGE2>]
OBSTACLE_IMAGE images/tree/tree.png images/rock/rock.png
TREE_IMAGE images/tree/tree.png images/tree/treeb.png images/tree/treec.png images/tree/treed.png
BUSH_IMAGE images/bush/bush1.png images/bush/bush2.png images/bush/bush3.png images/bush/bush4.png
ROCK_IMAGE images/rock/rock.png images/rock/rockb.png images/rock/rockc.png
WATER_IMAGE images/Water/waterTex.png images/Water/waterTexNS.png images/Water/waterTexES.png images/Water/waterTexSS.png images/Water/waterTexWS.png images/Water/waterTexNES.png images/Water/waterTexSES.png images/Water/waterTexSWS.png images/Water/waterTexNWS.png images/Water/waterTexEWS.png images/Water/waterTexNSS.png images/Water/waterTexNWES.png images/Water/waterTexNESS.png images/Water/waterTexSEWS.png images/Water/waterTexNWSS.png images/Water/waterTexPond.png

#Max Turns <200>
MAXTURNS 100

#TANK RULES
#STAT <AMMOUNT>
#VALID STATS: DAMAGE, HEALTH, RADAR, AP, SPECIAL, RANGE, AMMO
DAMAGE 2
HEALTH 6
RADAR 4 
AP 2
SPECIAL 1
RANGE 4
AMMO 6

#AI IDLE SPEED: AI_SPEED <750>
AI_SPEED 10

#ANIMATION SPEED: ANIMATION_SPEED<20>
ANIMATION_SPEED 1

#BULLET SPEED: BULLET_SPEED<80>
BULLET_SPEED 1

#TANK SPEED: TANK_SPEED<400>
TANK_SPEED 1

#DISABLEGUI Disables GUI
#DISABLEGUI
*/
