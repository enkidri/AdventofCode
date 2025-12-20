#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

//Very inefficient code and much larger than it needs to be
//but it does the job I guess.

class heightMap
{
    public:
        heightMap()
        {}

        void insertRow(string const& line)
        {
            for (auto const& digit:line)
            {
                row.push_back(digit - '0');
            }
            heightValues.push_back(row);
            row.clear();
        }

        void print()
        {
            for (auto const& y:heightValues)
            {
                for (auto const& x:y)
                {
                    cout << x << " ";
                }
                cout << endl;
            }
        }

        void findLow()
        {
            int row_size = heightValues.size();
            int col_size = heightValues[0].size();
            for (int y=0; y < row_size; ++y)
            {
                vector<int> row = heightValues[y];
                for (int x=0; x < col_size; ++x)
                {
                    int curr = row[x];
                    int idx_up;
                    int idx_down;
                    int idx_right;
                    int idx_left;
                    if ((x == 0) && (y == 0)) //Upper left
                    {
                        if ((row[x+1] > curr) && (heightValues[1][0] > curr))
                        {
                            ++low_count; 
                            risk_level += curr + 1;
                            // cout << curr << endl;
                        }
                    }
                    else if((x == col_size - 1) && (y == 0)) //Upper right
                    {
                        if ((row[x-1] > curr) && (heightValues[1][x] > curr)) 
                        {
                            ++low_count; 
                            risk_level += curr + 1;
                            // cout << curr << endl;
                        }
                    }
                    else if((x == 0) && (y == row_size - 1)) //Lower left
                    {
                        if ((row[x+1] > curr) && (heightValues[y-1][0] > curr))
                        {
                            ++low_count; 
                            risk_level += curr + 1;
                            // cout << curr << endl;
                        }
                    }
                    else if((x == col_size - 1) && (y == row_size - 1)) //Lower right
                    {
                        if ((row[x-1] > curr) && (heightValues[row_size -2][x] > curr))
                        {
                            ++low_count; 
                            risk_level += curr + 1;
                            // cout << curr << endl;
                        }
                    }
                    else if((y == 0) && (0 < x < col_size -1))  //Upper edge
                    {
                        if ((row[x+1] > curr) && (heightValues[1][x] > curr) && (row[x-1] > curr))
                        {
                            ++low_count; 
                            risk_level += curr + 1;
                            // cout << curr << endl;
                        }
                    }
                    else if((y == row_size - 1) && (0 < x < col_size -1))  //Lower edge
                    {
                        if ((row[x+1] > curr) && (heightValues[y-1][x] > curr) && (row[x-1] > curr))
                        {
                            ++low_count; 
                            risk_level += curr + 1;
                            // cout << curr << endl;
                        }
                    }
                    else if((x == 0) && (0 < x < row_size -1))  //left edge
                    {
                        if ((row[x+1] > curr) && (heightValues[y-1][x] > curr) && (heightValues[y+1][x] > curr))
                        {
                            ++low_count; 
                            risk_level += curr + 1;
                            // cout << curr << endl;
                        }
                    }
                    else if((x == col_size - 1) && (0 < x < row_size -1))  //left edge
                    {
                        if ((row[x-1] > curr) && (heightValues[y-1][x] > curr) && (heightValues[y+1][x] > curr))
                        {
                            ++low_count; 
                            risk_level += curr + 1;
                            // cout << curr << endl;
                        }
                    }
                    else
                    {
                        if ((row[x+1] > curr) && (row[x-1] > curr) && (heightValues[y-1][x] > curr) && (heightValues[y+1][x] > curr))
                        {
                            ++low_count; 
                            risk_level += curr + 1;
                            // cout << curr << endl;
                        }
                    }
                }
            }
        }

