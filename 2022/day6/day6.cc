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
using namespace std;


void partOne()
{
    string input{};
    ifstream file("day6.txt");
    getline(file, input);

    int processed_packets{};
    for (int i=0; i < input.length() - 3; i++)
    {
        string s = input.substr(i, 4);
        set<char> marker(s.begin(), s.end());
        if (marker.size() == s.size())
        {
            processed_packets = i + 4;
            break;
        }
        
    }

    cout << "Part 1: The number of packets before start-of-packets is " 
         << processed_packets << endl;
}

void partTwo()
{
    string input{};
    ifstream file("day6.txt");
    getline(file, input);
    int message_start_len = 14;

    int processed_packets{};
    for (int i=0; i < input.length() - message_start_len; i++)
    {
        string s = input.substr(i, message_start_len);
        set<char> marker(s.begin(), s.end());
        if (marker.size() == s.size())
        {
            processed_packets = i + message_start_len;
            break;
        }
        
    }

    cout << "Part 2: The number of packets before start-of-message is " 
         << processed_packets << endl;

}

int main()
{
    partOne();
    partTwo();
    return 0;
}
