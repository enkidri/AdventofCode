#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <regex>
#include <iterator>
#include <utility>

using Grid = std::vector<std::vector<char>>;

struct Point2D
{
    size_t x;
    size_t y;
};

std::vector<int> StringToVector(std::string& str)
{
    std::stringstream ss(str);
    std::vector<int> vec;
    int val;
    while (ss >> val)
    {
        vec.push_back(val);
    }

    return vec;
}

void Printer(std::vector<int>& vec)
{
    for (auto& val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl
        << std::endl;
}

void Printer(std::string::const_iterator it_start, std::string::const_iterator it_end)
{
    std::cout << "Printer: " << std::string(it_start, it_end) << std::endl;
}

void Printer(Grid& grid)
{
    for (auto& row : grid)
    {
        for (auto& ele : row)
        {
            std::cout << ele;
        }
        std::cout << std::endl;
    }
}

void Printer(Point2D& p)
{
    std::cout << "(" << p.x << " " << p.y << ")" << std::endl;
}

Grid ParseInput(std::ifstream& file)
{
    std::string input;

    Grid grid;
    while (getline(file, input))
    {
        std::vector<char> row;
        for (auto& c : input)
        {
            row.push_back(c);
        }
        grid.push_back(row);
    }

    return grid;
}

using namespace std;

// This function searches for the given word
// in all 8 directions from the coordinate.
int search2D(vector<vector<char>> grid, int row, int col, string word) {
    int m = grid.size();
    int n = grid[0].size();

    // return false if the given coordinate
    // does not match with first index char.
    if (grid[row][col] != word[0])
        return false;

    int len = word.size();

    // x and y are used to set the direction in which
    // word needs to be searched.
    vector<int>x = { -1, -1, -1, 0, 0, 1, 1, 1 };
    vector<int>y = { -1, 0, 1, -1, 1, -1, 0, 1 };

    // This loop will search in all the 8 directions
    // one by one. It will return true if one of the 
    // directions contain the word.
    int count{};
    for (int dir = 0; dir < 8; dir++) {

        // Initialize starting point for current direction
        int k, currX = row + x[dir], currY = col + y[dir];

        // First character is already checked, match remaining
        // characters
        for (k = 1; k < len; k++) {

            // break if out of bounds
            if (currX >= m || currX < 0 || currY >= n || currY < 0)
                break;

            if (grid[currX][currY] != word[k])
                break;

            //  Moving in particular direction
            currX += x[dir], currY += y[dir];
        }

        // If all character matched, then value of must
        // be equal to length of word
        if (k == len)
            ++count;
    }

    // if word is not found in any direction,
    // then return false
    return count;
}

// This function calls search2D for each coordinate
int searchWord(vector<vector<char>>grid, string word) {
    int m = grid.size();
    int n = grid[0].size();

    int count{};

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {

            // if the word is found from this coordinate,
            // then append it to result.
            count += search2D(grid, i, j, word);

        }
    }

    return count;
}

void partTwo()
{
    std::ifstream file("day4.txt");
    Grid grid = ParseInput(file);
    size_t N = grid.size();
    size_t M = grid.size();

    int count{};
    int checksum = 'M' + 'A' + 'S';
    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 1; j < M - 1; j++)
        {
            if (grid[i][j] == 'A')
            {
                int dig1 = grid[i - 1][j - 1] + 'A' + grid[i + 1][j + 1];
                int dig2 = grid[i + 1][j - 1] + 'A' + grid[i - 1][j + 1];
                if (dig1 == checksum && dig2 == checksum)
                    ++count;
            }
        }
    }

    std::cout << "Answer: " << count << std::endl;
}

void partOne()
{
    std::ifstream file("day4.txt");

    Grid grid = ParseInput(file);
    // Printer(grid);

    int count = searchWord(grid, "XMAS");

    std::cout << "Answer: " << count << std::endl;
}

int main()
{
    // partOne();
    partTwo();
    return 0;
}