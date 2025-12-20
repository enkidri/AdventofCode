#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <regex>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <sstream>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <variant>

#include <chrono>
using namespace std;

typedef pair<int,int> Coord;
typedef pair<Coord,Coord> SensVal;

vector<SensVal> parse(ifstream& file)
{
    string input{};
    const regex regex(R"(-?\d+)");
    smatch match;
    vector<SensVal> v;
    while (getline(file, input))
    {
        int i = 0;
        Coord sensor;
        Coord beacon;
        while (regex_search(input, match, regex))
        {
            if (i == 0)
                sensor.first = stoi(match.str());
            else if (i == 1)
                sensor.second = stoi(match.str());
            else if (i == 2)
                beacon.first = stoi(match.str());
            else if (i == 3)
                beacon.second = stoi(match.str());

            input = match.suffix();
            i++;
        }
        v.push_back(make_pair(sensor,beacon));
    }

    return v;
}

void printMap(vector<SensVal>& s)
{
    for (auto& sens:s)
    {
        Coord sensor = sens.first;
        Coord beacon = sens.second;

        cout << sensor.first << " " << sensor.second << endl;
        cout << beacon.first << " " << beacon.second << endl << endl;
    }
}

int manhattan(Coord& sensor, Coord& beacon)
{
    int x1 = sensor.first;
    int x2 = beacon.first;

    int y1 = sensor.second;
    int y2 = beacon.second;
    return abs(x2-x1) + abs(y2-y1);
}

void partOne()
{   
    ifstream file("day15.txt");
    vector<SensVal> parsed = parse(file);
    int y_goal = 2000000;
    set<Coord> known_pos;
    for (auto& sens:parsed)
    {
        Coord& sensor = sens.first;
        int x1 = sensor.first;
        int y1 = sensor.second;

        Coord& beacon = sens.second;
        int dist = manhattan(sensor, beacon);
        if ((y1-dist <= y_goal) && (y_goal <= y1+dist))
        {
            int dy = abs(y_goal - y1);
            int x_start = x1 - dist + dy;
            int num_iter= 2 * dist + 1 - 2 * dy;
            for (int i=0; i < num_iter; i++)
            {
                Coord c = make_pair(x_start+i, y_goal);
                if (c != beacon)
                    known_pos.insert(c);
            }
        }
    }
    
    cout << "Part 1: Known positions are " << known_pos.size() << endl;

}

vector<pair<Coord,int>> manhattan_associate(vector<SensVal>& v)
{
    vector<pair<Coord,int>> p;
    for(auto& sens:v)
    {
        Coord sensor = sens.first;
        Coord beacon = sens.second;
        int man = manhattan(sensor, beacon);
        p.push_back(make_pair(sensor,man));
    }

    return p;
}

bool isCovered(Coord current, vector<pair<Coord,int>> v)
{
    for (auto& ele:v)
    {
        Coord& sensor = ele.first;
        int manhattan = ele.second;
    }
    return false;
}

void partTwo()
{
    ifstream file("day15.txt");
    vector<SensVal> parsed = parse(file);
    vector<pair<Coord,int>> v = manhattan_associate(parsed);

    int ij_max = 4000000;
    bool spot_found = false;
    Coord spot;
    for (int i=0; (i < ij_max) && (true); i++)
    {
        for (int j=0; j < ij_max; j++)
        {
            int x1 = j;
            int y1 = i;
            bool is_covered = false;
            int x_skip{};
            for (auto& ele:v)
            {
                Coord sensor = ele.first;
                int x2 = sensor.first;
                int y2 = sensor.second;
                int d_man_sens = ele.second;
                int d_man_to_current = abs(x2-x1) + abs(y2-y1);
                if (d_man_to_current <= d_man_sens)
                {
                    int dy = abs(y2-y1);
                    x_skip = x2+d_man_sens -dy - j;
                    is_covered = true;
                    break;
                }
            }

            if (is_covered)
            {
                j += x_skip;
            }
            else
            {
                spot_found = true; 
                spot = make_pair(j,i);
                break;
            }
        }
    }

    unsigned long long int tuning_freq = (unsigned long long int)spot.first * 4000000 
                                       + (unsigned long long int)spot.second;
    cout << "Part 2: The tuning frequency is " << tuning_freq << endl;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();
    partOne();
    partTwo();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
    cout << "It took " << duration.count()/(float)1000000 << " seconds to run." << endl;
    return 0;
}
