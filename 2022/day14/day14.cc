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
using namespace std;

// Holy hell sets are so fkin fast at checking if elements exists.
// Vectors fkn sucks lol
// Naive implementation of the problem. Every grain of sand is simulated.

typedef pair<int,int> Coord;
typedef vector<Coord> Rock;
typedef vector<Rock> Rocks;

Rocks parse(ifstream& file)
{
    string input;
    regex regex(R"((\d+),(\d+))");
    smatch match;
    Rocks rs;
    while (getline(file, input))
    {
        Rock r;
        while (regex_search(input, match, regex))
        {
            int x = stoi(match[1].str());
            int y = stoi(match[2].str());
            r.push_back({x,y});
            input = match.suffix();
        }
        rs.push_back(r);
    }

    return rs;
}

void rocksPrinter(Rocks& rocks)
{
    for (auto& r:rocks)
    {
        for (auto& coord:r)
        {
            cout << coord.first << "," << coord.second << "  ";
        }
        cout << endl;
    }
}


bool drop(vector<Coord>& rock, int lwst)
{
    Coord current = {500, 0};
    bool is_stationary = false;
    bool is_overflow = false;

    while (!is_stationary)
    {
        Coord prev = current;
        current.first++;
        if (current.second > lwst)
        {
            is_overflow = true;
            break;
        } 
        
        if (find(rock.begin(), rock.end(), current) != rock.end())
        {
            Coord left = {current.first, current.second -1};
            Coord right = {current.first, current.second +1};
            if (find(rock.begin(), rock.end(), left) == rock.end())
            {
                current.second--;
                break;
            }
            else if (find(rock.begin(), rock.end(), right) == rock.end())
            {
                current.first++;
                break;
            }
            else {
                is_stationary = true;
                rock.push_back(prev);
                break;
            }
        }
    }
    return is_overflow;
}

pair<int,set<Coord>> transform(Rocks& r)
{
    set<Coord> platform;
    int lowest_platform{};
    for (auto& rock:r)
    {
        for (int i=0; i < rock.size()-1; i++)
        {
            int dx = rock[i+1].first-rock[i].first;
            int dy = rock[i+1].second-rock[i].second;

            if (rock[i].second > lowest_platform)
                lowest_platform = rock[i].second;
            else if (rock[i+1].second > lowest_platform)
                lowest_platform = rock[i+1].second;

            for (int k=0; k < max(abs(dx),abs(dy)); k++)
            {
                Coord c;
                c = make_pair(rock[i].first + k*dx/max(abs(dx),abs(dy)), 
                            rock[i].second + k*dy/max(abs(dx),abs(dy)));
                platform.insert(c);
            }
        }
        platform.insert(rock[rock.size()-1]);
    }
    return {lowest_platform,platform};
}

void partOne()
{   
    ifstream file("day14.txt");
    Rocks parsed = parse(file);
    pair<int,set<Coord>> t = transform(parsed);
    int lwst = t.first;
    set<Coord>& rock = t.second;

    bool is_overflow = false;
    bool is_stationary = false;
    int drop_count{};
    //for (int i=0; i < 22; i++)
    while (!is_overflow)
    {
        Coord current = {500,0};
        is_stationary = false;
        while (!is_stationary)
        {
            Coord prev = current;
            current.second++;
            if (current.second > lwst)
            {
                is_overflow = true;
                break;
            } 
            if (rock.find(current) != rock.end())
            {
                Coord left = {current.first-1, current.second};
                Coord right = {current.first+1, current.second};
                if (rock.find(left) == rock.end())
                {
                    current.first--;
                    continue;
                }
                else if (rock.find(right) == rock.end())
                {
                    current.first++;
                    continue;
                }
                else {
                    is_stationary = true;
                    rock.insert(prev);
                    drop_count++;
                    break;
                }
            }
        }
    }
    
    cout << "Part 1: It took " << drop_count << " iterations to overflow." << endl;

}



void partTwo()
{
    ifstream file("day14.txt");
    Rocks parsed = parse(file);
    pair<int,set<Coord>> t = transform(parsed);
    int lwst = t.first;
    set<Coord>& rock = t.second;
    int floor = t.first + 2;

    bool is_blocked = false;
    bool is_stationary = false;
    int drop_count{};
    for (int i=0; i<2; i++)
    while (!is_blocked)
    {
        Coord current = {500,0};
        is_stationary = false;
        while (!is_stationary)
        {
            Coord prev = current;
            current.second++;
            if (rock.find(current) != rock.end())
            {
                Coord left = {current.first-1, current.second};
                Coord right = {current.first+1, current.second};
                if (rock.find(left) == rock.end())
                {
                    current.first--;
                    continue;
                }
                else if (rock.find(right) == rock.end())
                {
                    current.first++;
                    continue;
                }
                else {
                    if (prev == make_pair(500,0))
                    {
                        is_blocked = true;
                        break;
                    }
                    
                    is_stationary = true;
                    rock.insert(prev);
                    drop_count++;
                    break;
                }
            }
            if(current.second == floor)
            {
                is_stationary = true;
                rock.insert(prev);
                drop_count++;
                break;
            }
        }
    }
   
    cout << "Part 2: It took " << drop_count << " drops to block the start" << endl;
}

int main()
{
    partOne();
    partTwo();

    return 0;
}
