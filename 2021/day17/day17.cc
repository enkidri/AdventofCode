#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <regex>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <queue>
#include <set>
#include <numeric>
#include <functional>
#include <math.h>
using namespace std;

//Solved using the fact that distance can be described as an arithmetic sum

vector<int> getHorizontalRange(int target[2])
{
    vector<int> ret;
    int nr = target[0];
    while (nr <= target[1])
    {
        float r = (-0.5) + sqrt((0.25)+2*nr);
        if (int(r) == r)
        {
            ret.push_back(int(r));
        }
        ++nr;
    }
return ret;
}

vector<int> getVerticalRange(int target[2])
{
    vector<int> ret;
    int nr = abs(target[1]);
    while (nr <= abs(target[0]))
    {
        float r = (-0.5) + sqrt((0.25)+2*(nr+1));
        if (int(r) == r)
        {
            ret.push_back(nr);
        }
        ++nr;
    }
return ret;
}

bool isValid(int x, int y)
{
    int distx{x};
    int disty{y};
    while (distx <= 303 && disty >= -91)
    {
        if (distx >= 244 && disty <= -54)
            return true;
        if (x> 0)
        {
            --x;
            distx += x;  
        }
        --y;
        disty += y;
    }
    return false;
}

void partTwo()
{
    int target_x[2] = {244, 303};
    int target_y[2] = {-91, -54};
    int max_vel = abs(target_y[0])-1;

    vector<int> horiVal = getHorizontalRange(target_x);
    vector<int> verticalVal = getVerticalRange(target_y);

    int total_combinations{};
    for (int x=horiVal[0]; x <= target_x[1]; ++x)
    {
        for (int y=max_vel; y >= (-max_vel-1); --y)
        {
            if (isValid(x,y))
            {
                ++total_combinations;
            }
        }
    }
    
    cout << "The total combinations is: " << total_combinations << endl; 
}

void partOne()
{
    int target_x[2] = {244, 303};
    int target_y[2] = {-91, -54};

    int max_vel = abs(target_y[0])-1;

    int max_height = max_vel*(1 + max_vel) / 2;

    cout << "The max height is: " << max_height << endl;

}

int main()
{
    partOne();
    partTwo();
    return 0;
}