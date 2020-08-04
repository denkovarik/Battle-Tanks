#include <bits/stdc++.h>

using namespace std;



struct spot
{
    int x, y;
    bool visited = false;
    int prevx, prevy;
};



bool findPath(int startx, int starty, vector< vector<int> > &map);
void showMap(int height, int width, vector< vector<int> > map);

int main(int argc, char **argv)
{
    ifstream fin;
    vector< vector<int> > map;
    vector< vector<int> >adj;
    int height, width, x, y;
    char character;

    if(argc != 2)
    {
        cout << "This program requires only 1 argument, which is the name of "
             << "the input file to be tested." << endl;
        return 2;
    }

    fin.open(argv[1]);

    if(!fin)
    {
        cout << "Could not open file reader" << endl;
        return 1;
    }



    //Read file
    fin >> height >> width;
    fin >> x >> y;

    map.resize(height);

    for(int i = 0; i < height; i++)
    {
	map[i].resize(width);

	for(int j = 0; j < width; j++)
	{
	    fin >> character;

	    if(character == 'T')
	    {
		x = j;
		y = i;
	    }

	    map[i][j] = int(character);
	}
    }

    showMap(height, width, map);

    findPath(x, y, map);

    showMap(height, width, map);



    fin.close();

    return 0;
}



bool findPath(int startx, int starty, vector< vector<int> > &map)
{
    queue<spot> next;
    vector< vector<spot> > explored;
    spot newSpot;
    spot cur;
    spot end;
    spot start;
    int width, height;

    explored.resize(map.size());
    height = map.size();
    
	width = map[0].size();

    for(int i = 0; i < height && width > 0; i++)
    {
	explored[i].resize(width);
    }

    //make start node
    newSpot.x = startx;
    newSpot.y = starty;
    newSpot.prevx = startx;
    newSpot.prevy = starty;

    start = newSpot;

    next.push(newSpot);

    while(!next.empty())
    {
	cur = next.front();
       	next.pop();
	cur.visited = true;

	if(map[cur.y][cur.x] == (int)'F')
	{
	    //retrace steps
	    cout << "Made it" << endl;
	    end = explored[cur.y][cur.x];

	    while(cur.x != startx || cur.y != starty)
	    {
		cur = explored[cur.prevy][cur.prevx];
	
		if(cur.x != startx || cur.y != starty)
		    map[cur.y][cur.x] = int(' ');
	    }

	    return true;

	}
	else
	{ 
	    //Store cur
	    if(cur.y >= 0 && cur.y < height && cur.x >= 0 && cur.x < width)
		explored[cur.y][cur.x] = cur;

	    //search one space forward in each direction
	    newSpot = cur;

	    //up
	    if(cur.y - 1 >= 0  && !explored[cur.y-1][cur.x].visited && map[cur.y-1][cur.x] != 'R')
	    {
		newSpot.y = cur.y - 1;
		newSpot.x = cur.x;
		newSpot.prevx = cur.x;
		newSpot.prevy = cur.y;
		newSpot.visited = true;
		explored[cur.y-1][cur.x] = newSpot;
		next.push(newSpot);
	    }
	    //up right
	    if(cur.y - 1 >= 0 && cur.x + 1 < width && !explored[cur.y-1][cur.x+1].visited && map[cur.y-1][cur.x+1] != 'R')
	    {
		newSpot.y = cur.y - 1;
		newSpot.x = cur.x + 1;
		newSpot.prevx = cur.x;
		newSpot.prevy = cur.y;
		newSpot.visited = true;
		explored[cur.y-1][cur.x+1] = newSpot;
		next.push(newSpot);
	    }
	    //right
	    if(cur.x + 1 < width && !explored[cur.y][cur.x+1].visited && map[cur.y][cur.x+1] != 'R')
	    {
		newSpot.x = cur.x + 1;
		newSpot.y = cur.y;
		newSpot.prevx = cur.x;
		newSpot.prevy = cur.y;
		newSpot.visited = true;
		explored[cur.y][cur.x+1] = newSpot;
		next.push(newSpot);
	    }
	    //down right
	    if(cur.y + 1 < height && cur.x + 1 < width && !explored[cur.y+1][cur.x+1].visited && map[cur.y+1][cur.x+1] != 'R')
	    {
		newSpot.y = cur.y + 1;
		newSpot.x = cur.x + 1;
		newSpot.prevx = cur.x;
		newSpot.prevy = cur.y;
		newSpot.visited = true;
		explored[cur.y+1][cur.x+1] = newSpot;
		next.push(newSpot);
	    }
	    //down
	    if(cur.y + 1 < height && !explored[cur.y+1][cur.x].visited && map[cur.y+1][cur.x] != 'R')
	    {
		newSpot.y = cur.y + 1;
		newSpot.x = cur.x;
		newSpot.prevx = cur.x;
		newSpot.prevy = cur.y;
		newSpot.visited = true;
		explored[cur.y+1][cur.x] = newSpot;
		next.push(newSpot);
	    }
	    //down left
	    if(cur.y + 1 < height && cur.x - 1 >= 0 && !explored[cur.y+1][cur.x-1].visited && map[cur.y+1][cur.x-1] != 'R')
	    {
		newSpot.y = cur.y + 1;
		newSpot.x = cur.x - 1;
		newSpot.prevx = cur.x;
		newSpot.prevy = cur.y;
		newSpot.visited = true;
		explored[cur.y+1][cur.x-1] = newSpot;
		next.push(newSpot);
	    }
	    //left
	    if(cur.x - 1 >= 0 && !explored[cur.y][cur.x-1].visited && map[cur.y][cur.x-1] != 'R')
	    {
		newSpot.y = cur.y;
		newSpot.x = cur.x - 1;
		newSpot.prevx = cur.x;
		newSpot.prevy = cur.y;
		newSpot.visited = true;
		explored[cur.y][cur.x-1] = newSpot;
		next.push(newSpot);
	    }
	    //up left
	    if(cur.y - 1 >= 0 && cur.x - 1 >= 0 && !explored[cur.y-1][cur.x-1].visited && map[cur.y-1][cur.x-1] != 'R')
	    {
		newSpot.y = cur.y - 1;
		newSpot.x = cur.x - 1;
		newSpot.prevx = cur.x;
		newSpot.prevy = cur.y;
		newSpot.visited = true;
		explored[cur.y-1][cur.x-1] = newSpot;
		next.push(newSpot);
	    }
	}
    }


    return false;
}



void showMap(int height, int width, vector< vector<int> > map)
{
    for(int i = 0; i < height; i++)
    {
	for(int j = 0; j < width; j++)
	{
	    cout << (char)map[i][j] << " ";
	}
	cout << endl;
    }
    cout << endl;

    return;
}
