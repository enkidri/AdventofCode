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

void printer(std::vector<int>& vec)
{
    for (auto& val : vec)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl << std::endl;
}

void printer(std::string::const_iterator it_start, std::string::const_iterator it_end)
{
    std::cout << "Printer: " << std::string(it_start, it_end) << std::endl;
}

long long int FindSum(std::string::const_iterator it_start, std::string::const_iterator it_end)
{
    std::regex r("mul\\((\\d+),(\\d+)\\)");
    auto m1 = std::sregex_iterator(it_start, it_end, r);
    auto m2 = std::sregex_iterator();

    // printer(it_start, it_end);

    long long int sum{};
    for (std::sregex_iterator it = m1; it != m2; ++it)
    {
        std::smatch match = *it; // Dereferencing the iterator gives a match
        int p1 = std::stoi(match[1].str());
        int p2 = std::stoi(match[2].str());
        sum += p1 * p2;
    }

    return sum;
}

// void partTwo()
// {
//     std::ifstream file("day3.txt");
//     std::regex r_do("do\\(\\)");
//     std::regex r_dont("don't\\(\\)");
//     std::string full_input;
//     std::string input;

//     long long int sum{};
//     while (getline(file, input))
//     {
//         full_input += input;
//     }

//     std::smatch m_dont;
//     std::smatch m_do;
//     std::string::const_iterator it_start = input.cbegin();
//     std::regex_search(it_start, input.cend(), m_dont, r_dont);
//     sum += FindSum(it_start, m_dont.suffix().first);
//     it_start = m_dont.suffix().first;
//     while (std::regex_search(it_start, input.cend(), m_do, r_do))
//     {
//         // std::cout << m_do.suffix().str() << std::endl;
//         it_start = m_do.suffix().first;
//         if (std::regex_search(it_start, input.cend(), m_dont, r_dont))
//         {
//             // std::cout << "Continue" << std::endl;
//             sum += FindSum(m_do.suffix().first, m_dont.suffix().first);
//         }
//         else
//         {
//             // std::cout << "About to break." << std::endl;
//             sum += FindSum(m_do.suffix().first, input.cend());
//             break;
//         }
//         it_start = m_dont.suffix().first;
//     }

//     std::cout << "Answer: " << sum << std::endl;
// }

void partTwo()
{
    std::ifstream file("day3.txt");
    std::regex r_do("do\\(\\)");
    std::regex r_dont("don't\\(\\)");
    std::string full_input;
    std::string input;

    long long int sum{};
    while (getline(file, input))
    {
        full_input += input;
    }

    std::smatch m_dont;
    std::smatch m_do;
    std::string::const_iterator it_start = full_input.cbegin();
    std::regex_search(it_start, full_input.cend(), m_dont, r_dont);
    sum += FindSum(it_start, m_dont.suffix().first);
    it_start = m_dont.suffix().first;
    while (std::regex_search(it_start, full_input.cend(), m_do, r_do))
    {
        // std::cout << m_do.suffix().str() << std::endl;
        it_start = m_do.suffix().first;
        if (std::regex_search(it_start, full_input.cend(), m_dont, r_dont))
        {
            // std::cout << "Continue" << std::endl;
            sum += FindSum(m_do.suffix().first, m_dont.suffix().first);
        }
        else
        {
            // std::cout << "About to break." << std::endl;
            sum += FindSum(m_do.suffix().first, full_input.cend());
            break;
        }
        it_start = m_dont.suffix().first;
    }

    std::cout << "Answer: " << sum << std::endl;
}



void partOne()
{
    std::ifstream file("day3.txt");
    std::regex r("mul\\((\\d+),(\\d+)\\)");
    std::string input;

    int sum{};
    while (getline(file, input))
    {
        auto m1 = std::sregex_iterator(input.begin(), input.end(), r);
        auto m2 = std::sregex_iterator();

        for (std::sregex_iterator it = m1; it != m2; ++it)
        {
            std::smatch match = *it; // Dereferencing the iterator gives a match
            int p1 = std::stoi(match[1].str());
            int p2 = std::stoi(match[2].str());
            sum += p1 * p2;
        }
    }

    std::cout << "Answer: " << sum << std::endl;
}

int main()
{
    // partOne();
    partTwo();
    return 0;
}