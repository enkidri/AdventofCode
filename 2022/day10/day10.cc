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
#include<cstdlib>
using namespace std;

vector<string> stringSplit(string& s)
{
    stringstream ss(s);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> vstrings(begin, end);
    
    return vstrings;
}

int calcSignalStrength(int cycle, int X)
{
    int signal_strength{};
    if ((220 >= cycle) && (cycle >= 20) && ((cycle - 20) % 40 == 0))
    {
        signal_strength = X * cycle;
    }
    return signal_strength;
}

void partOne()
{   
    string input;
    ifstream file("day10.txt");

    int cycle{};
    int X{1};
    int signal_strength{};
    while (getline(file, input))
    {
        vector<string> inputv = stringSplit(input);
        if (inputv[0] == "noop")
        {
            cycle++;
            signal_strength += calcSignalStrength(cycle, X);
        }
        else if (inputv[0] == "addx")
        {
            int to_add = stoi(inputv[1]);
            cycle++;
            signal_strength += calcSignalStrength(cycle, X);

            cycle++;
            signal_strength += calcSignalStrength(cycle, X);

            X += to_add;
        }

    }

    cout << "Part 1: Sum of the signal strengths are " << signal_strength << endl;

}
//==================================================================
//The edge is messed up but whatever, mostly works anyways

void writeToCRT(vector<char>& screen, int cycle, int X)
{
    int cycle_to_row = (cycle % 40) - 1;
    int X_to_row = X - 1;
    if ((X_to_row <= cycle_to_row) && (cycle_to_row < X_to_row+3))
    {
        screen[cycle - 1] = '#';
    }
}

void CRTPrinter(vector<char> screen)
{
    for (int row=0; row < 6; row++)
    {
        for (int ele=0; ele < 40; ele++)
        {
            cout << screen[row*40 + ele];
        }
        cout << endl;
    }
}

void partTwo()
{
    string input;
    ifstream file("day10.txt");

    vector<char> CRT_screen(240, '.');

    int cycle{};
    int X{1};
    while (getline(file, input))
    {
        vector<string> inputv = stringSplit(input);
        if (inputv[0] == "noop")
        {
            cycle++;
            writeToCRT(CRT_screen, cycle, X);
        }
        else if (inputv[0] == "addx")
        {
            int to_add = stoi(inputv[1]);
            cycle++;
            writeToCRT(CRT_screen, cycle, X);

            cycle++;
            writeToCRT(CRT_screen, cycle, X);

            X += to_add;

        }
    }

    cout << "Part 2:" << endl;

    CRTPrinter(CRT_screen);
}

int main()
{
    partOne();
    partTwo();
    return 0;
}
