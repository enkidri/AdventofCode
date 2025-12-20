#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void partTwo()
{
    ifstream f{"day2_input.txt"};
    int forward{};
    int depth{};
    int aim{};
    string command{};
    int dist{};
    if (f.is_open())
    {
        while(f >> command >> dist)
        {
            if (command == "forward")
            {
                forward += dist;
                depth += dist*aim;
            }
            else if(command == "down")
            {
                aim += dist;
            }
            else if(command == "up")
            {
                aim -= dist;
            }
        }
    }

    cout << "The position of the submarine is\nHorizontal: " << forward << "\n" <<
              "Depth: " << depth << endl;

    cout << "The product is: " << depth*forward << endl;
}

int partOne()
{
    ifstream f{"day2_input.txt"};
    int forward{};
    int depth{};
    string command{};
    int dist{};
    if (f.is_open())
    {
        while(f >> command >> dist)
        {
            if (command == "forward")
            {
                forward += dist;
            }
            else if(command == "down")
            {
                depth += dist;
            }
            else if(command == "up")
            {
                depth -= dist;
            }
        }
    }

    cout << "The position of the submarine is\nHorizontal: " << forward << "\n" <<
              "Depth: " << depth << endl;

    cout << "The product is: " << depth*forward << endl;
}

int main()
{
    partOne();
    partTwo();
    return 0;
}
