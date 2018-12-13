#include <bits/stdc++.h>

using namespace std;

void placeFlags(int radar, vector<int> &map, int height, int width);
void showMap(int height, int width, vector<int> map);

int main(int argc, char **argv)
{
    vector<int> map;
    ifstream fin;
    int width, height;
    char character;
    int radar = 4;

    if(argc == 2)
    {
         radar = atof(argv[1]);
    }
    else if(argc > 2)
    {
        cout << "This program takes 0 to 1 arguments. The one argument "
             << "passed into this program represents the size of the simulated "
             << "tank's radar." << endl;
        return 1;
    }

    fin.open("testmap.txt");

    if(!fin)
    {
        cout << "Could not open file reader" << endl;
        return 0;
    }

    fin >> height >> width;

    map.resize(width * height);

    for(int i = 0; i < height * width; i++)
    {
        fin >> character;

        map[i] = (int)character;
    }

    showMap(height, width, map);

    placeFlags(radar, map, height, width);

    showMap(height, width, map);

    fin.close();

    return 0;
}



void placeFlags(int radar, vector<int> &map, int height, int width)
{
    for(int i = radar / 2 - 1; i < height; i += radar / 2)
    {
        for(int j = radar / 2 - 1; j < width; j += radar / 2)
        {
            map[j + i * width] = int('F');
        }
    }
    cout << endl;    

    return;
}



void showMap(int height, int width, vector<int> map)
{
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            cout << (char)map[j + i * width] << " ";
        }
        cout << endl;
    }

    return;
}
