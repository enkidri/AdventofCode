#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>

std::vector<int> StringToVector(std::string &str)
{
    std::stringstream ss(str);
    std::vector<int> vec;
    int val;
    while (ss >> val)
    {
        vec.push_back(val);
    }

    return vec;
}

void printer(std::vector<int> &vec)
{
    for (auto &val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

bool IsSafeReport(std::vector<int> &vec)
{
    bool is_increasing = true;
    bool is_decreasing = true;
    bool is_in_range = true;
    for (int i = 1; i < vec.size(); i++)
    {
        int val = vec[i] - vec[i - 1];

        if (std::abs(val) > 3 || std::abs(val) < 1)
            is_in_range = false;

        if (val > 0)
            is_decreasing = false;

        if (val < 0)
            is_increasing = false;
    }

    return is_in_range && (is_decreasing || is_increasing);
}

bool IsSafeReport2(std::vector<int> &vec)
{
    bool is_increasing = true;
    bool is_decreasing = true;
    for (int i = 1; i < vec.size() - 1; i++)
    {
        int val = vec[i] - vec[i + 1];

        if (val > 0)
            is_decreasing = false;

        if (val < 0)
            is_increasing = false;

        if (std::abs(val) > 3 || std::abs(val) < 1 || !(is_decreasing || is_increasing))
        {
            std::vector<int> temp1 = vec;
            std::vector<int> temp2 = vec;
            std::vector<int> temp3 = vec;
            temp3.erase(temp3.begin() + i - 1);
            temp1.erase(temp1.begin() + i);
            temp2.erase(temp2.begin() + i + 1);
            return IsSafeReport(temp1) || IsSafeReport(temp2) || IsSafeReport(temp3);
        }
    }

    return true;
}

void partOne()
{
    std::ifstream file("day2.txt");
    std::string line;
    int safe_reports{};
    while (std::getline(file, line))
    {
        std::vector<int> vec = StringToVector(line);
        if (IsSafeReport2(vec))
        {
            ++safe_reports;
        }
    }

    std::cout << "Answer: " << safe_reports << std::endl;
}

int main()
{
    partOne();

    return 0;
}