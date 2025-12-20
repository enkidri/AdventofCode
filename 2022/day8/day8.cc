#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<iterator>
#include<numeric>
#include<regex>
#include<stack>
#include<queue>
#include<set>
#include<map>
#include<sstream>
#include<functional>
#include<algorithm>
using namespace std;

typedef vector<vector<int>> TreeGrid;

bool upIsShorter(TreeGrid& g, int center, int x, int y)
{
    for (int i=y-1; i >= 0; i--)
    {
        if (g[i][x] >= center)
        {
            return false;
        }
    }
    return true;
}

bool rightIsShorter(TreeGrid& g, int center, int x, int y)
{
    int x_size = g[0].size();
    for (int i=x+1; i < x_size; i++)
    {
        if (g[y][i] >= center)
        {
            return false;
        }
    }

    return true;
}

bool leftIsShorter(TreeGrid& g, int center, int x, int y)
{
    int x_size = g[0].size();
    for (int i=x-1; i >= 0; i--)
    {
        if (g[y][i] >= center)
        {
            return false;
        }
    }

    return true;
}

bool downIsShorter(TreeGrid& g, int center, int x, int y)
{
    int y_size = g.size();
    for (int i=y+1; i < y_size; i++)
    {
        if (g[i][x] >= center)
        {
            return false;
        }
    }
    return true;
}

int upView(TreeGrid& g, int center, int x, int y)
{
    int dist{1};
    for (int i=y-1; i > 0; i--)
    {
        if (g[i][x] >= center)
        {
            break;
        }
        dist++;
    }
    return dist;
}

int rightView(TreeGrid& g, int center, int x, int y)
{
    int dist{1};
    int x_size = g[0].size();
    for (int i=x+1; i < x_size-1; i++)
    {
        if (g[y][i] >= center)
        {
            break;
        }
        dist++;
    }

    return dist;
}

int leftView(TreeGrid& g, int center, int x, int y)
{
    int dist{1};
    int x_size = g[0].size();
    for (int i=x-1; i > 0; i--)
    {
        if (g[y][i] >= center)
        {
            break;
        }
        dist++;
    }

    return dist;
}

int downView(TreeGrid& g, int center, int x, int y)
{
    int dist{1};
    int y_size = g.size();
    for (int i=y+1; i < y_size-1; i++)
    {
        if (g[i][x] >= center)
        {
            break;
        }
        dist++;
    }
    return dist;
}

//This part could have done cleaner by checking if there
//is a number that is larger than the center number in both row/column
//If true then view is blocked.
void partOne()
{   
    string input;
    ifstream file("day8.txt");
    
    TreeGrid grid;
    int grid_y_size{};
    int grid_x_size{};

    while (getline(file,input))
    {
        vector<int> row;
        for (char& c:input)
        {
            row.push_back(c - '0');
        }
        grid.push_back(row);
    }
    grid_x_size = grid[0].size();
    grid_y_size = grid.size();

    int number_hidden_trees{};
    for (int y=1; y < grid_y_size - 1; y++)
    {
        for (int x=1; x < grid_x_size - 1; x++)
        {
            int center = grid[y][x];
            bool rightOK = rightIsShorter(grid, center, x, y);
            bool leftOK = leftIsShorter(grid, center, x, y);
            bool upOK = upIsShorter(grid, center, x, y);
            bool downOK = downIsShorter(grid, center, x, y);

            if ((rightOK) || (leftOK) || (upOK) || (downOK))
            {
                number_hidden_trees++;
            }
        }
    }
    number_hidden_trees += 2*grid_y_size + 2*(grid_x_size-2);
    

    cout << "Part 1: The number of hidden trees are " << number_hidden_trees << endl;

}

void partTwo()
{
    string input;
    ifstream file("day8.txt");

    TreeGrid grid;
    int grid_y_size{};
    int grid_x_size{};

    while (getline(file,input))
    {
        vector<int> row;
        for (char& c:input)
        {
            row.push_back(c - '0');
        }
        grid.push_back(row);
    }
    grid_x_size = grid[0].size();
    grid_y_size = grid.size();

    int top_score{};
    for (int y=1; y < grid_y_size-1; y++)
    {
        for (int x=1; x < grid_x_size-1; x++)
        {
            int center = grid[y][x];
            int right_view = rightView(grid, center, x, y);
            int left_view = leftView(grid, center, x, y);
            int up_view = upView(grid, center, x, y);
            int down_view = downView(grid, center, x, y);

            int scenic_score = right_view * left_view * up_view * down_view;
            if (scenic_score > top_score)
            {
                top_score = scenic_score;
            }
        }
    }

    cout << "Part 2: The tree with best view has a scenic score of " << top_score << endl;

    
}

int main()
{
    partOne();
    partTwo();
    return 0;
}
