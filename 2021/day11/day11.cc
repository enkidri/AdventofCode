#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
using namespace std;


class OctoGrid
{
    public:
        OctoGrid()
        {}

        void addGrid(string const& filename)
        {
            string line;
            ifstream f{filename};
            vector<int> row;
            while (getline(f,line))
            {
                for (auto const& c:line)
                {
                    row.push_back(c - '0');
                }
                grid.push_back(row);
                row.clear();
                ++M;
            }
            N = grid[0].size();
        }

        void step()
        {
            for (int y=0; y < grid.size(); ++y)
            {
                for (int x=0; x < grid[0].size(); ++x)
                {
                    flashFill(x,y);
                }
            }
            resetOcto();
        }

        void flashFill(int x, int y)
        {
            if (x < 0 || x >= M || y < 0 || y >= N) 
                return; 
            if (grid[y][x] == 10)
            {
                return;
            }
            ++grid[y][x];
            if ((1 <= grid[y][x]) && (grid[y][x] <= 9))
                return;

            //Sides
            flashFill(x+1, y);
            flashFill(x-1, y);
            flashFill(x, y+1);
            flashFill(x, y-1);
            //Diagonals
            flashFill(x+1, y+1);
            flashFill(x+1, y-1);
            flashFill(x-1, y-1);
            flashFill(x-1, y+1);
        }

        void resetOcto()
        {
            for (auto& row:grid)
            {
                for(auto& ele:row)
                {
                    if (ele == 10)
                    {
                        ele = 0;
                        ++flashAmount;
                    }
                }
            }
        }

        void print()
        {
            for (auto const& row:grid)
            {
                for (auto const& ele:row)
                {
                    cout << ele << " ";
                }
                cout << endl;
            }
            //cout << M << " " << N << endl;
        }

        int getFlashTotal()
        {
            return flashAmount;
        }

        int getGridSize()
        {
            return M*N;
        }

    private:
        vector<vector<int>> grid;
        int flashAmount{};
        int M{};
        int N{};
};

void partTwo()
{
    OctoGrid m;
    m.addGrid("day11.txt");
    int count{0};
    while (true)
    {
        int flashBefore = m.getFlashTotal();
        m.step();
        int flashAfter = m.getFlashTotal();
        ++count;
        if ((flashAfter - flashBefore) == m.getGridSize())
        {
            break;
        }
    }
    m.print();
    cout << "The synchronized flash starts at " << count << endl;

}

void partOne()
{
    OctoGrid m;
    m.addGrid("day11.txt");
    cout << "=====" << endl;
    for (int i=0; i<100; ++i)
    {
        m.step();
    }
    cout << "The total amount of flashes is " << m.getFlashTotal() << endl;

}

int main()
{
    partTwo();
    return 0;
}