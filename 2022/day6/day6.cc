#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <numeric>
using namespace std;

//part two only
int main(int argc, char* argv[])
{
    vector<uint64_t> school(9);
    ifstream f{argv[1]};
    string line;

    while(getline(f,line))
    {
        for (auto const& digit:line)
        {
            if (!isdigit(digit))
            {
                continue;
            }
            school[digit - '0'] += 1;
        }
    }

    int days = stoi(argv[2]);
    for (int i=0; i < days; ++i)
    {
        uint64_t newborn = school[0];       //uints has to be used to compute the answer (it is that high!!)
        school.erase(school.begin());
        school.push_back(0);
        school[8] += newborn;
        school[6] += newborn;
    }
    uint64_t zero = 0;
    uint64_t total = accumulate(school.begin(), school.end(), zero);    //zero has to be uint not to convert to int (just in case)

    cout << "The total of lanternfish after " << days <<" days is " << total << endl;
    
    return 0;
}
