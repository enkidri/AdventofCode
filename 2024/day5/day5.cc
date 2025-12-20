#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <regex>
#include <iterator>
#include <utility>
#include <map>

using Grid = std::vector<std::vector<int>>;
using namespace std;

struct Point2D
{
    size_t x;
    size_t y;
};

std::vector<int> StringToVector(std::string& str)
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

void Printer(std::vector<int>& vec)
{
    for (auto& val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void Printer(std::string::const_iterator it_start, std::string::const_iterator it_end)
{
    std::cout << "Printer: " << std::string(it_start, it_end) << std::endl;
}

void Printer(Grid& grid)
{
    for (auto& row : grid)
    {
        for (auto& ele : row)
        {
            std::cout << ele << " ";
        }
        std::cout << std::endl;
    }
}

void Printer(Point2D& p)
{
    std::cout << "(" << p.x << " " << p.y << ")" << std::endl;
}

map<int, vector<int>> ParseInputToMap(ifstream& file)
{
    regex pattern(R"((\d+)\|(\d+))");
    smatch match;

    string input;
    map<int, vector<int>> m;
    while (getline(file, input))
    {
        if (regex_search(input, match, pattern))
        {
            m[stoi(match[1].str())].push_back(stoi(match[2].str()));
        }
        else if (input.empty())
        {
            break;
        }
    }

    return m;
}

vector<vector<int>> ParseInputToVector(ifstream& file)
{
    string input;
    vector<vector<int>> vec_2D;
    while (getline(file, input))
    {
        // A vector to hold the parsed numbers
        std::vector<int> numbers;

        // Regular expression to match numbers
        std::regex numberRegex(R"(\d+)");
        std::sregex_iterator it(input.begin(), input.end(), numberRegex);
        std::sregex_iterator end;

        // Extract each matched number
        while (it != end) {
            numbers.push_back(std::stoi(it->str())); // Convert to int and add to the vector
            ++it;
        }
        vec_2D.push_back(numbers);
    }

    return vec_2D;
}

void partTwo()
{
    std::ifstream file("day5.txt");
    map<int, vector<int>> m = ParseInputToMap(file);
    vector<vector<int>> page_numbers = ParseInputToVector(file);
    // Printer(page_numbers);
    int count{};
    for (auto& row : page_numbers)
    {
        bool was_swapped_once = false;
        while (true)
        {
            bool is_swapped = false;
            for (auto it = row.rbegin(); it != row.rend(); it++)
            {
                for (auto& val_rule : m[*it])
                {
                    auto it_wrong = std::find(it, row.rend(), val_rule);
                    if (it_wrong != row.rend())
                    {
                        iter_swap(it, it_wrong);
                        is_swapped = true;
                        was_swapped_once = true;
                        break;
                    }
                }
                if (is_swapped)
                    break;
            }

            if (!is_swapped)
                break;
        }

        if (was_swapped_once)
        {
            int middle_val = row[row.size() / 2];
            count += middle_val;
        }
    }

    cout << "Anwer: " << count << endl;
}

void partOne()
{
    std::ifstream file("day5.txt");
    map<int, vector<int>> m = ParseInputToMap(file);
    vector<vector<int>> page_numbers = ParseInputToVector(file);
    // Printer(page_numbers);
    int count{};
    for (auto& row : page_numbers)
    {
        bool is_wrong = false;
        for (auto it = row.rbegin(); it != row.rend(); it++)
        {
            for (auto& val_rule : m[*it])
            {
                if (std::find(it, row.rend(), val_rule) != row.rend())
                {
                    is_wrong = true;
                    it = --row.rend();
                    break;
                }
            }
        }
        if (!is_wrong)
        {
            int middle_val = row[row.size() / 2];
            count += middle_val;
        }
    }

    cout << "Anwer: " << count << endl;
}

int main()
{
    // partOne();
    partTwo();
    return 0;
}