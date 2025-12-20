#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>
#include <fstream>
#include <iomanip>
#include <utility>
#include <sstream>
#include <istream>
#include <limits.h>
#include <queue>
#include <set>
using namespace std;

class RiskAnalyser
{
    public:
        RiskAnalyser()
        {}

        void getInput()
        {
            ifstream f{"day15.txt"};
            string line;
            while(getline(f,line))
            {
                vector<int> row;
                for (auto& l:line)
                {
                    row.push_back(l - '0');
                }
                riskLevels.push_back(row);
            }
            N = riskLevels.size();
            M = riskLevels[0].size();
            riskLevels_total.resize(N);
            fill(riskLevels_total.begin(), riskLevels_total.end(), vector<int>(M, 0));
            riskLevelsFull.resize(N*5);
        }

        void createFullGrid()
        {
            for (int row = 0; row < 5; ++row)
            {
                vector<vector<int>> temp_riskMap = riskLevels;
                for (int y = 0; y < 5; ++y)
                {
                    addTileRight(row*N);
                    nextTile();
                }
                riskLevels = temp_riskMap;
                nextTile();
            }
            riskLevels = riskLevelsFull;
            N = riskLevels.size();
            M = riskLevels[0].size();
            riskLevels_total.resize(N);
            fill(riskLevels_total.begin(), riskLevels_total.end(), vector<int>(M, 0));
        }

        void findLowestRisk()   //Brute force method
        {
            findLowestRisk_helper(0, 0, 0);
        }

        void findLowestRisk_helper(int x, int y, int prevCost)
        {
            if ((x < 0) ||( x >= M) || (y < 0) || (y >= N)) 
                return; 

            int currentCost = prevCost + riskLevels[y][x];
            if ((riskLevels_total[y][x] == 0) || (currentCost < riskLevels_total[y][x]))
            {
                riskLevels_total[y][x] = currentCost;
                if ((x == (N-1)) && (y == (M-1)))
                    {
                        cout << currentCost << endl;
                        return;
                    }
            }
            else
            {
                return;
            }
            //cout << x << " " << y << endl;
        
            findLowestRisk_helper(x+1, y, currentCost);
            findLowestRisk_helper(x-1, y, currentCost);
            findLowestRisk_helper(x, y+1, currentCost);
            findLowestRisk_helper(x, y-1, currentCost);
        }

        int getTotalRisk()
        {
            return riskLevels_total[N-1][M-1] - riskLevels[0][0];
        }

    // Function that implements Dijkstra's single source shortest path algorithm
    // Thanks to ankitsumitg for inspiration.
    // Note to self: Do not use vectors unless necessary, shits slow as fuck
    // Runtime: <1 s.
    int dijkstra()
    {
        int dir_check = 4;      //Amount of direction to check (up,down,left,right)
        int dx[4] = {0, 0, -1, 1};
        int dy[4] = {1, -1, 0, 0};
        // priority queue min-heap
        priority_queue<pair<int,pair<int,int>>, 
                       vector<pair<int,pair<int,int>>>, greater<pair<int,pair<int,int>>>> q;
        q.push({0,{0,0}});

        set<pair<int,int>> visited;
        visited.insert({0,0});

        int curr_risk;
        int x,y;
        while (!q.empty())
        {
            auto [curr_risk, coord] = q.top();
            q.pop();
            x = coord.first;
            y = coord.second;

            if (x == M - 1 && y == N - 1)
                return curr_risk;
            for (int i=0; i < dir_check; ++i)
            {
                int x_curr = x + dx[i];
                int y_curr = y + dy[i];
                bool coord_is_found = visited.find(pair<int,int>(x_curr, y_curr)) != visited.end();
                if (((x_curr >= 0) && ( x_curr < M) && (y_curr >= 0) && (y_curr < N)) 
                    && (!coord_is_found))
                {
                    int risk = riskLevels[y_curr][x_curr];
                    q.push({curr_risk + risk, {x_curr, y_curr}});
                    visited.insert({x_curr, y_curr});
                }
            }
        }
        return 0; //Placeholder. Should not be used if run correctly
    }

    private:
        void nextTile()
        {
            for (auto& row:riskLevels)
            {
                for (auto& risk:row)
                {
                    ++risk;
                    if (risk == 10)
                        risk = 1;
                }
            }
        }

        void addTileRight(int r)
        {
            for (auto const& row:riskLevels)
            {
                for (auto const& risk:row)
                {
                    riskLevelsFull[r].push_back(risk);
                }
                ++r;
            }
        }

        vector<vector<int>> riskLevels;
        vector<vector<int>> riskLevelsFull;
        vector<vector<int>> riskLevels_total;
        int M{};               //Y
        int N{};               //X
};

void partTwo()
{
    RiskAnalyser a;
    a.getInput();
    a.createFullGrid();
    cout << "The total risk in the path is " << a.dijkstra();
}

void partOne()
{
    RiskAnalyser a;
    a.getInput();
    a.findLowestRisk();         //Uses a brute force approach
    cout << "The total risk in the path is " << a.getTotalRisk();
}

int main()
{
    //partOne();
    partTwo();

    return 0;
}