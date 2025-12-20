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
using namespace std;

// TESTINPUT
//     [D]    
// [N] [C]    
// [Z] [M] [P]
//  1   2   3 

vector<stack<char>> SupplyStacks()
{
    stack<char> s1;
    s1.push('Z');
    s1.push('N');
    stack<char> s2;
    s2.push('M');
    s2.push('C');
    s2.push('D');
    stack<char> s3;
    s3.push('P');

    vector<stack<char>> stacks = {s1, s2, s3};

    return stacks;
}

//                 [V]     [C]     [M]
// [V]     [J]     [N]     [H]     [V]
// [R] [F] [N]     [W]     [Z]     [N]
// [H] [R] [D]     [Q] [M] [L]     [B]
// [B] [C] [H] [V] [R] [C] [G]     [R]
// [G] [G] [F] [S] [D] [H] [B] [R] [S]
// [D] [N] [S] [D] [H] [G] [J] [J] [G]
// [W] [J] [L] [J] [S] [P] [F] [S] [L]
//  1   2   3   4   5   6   7   8   9 

vector<stack<char>> SupplyStacksForReal()
{
    stack<char> s1;
    s1.push('W');
    s1.push('D');
    s1.push('G');
    s1.push('B');
    s1.push('H');
    s1.push('R');
    s1.push('V');
    stack<char> s2;
    s2.push('J');
    s2.push('N');
    s2.push('G');
    s2.push('C');
    s2.push('R');
    s2.push('F');
    stack<char> s3;
    s3.push('L');
    s3.push('S');
    s3.push('F');
    s3.push('H');
    s3.push('D');
    s3.push('N');
    s3.push('J');
    stack<char> s4;
    s4.push('J');
    s4.push('D');
    s4.push('S');
    s4.push('V');
    stack<char> s5;
    s5.push('S');
    s5.push('H');
    s5.push('D');
    s5.push('R');
    s5.push('Q');
    s5.push('W');
    s5.push('N');
    s5.push('V');
    stack<char> s6;
    s6.push('P');
    s6.push('G');
    s6.push('H');
    s6.push('C');
    s6.push('M');
    stack<char> s7;
    s7.push('F');
    s7.push('J');
    s7.push('B');
    s7.push('G');
    s7.push('L');
    s7.push('Z');
    s7.push('H');
    s7.push('C');
    stack<char> s8;
    s8.push('S');
    s8.push('J');
    s8.push('R');
    stack<char> s9;
    s9.push('L');
    s9.push('G');
    s9.push('S');
    s9.push('R');
    s9.push('B');
    s9.push('N');
    s9.push('V');
    s9.push('M');

    vector<stack<char>> stacks = {s1, s2, s3, s4, s5, s6, s7, s8, s9};

    return stacks;
}

string getTopCrates(vector<stack<char>>& v)
{
    string str{};
    for (auto& stack:v)
    {
        str.append(1, stack.top());
    }

    return str;
}

void partOne()
{
    string input{};
    ifstream file("day5.txt");

    const regex regex("move (\\d+) from (\\d+) to (\\d+)");
    smatch match;

    vector<stack<char>> stacks = SupplyStacksForReal();

    int count = 0;
    while(getline(file, input))
    {
        regex_search(input, match, regex);

        int move_count = stoi(match[1].str());
        int move_from = stoi(match[2].str()) - 1;
        int move_to = stoi(match[3].str()) - 1;

        for (int i=0; i < move_count; i++)
        {
            char top_crate = stacks[move_from].top();
            stacks[move_to].push(top_crate);
            stacks[move_from].pop();
        }
    }

    cout << "Part 1: The top crates are " << getTopCrates(stacks) << endl;
}

void partTwo()
{
    string input{};
    ifstream file("day5.txt");

    const regex regex("move (\\d+) from (\\d+) to (\\d+)");
    smatch match;

    vector<stack<char>> stacks = SupplyStacksForReal();

    int count = 0;
    while(getline(file, input))
    {
        regex_search(input, match, regex);

        int move_count = stoi(match[1].str());
        int move_from = stoi(match[2].str()) - 1;
        int move_to = stoi(match[3].str()) - 1;

        stack<char> moving_crates;

        for (int i=0; i < move_count; i++)
        {
            char top_crate = stacks[move_from].top();
            moving_crates.push(top_crate);
            stacks[move_from].pop();
        }

        while (!moving_crates.empty())
        {
            char to_move = moving_crates.top();
            stacks[move_to].push(to_move);
            moving_crates.pop();
        }
    }

    cout << "Part 2: The top crates are " << getTopCrates(stacks) << endl;
}

int main()
{
    partOne();
    partTwo();
    return 0;
}
