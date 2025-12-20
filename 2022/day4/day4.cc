#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<iterator>
#include<numeric>
#include<regex>
using namespace std;


// NOTE: Yea I know this solution is a bit of an 
// overkill since I really only need to do conditional checks
// on the allowed ranges but I'm lazy.

void vecPrint(vector<int>& v)
{
    for (auto& e:v)
    {
        cout << e << " ";
    }
    cout << endl;
}

void partOne()
{
    string input{};
    ifstream file("day4.txt");

    int subsets{};
    while (getline(file, input))
    {
        //Extracts the numbers from the string
        vector<int> n;
        regex regex(R"(\d+)");
        smatch match;
        while (regex_search(input, match, regex))
        {
            n.push_back(stoi(match.str()));
            input = match.suffix();
        }

        //Elf 1 of the pair
        int r1 = n[0];
        int r2 = n[1];
        vector<int> id1(r2-r1 + 1);
        iota(id1.begin(), id1.end(),r1);

        //Elf 2 of the pair
        int l1 = n[2];
        int l2 = n[3];
        vector<int> id2(l2-l1 + 1);
        iota(id2.begin(), id2.end(), l1);

        if ((includes(id1.begin(), id1.end(),
                      id2.begin(), id2.end()) ||
             includes(id2.begin(), id2.end(),
                      id1.begin(), id1.end())))
        {
            subsets += 1;
        }
    }

    cout << "Part 1: There are " << subsets << " numbers of pairs overlapping." << endl;
}

void partTwo()
{
    string input{};
    ifstream file("day4.txt");

    int overlaps{};
    while (getline(file, input))
    {
        //Extracts the numbers from the string
        vector<int> n;
        regex regex(R"(\d+)");
        smatch match;
        while (regex_search(input, match, regex))
        {
            n.push_back(stoi(match.str()));
            input = match.suffix();
        }

        //Elf 1 of the pair
        int r1 = n[0];
        int r2 = n[1];
        vector<int> id1(r2-r1 + 1);
        iota(id1.begin(), id1.end(),r1);

        //Elf 2 of the pair
        int l1 = n[2];
        int l2 = n[3];
        vector<int> id2(l2-l1 + 1);
        iota(id2.begin(), id2.end(), l1);

        vector<int> intersects;
        set_intersection(id1.begin(), id1.end(), id2.begin(), id2.end(), back_inserter(intersects));

        if (!intersects.empty())
        {
            overlaps += 1;
        }
    }

    cout << "Part 2: There are " << overlaps << " numbers of pairs intersecting." << endl;
}

int main()
{
    partOne();
    partTwo();
    return 0;
}