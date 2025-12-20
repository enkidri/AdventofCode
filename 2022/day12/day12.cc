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
using namespace std;

typedef pair<int, int> Coordinates;
typedef vector<vector<int>> Grid;

pair<pair<Coordinates,Coordinates>, Grid> parser(ifstream& file)
{
    string input{};
    Grid v;
    Coordinates start;
    Coordinates end;
    
    int y{};
    while (getline(file, input))
    {
        int x = 0;
        vector<int> v_row;
        for (char& c:input)
        {
            if (c == 'S')
            {
                start = {x, y};
                v_row.push_back(0);
            }
            else if (c == 'E')
            {
                end = {x, y};
                v_row.push_back('z' - 'a');
            }
            else
            {
                v_row.push_back(c - 'a');
            }
            
            
            x++;
        }

        v.push_back(v_row);
        y++;
    }
    
    return pair<pair<Coordinates,Coordinates>, Grid>{{start, end}, v};
}

void gridPrinter(Grid& v)
{
    for (auto& row:v)
    {
        for (auto& ele:row)
        {
            cout << setfill(' ') << setw(2) <<  ele << " ";
        }
        cout << endl;
    }
}

vector<Coordinates> next(Grid& g, Coordinates& c)
{
    vector<Coordinates> allowed_pos;
    int x = c.first;
    int y = c.second;
    int current = g[y][x];
    int x_size = g[0].size();
    int y_size = g.size();

    int up{};
    int down{};
    int left{};
    int right{};
    if ((y-1)>=0)
    {
        up = g[y-1][x];
        if (up <= current + 1)
        {
            allowed_pos.push_back(Coordinates(x, y-1));
        }
    }
    if ((y+1) < y_size)
    {
        down = g[y+1][x];
        if (down <= current + 1)
        {
            allowed_pos.push_back(Coordinates(x, y+1));
        }
    }
    if ((x-1) >= 0)
    {
        left = g[y][x-1];
        if (left <= current + 1)
        {
            allowed_pos.push_back(Coordinates(x-1, y));
        }
    }
    if ((x+1) < x_size)
    {
        right = g[y][x+1];
        if (right <= current + 1)
        {
            allowed_pos.push_back(Coordinates(x+1, y));
        }
    }
    
    return allowed_pos;
}


//0 start
//-1 goal
void partOne()
{   
    ifstream file("day12.txt");

    pair<pair<Coordinates,Coordinates>, Grid> parsed = parser(file);
    Coordinates start = parsed.first.first;
    Coordinates goal = parsed.first.second;
    Grid grid = parsed.second;
    int y_size = grid.size();
    int x_size = grid[0].size();
    Grid cost_to_come(y_size, vector<int>(x_size, 0));

    auto compare = [](pair<Coordinates,int>& lhs, pair<Coordinates,int>& rhs)
                {
                    return lhs.second > rhs.second;
                };

    priority_queue<pair<Coordinates,int>, vector<pair<Coordinates,int>>, decltype(compare)> queue(compare);
    queue.push(pair<Coordinates,int>{start, 0});
    while (!queue.empty())
    {
        queue.pop();
        pair<Coordinates,int> current = queue.top();
        if (current.first == goal)
        {
            break;
        }
        vector<Coordinates> n = next(grid, current.first);
        for (auto& coord:n)
        {
            int x = current.first.first;
            int y = current.first.second;
            int xi = coord.first;
            int yi = coord.second;
            if (((cost_to_come[yi][xi] == 0)) || (cost_to_come[yi][xi] > cost_to_come[y][x] + 1))
            {
                cost_to_come[yi][xi] = cost_to_come[y][x] + 1;
                queue.push(pair<Coordinates,int>(Coordinates{xi,yi}, cost_to_come[yi][xi]));
            }
        }
    }

    cout << "Part 1: The number of steps to the best spot is " << cost_to_come[goal.second][goal.first] << endl;

}

void partTwo()
{
    ifstream file("day12.txt");

    pair<pair<Coordinates,Coordinates>, Grid> parsed = parser(file);
    Grid grid = parsed.second;
    Coordinates goal = parsed.first.second;
    vector<Coordinates> starts;
    {
        int y = 0;
        for (auto& row:grid)
        {
            int x = 0;
            for (auto& ele:row)
            {
                if (ele == 0)
                {
                    starts.push_back(Coordinates{x,y});
                }
                x++;
            }
            y++;
        }
    }

    int y_size = grid.size();
    int x_size = grid[0].size();
    vector<int> lengths;
    for (auto& start:starts)
    {
        Grid cost_to_come(y_size, vector<int>(x_size, 0));
        auto compare = [](pair<Coordinates,int>& lhs, pair<Coordinates,int>& rhs)
                    {
                        return lhs.second > rhs.second;
                    };
        priority_queue<pair<Coordinates,int>, vector<pair<Coordinates,int>>, decltype(compare)> queue(compare);
        queue.push(pair<Coordinates,int>{start, 0});
        while (!queue.empty())
        {
            pair<Coordinates,int> current = queue.top();
            if (current.first == goal)
            {
                lengths.push_back(cost_to_come[goal.second][goal.first]);
                break;
            }
            queue.pop();
            vector<Coordinates> n = next(grid, current.first);
            for (auto& coord:n)
            {
                int x = current.first.first;
                int y = current.first.second;
                int xi = coord.first;
                int yi = coord.second;
                if ((((cost_to_come[yi][xi] == 0)) || (cost_to_come[yi][xi] > cost_to_come[y][x] + 1)))
                {
                    cost_to_come[yi][xi] = cost_to_come[y][x] + 1;
                    queue.push(pair<Coordinates,int>(Coordinates{xi,yi}, cost_to_come[yi][xi]));
                }
            }
        }
    }

    int ans = *min_element(lengths.begin(), lengths.end());

    cout << "Part 2: The smallest number of steps to the goal from the best viewing spot is " << ans << endl;
}

int main()
{
    partOne();
    partTwo();

    return 0;
}
