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

typedef pair<int,int> Position;

vector<string> stringSplit(string& s)
{
    stringstream ss(s);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> vstrings(begin, end);
    
    return vstrings;
}

void partOne()
{   
    string input;
    ifstream file("day9.txt");
    

    //Getting the data
    vector<vector<string>> instructions;
    while (getline(file, input))
    {
        vector<string> instruct = stringSplit(input);
        instructions.push_back(instruct);
    }

    //Actual algorithm
    Position head{0,0};
    Position tail{head};
    set<Position> visited_positions;
    visited_positions.insert(head);

    for (int i=0; i < instructions.size(); i++)
    {
        vector<string> instruct = instructions[i];
        string dir = instruct[0];
        int length = stoi(instruct[1]);

        for (int i=0; i < length; i++)
        {
            Position head_prev = head;
            if (dir == "R")
            {
                head.first++;
            }
            else if (dir == "L")
            {
                head.first--;
            }
            else if (dir == "U")
            {
                head.second++;
            }
            else if (dir == "D")
            {
                head.second--;
            }

            if ((abs(head.first - tail.first) >= 2) || (abs(head.second - tail.second) >= 2))
            {
                tail = head_prev;
                visited_positions.insert(tail);
            }
        }
    }
    
    cout << "Part 1: The number of unique positions visited is " << visited_positions.size() << endl;

}

void moveOne(set<Position>& vp, vector<Position>& knots, string dir)
{
    vector<Position>::iterator head = knots.begin();
    Position head_prev = *head;
    
    if (dir == "R") 
    {
        (*head).first++;
    }
    else if (dir == "L")
    {
        (*head).first--;
    }
    else if (dir == "U")
    {
        (*head).second++;
    }
    else if (dir == "D")
    {
        (*head).second--;
    }


    for (int i=0; i < knots.size() - 1; i++)
    {
        vector<Position>::iterator knot = head + i;
        vector<Position>::iterator tail = (knot + 1);

        if ((abs((*knot).first - (*tail).first) < 2) && (abs((*knot).second - (*tail).second) < 2))
        {
            break;
        } 

        //Up
        if (((((*knot).first - (*tail).first) == 0) && (((*knot).second - (*tail).second) >= 2)))
        {
            (*tail).second++;
        } //Left
        else if (((((*knot).first - (*tail).first) <= -2) && (((*knot).second - (*tail).second) == 0)))
        {
            (*tail).first--;
        } //Right
        else if (((((*knot).first - (*tail).first) >= 2) && (((*knot).second - (*tail).second) == 0)))
        {
            (*tail).first++;
        } //Down
        else if (((((*knot).first - (*tail).first) == 0) && (((*knot).second - (*tail).second) <= -2)))
        {
            (*tail).second--;
        }

        //First quadrant
        if (((((*knot).first - (*tail).first) >= 1) && (((*knot).second - (*tail).second) >= 1)))
        {
            (*tail).first++;
            (*tail).second++;
        } //Second quadrant
        else if (((((*knot).first - (*tail).first) <= -1) && (((*knot).second - (*tail).second) >= 1)))
        {
            (*tail).first--;
            (*tail).second++;
        } //Third quadrant
        else if (((((*knot).first - (*tail).first) <= -1) && (((*knot).second - (*tail).second) <= -1)))
        {
            (*tail).first--;
            (*tail).second--;
        } //Fourth quadrant
        else if (((((*knot).first - (*tail).first) >= 1) && (((*knot).second - (*tail).second) <= -1)))
        {
            (*tail).first++;
            (*tail).second--;
        }

    }
    vp.insert(*(knots.end()-1));
}

void partTwo()
{
    string input;
    ifstream file("day9.txt");

    int total_knots{10};
    Position start{0,0};
    vector<Position> knots(10, start);
    set<Position> visited_positions;
    visited_positions.insert(start);
    while (getline(file, input))
    {
        vector<string> instruct = stringSplit(input);
        string dir = instruct[0];
        int len = stoi(instruct[1]);

        for (int i=0; i < len; i++)
        {
            moveOne(visited_positions, knots, dir);
        }
    }

    cout << "Part 2: The unique positions passed is: " << visited_positions.size() << endl;
}

int main()
{
    partOne();
    partTwo();
    return 0;
}
