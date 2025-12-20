#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<iterator>
using namespace std;

void partOne()
{
    vector<char> Items = 
    {
    '0',               //Used to start at zero index
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
    };

    string input{};
    ifstream file("day3.txt");

    int sum_priority{};
    while (getline(file, input))
    {
        int l = input.length()/2;
        string s1 = input.substr(0, l);
        string s2 = input.substr(l, l);

        sort(begin(s1), end(s1));
        sort(begin(s2), end(s2));
        string intersect;
        set_intersection(begin(s1), end(s1), begin(s2), end(s2),
                         back_inserter(intersect));
        int priority = distance(Items.begin(), 
                    find(Items.begin(), Items.end(), intersect[0]));
        sum_priority += priority;
    }

    cout << "Part 1: Sum of total priorities is " 
         << sum_priority << endl;

}

void partTwo()
{
    vector<char> Items = 
    {
    '0',  //Used to start at zero index
    'a','b','c','d','e','f','g','h','i','j','k','l','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z',
    'A','B','C','D','E','F','G','H','I','J','K','L','M',
    'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
    };

    string input{};
    ifstream file("day3.txt");

    int sum_priority{};
    vector<string> group;
    int idx{};
    while (getline(file, input))
    {
        group.push_back(input);
        idx += 1;
        if (!(idx % 3 == 0))      //Keep pushing until
        {                                       //third iteration
            continue;
        }

        string Elf1 = group[0];
        string Elf2 = group[1];
        string Elf3 = group[2];

        sort(begin(Elf1), end(Elf1));
        sort(begin(Elf2), end(Elf2));
        sort(begin(Elf3), end(Elf3));

        string r1, r2;
        set_intersection(begin(Elf1), end(Elf1), begin(Elf2), end(Elf2),
                         back_inserter(r1));
        set_intersection(begin(r1), end(r1), begin(Elf3), end(Elf3),
                         back_inserter(r2));
        int priority = distance(Items.begin(), 
                    find(Items.begin(), Items.end(), r2[0]));
        sum_priority += priority;
        group.clear();
    }

    cout << "Part 2: Sum of total priorities is " 
         << sum_priority << endl;

}

int main()
{
    partOne();
    partTwo();
    return 0;
}