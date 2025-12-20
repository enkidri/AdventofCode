#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
using namespace std;

void partOne()
{
    string line;
    ifstream file("day2.txt");

    vector<char> O = {'O', 'A', 'B', 'C'};
    vector<char> U = {'O', 'X', 'Y', 'Z'};

    int score{};
    while (getline(file, line))
    {
        int opponent_idx = distance(O.begin(), find(O.begin(), O.end(), line[0]));
        int my_idx = distance(U.begin(), find(U.begin(), U.end(), line[2]));
        
        int res = my_idx - 1;
        if ((res == 0 && opponent_idx == 3) || (res == opponent_idx))       //Winnig score
        {
            score += 6;
        }
        if (opponent_idx == my_idx)                                         //Drawing score
        {
            score += 3;
        }
        score += my_idx;
    }

    cout << "Part 1: Total score is " << score << endl;
}

void partTwo()
{
    string line;
    ifstream file("day2.txt");

    vector<char> O = {'O', 'A', 'B', 'C'};
    vector<char> U = {'O', 'X', 'Y', 'Z'};

    int score{};
    int win_points{6};
    int draw_points{3};
    while (getline(file, line))
    {
        int opponent_idx = distance(O.begin(), find(O.begin(), O.end(), line[0]));
        int my_idx{};
        
        if (line[2] == 'X')
        {
            if (opponent_idx != 1)
            {
                my_idx = opponent_idx - 1;
            }
            else
            {
                my_idx = 3;
            }
            score += my_idx;
        }
        else if (line[2] == 'Y')
        {
            my_idx = opponent_idx;
            score += my_idx + draw_points;
        }
        else if (line[2] == 'Z')
        {
            if (opponent_idx != 3)
            {
                my_idx = opponent_idx + 1;
            }
            else
            {
                my_idx = 1;
            }
            score += my_idx + win_points;
        }
    }
    cout << "Part 2: Total total amount of points are " << score << endl;
}

int main()
{
    //partOne();
    partTwo();
    return 0;
}