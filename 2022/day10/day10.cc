#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <stack>
#include <fstream>
#include <string>
using namespace std;

unsigned long long int scoreCalc(string const& s)
{
    unsigned long long int score{};
    map<char, unsigned long long int> brackets_score{{')', 1}, {']', 2}, {'}', 3}, {'>', 4}};
    for (char const& c:s)
    {
        score *= 5;
        score += brackets_score[c];
    }
    return score;
}

void partTwo()
{
    ifstream f{"day10.txt"};
    string line;
    map<char, char> brackets{{')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}};
    vector<unsigned long long int> scores;

    stack<char> s;
    bool skipLine = false;
    while (getline(f,line))
    {
        s = {};
        for (char const& c:line)
        {
            if ((c == '(') || (c == '[') || (c == '{') || (c == '<'))
            {
                s.push(c);
            }
            else
            {
                if (brackets[c] != s.top())
                {
                    skipLine = true;
                    break;
                }
                else
                {
                    s.pop();
                }
            }
        }
        if (skipLine)
        {
            skipLine = false;
            continue;
        }
        string str = "";
        int s_size = s.size();
        for (int i=0; i < s_size; ++i)
        {
            for (auto const&i:brackets)
            {
                if (i.second == s.top())
                {
                    str += i.first;
                    break;
                }
            }
            s.pop();
        }
        scores.push_back(scoreCalc(str));
    }
    sort(scores.begin(), scores.end(), greater<unsigned long long int>());
    cout << "The middle score is " << scores[((scores.size()-1)/2)];
}

void partOne()
{
    ifstream f{"day10.txt"};
    string line;
    map<char, char> brackets{{')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}};
    map<char, int> brackets_score{{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};

    stack<char> s;
    int syntaxScore{};
    while (getline(f,line))
    {
        for (char const& c:line)
        {
            if ((c == '(') || (c == '[') || (c == '{') || (c == '<'))
            {
                s.push(c);
            }
            else
            {
                if (brackets[c] != s.top())
                {
                    syntaxScore += brackets_score[c];
                    break;
                }
                else
                {
                    s.pop();
                }
            }
        }
    }
    cout << "The current score is " << syntaxScore;
}

int main()
{
    partTwo();
    return 0;
}