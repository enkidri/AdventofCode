#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
using namespace std;

//Only valid for part 2

int main()
{
    int data{};
    vector<int> vdata{};
    ifstream s{"input.txt"};
    if(s.is_open())
    {
        while(s >> data)
        {
            vdata.push_back(data);
        }
    }

    int inc{};
    for (int i=0; i < vdata.size(); ++i)
    {
        int A = accumulate(vdata.begin()+i, vdata.begin()+i+3, 0);
        int B = accumulate(vdata.begin()+i+1, vdata.begin()+i+4, 0);

        if (B > A)
        {
            ++inc;
        }

        if (vdata.begin()+i == vdata.end())
        {
            break;
        }
    }

    cout << "There are " << inc << " sums larger than the previous sum." << endl;

    return 0;
}