        void findAllBasins()
        {
            int row_size = heightValues.size();
            int col_size = heightValues[0].size();
            for (int y=0; y < row_size; ++y)
            {
                vector<int> row = heightValues[y];
                for (int x=0; x < col_size; ++x)
                {
                    int curr = row[x];
                    int idx_up;
                    int idx_down;
                    int idx_right;
                    int idx_left;
                    if ((x == 0) && (y == 0)) //Upper left
                    {
                        if ((row[x+1] > curr) && (heightValues[1][0] > curr))
                        {
                            basin_sizes.push_back(findBasinSize(heightValues, y, x));
                            // cout << y << " " << x << endl;
                        }
                    }
                    else if((x == col_size - 1) && (y == 0)) //Upper right
                    {
                        if ((row[x-1] > curr) && (heightValues[1][x] > curr)) 
                        {
                            basin_sizes.push_back(findBasinSize(heightValues, y, x));
                            // cout << y << " " << x << endl;
                        }
                    }
                    else if((x == 0) && (y == row_size - 1)) //Lower left
                    {
                        if ((row[x+1] > curr) && (heightValues[y-1][0] > curr))
                        {
                            basin_sizes.push_back(findBasinSize(heightValues, y, x));
                            // cout << y << " " << x << endl;
                        }
                    }
                    else if((x == col_size - 1) && (y == row_size - 1)) //Lower right
                    {
                        if ((row[x-1] > curr) && (heightValues[row_size -2][x] > curr))
                        {
                            basin_sizes.push_back(findBasinSize(heightValues, y, x));
                            // cout << y << " " << x << endl;
                        }
                    }
                    else if((y == 0) && (0 < x < col_size -1))  //Upper edge
                    {
                        if ((row[x+1] > curr) && (heightValues[1][x] > curr) && (row[x-1] > curr))
                        {
                            basin_sizes.push_back(findBasinSize(heightValues, y, x));
                            // cout << y << " " << x << endl;
                        }
                    }
                    else if((y == row_size - 1) && (0 < x < col_size -1))  //Lower edge
                    {
                        if ((row[x+1] > curr) && (heightValues[y-1][x] > curr) && (row[x-1] > curr))
                        {
                            basin_sizes.push_back(findBasinSize(heightValues, y, x));
                            // cout << y << " " << x << endl;
                        }
                    }
                    else if((x == 0) && (0 < x < row_size -1))  //left edge
                    {
                        if ((row[x+1] > curr) && (heightValues[y-1][x] > curr) && (heightValues[y+1][x] > curr))
                        {
                            basin_sizes.push_back(findBasinSize(heightValues, y, x));
                            // cout << y << " " << x << endl;
                        }
                    }
                    else if((x == col_size - 1) && (0 < x < row_size -1))  //left edge
                    {
                        if ((row[x-1] > curr) && (heightValues[y-1][x] > curr) && (heightValues[y+1][x] > curr))
                        {
                            basin_sizes.push_back(findBasinSize(heightValues, y, x));
                            // cout << y << " " << x << endl;
                        }
                    }
                    else
                    {
                        if ((row[x+1] > curr) && (row[x-1] > curr) && (heightValues[y-1][x] > curr) && (heightValues[y+1][x] > curr))
                        {
                            basin_sizes.push_back(findBasinSize(heightValues, y, x));
                            // cout << y << " " << x << endl;
                        }
                    }
                }
            }
        }

        void floodFill(vector<vector<int>>& myScreen, int x, int y, int currColor, int& count) 
        { 
            int M = heightValues.size();
            int N = heightValues[0].size();
            // Base cases 
            if (x < 0 || x >= M || y < 0 || y >= N) 
                return; 
            if (myScreen[x][y] == 9) 
                return; 
        
            // Replace the color at cell (x, y) 
            myScreen[x][y] = 9; 
            ++count;
        
            // Recursively call for north, east, south and west 
            floodFill(myScreen, x+1, y, currColor, count); 
            floodFill(myScreen, x-1, y, currColor, count); 
            floodFill(myScreen, x, y+1, currColor, count); 
            floodFill(myScreen, x, y-1, currColor, count); 
        } 

        int findBasinSize(vector<vector<int>>& myScreen, int x, int y) 
        { 
            int count = 0;
            int currColor = myScreen[x][y]; 
            floodFill(myScreen, x, y, currColor, count); 
            return count;
        } 

        int getRisk()
        {
            return risk_level;
        }
        
        int getCount()
        {
            return low_count;
        }

        int getMaxBasin()
        {
            return *max_element(basin_sizes.begin(), basin_sizes.end());
        }

        void printBasinSize()
        {
            cout << "The basin sizes are: ";
            for (auto& nr:basin_sizes)
            {
                cout << nr << " ";
            }
            cout << endl;
        }

        int findSpecificBasin()
        {
            int size = findBasinSize(heightValues, 4, 6);
            return size;
        }

        int findMaxAndRemove()
        {
            vector<int>::iterator m = max_element(basin_sizes.begin(), basin_sizes.end());
            int idx = distance(basin_sizes.begin(), m);
            int max_val = *m;
            basin_sizes.erase(basin_sizes.begin()+idx, basin_sizes.begin()+idx+1);
            return max_val;
        }

    private:
        vector<vector<int>> heightValues;
        vector<int> row;
        vector<int> basin_sizes{};
        int low_count{};
        int risk_level{};

};

void partTwo(string const& filename)
{
    ifstream f{filename};
    string line{};
    heightMap m;

    while(getline(f,line))
    {
        m.insertRow(line);
    }
    m.findAllBasins();
    
    int sol = 1;
    for (int i=0; i < 3; ++i)
    {
        sol *= m.findMaxAndRemove();
    }

    cout << "The product of the three largest basin is " << sol << endl;
}

void partOne(string const& filename)
{
    ifstream f{filename};
    string line{};
    heightMap m;

    while(getline(f,line))
    {
        m.insertRow(line);
    }
    m.findLow();
    cout << "The risk level is " << m.getRisk() << endl;
}

int main(int argc, char** argv)
{
    partTwo(argv[1]);
    return 0;
}