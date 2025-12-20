#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

string oxygen(vector<string> bins)
{
    int i{};
    double rate{};
    int line_size{};
    vector<string> temp{};
    while(bins.size() > 1)
    {
        rate = 0;
        line_size = 0;
        for (auto bin:bins)
        {
            if (bin[i] == '1')
            {
                rate += 1.0;
            }
            ++line_size;
        }
        for (auto bin:bins)
        {
            if (bin[i] == '1' && (rate/line_size >= 0.5))
            {
                temp.push_back(bin);
            }
            else if(bin[i] == '0' && (rate/line_size < 0.5))
            {
                temp.push_back(bin);
            }
        }
        ++i;
        bins = temp;
        temp.clear();
    }
    return bins[0];
}

string CO_2(vector<string> bins)
{
    int i{};
    double rate{};
    int line_size{};
    vector<string> temp{};
    while(bins.size() > 1)
    {
        rate = 0;
        line_size = 0;
        for (auto bin:bins)
        {
            if (bin[i] == '1')
            {
                rate += 1.0;
            }
            ++line_size;
        }
        for (auto bin:bins)
        {
            if (bin[i] == '1' && (rate/line_size < 0.5))
            {
                temp.push_back(bin);
            }
            else if(bin[i] == '0' && (rate/line_size >= 0.5))
            {
                temp.push_back(bin);
            }
        }
        ++i;
        bins = temp;
        temp.clear();
    }
    return bins[0];
}

int bin_to_dec(string const& str)
{
    string r_str = string(str.rbegin(), str.rend());
    int i{};
    int dec{};
    for (auto l:r_str)
    {
        if (l == '1')
        {
            dec += pow(2,i);
        }
        ++i;
    }
    return dec;
}

void partTwo()
{
    ifstream f{"day3_input.txt"};
    string line;
    vector<string> bins{};
    
    while (f >> line)
    {
        bins.push_back(line);
    }

    string oxy{oxygen(bins)};
    string CO2{CO_2(bins)};

    cout << "Oxygen rating: " << oxy << "\n" <<
            "CO 2 rating: " << CO2 << endl;

    int prod{bin_to_dec(oxy) * bin_to_dec(CO2)};

    cout << "Solution: " << prod << endl;
}

void partOne()
{
    ifstream f{"day3_input.txt",};
    string line{};
    int total_lines{};
    vector<double> rate_prob(12);
    int i{};

    vector<string> file{};
    if (f.is_open())
    {
        while(f >> line)
        {
            file.push_back(line);
            ++total_lines;
        }
    }

    int y{};
    for (int i=0; i < file.size(); ++i)
    {
        line = file[i];
        y = 0;
        for (char const& c:line)
        {
            if (c == '1')
            {
                rate_prob.at(y) += 1.0;
            }
            ++y;
        }
    }

    transform(rate_prob.begin(), rate_prob.end(), rate_prob.begin(),
            [&total_lines](double &c)
            {
                return c/total_lines;
            });

    string gamma{};
    string epsilon{};
    for (auto prob:rate_prob)
    {
        cout << prob << endl;
        if (prob>0.5)
        {
            gamma += '1';
            epsilon += '0';
        }
        else
        {
            gamma += '0';
            epsilon += '1';
        }
    }

    cout << "Gamma: " << gamma << "\n" <<
            "Epsilon: " << epsilon << endl;
}

int main()
{
    partOne();
    partTwo();
    
    return 0;
}
