#include <math.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using Grid = std::vector<std::vector<int>>;
using namespace std;

struct Index2D {
    int i;
    int j;

    bool operator==(Index2D other) const { return i == other.i && j == other.j; }

    bool operator!=(Index2D other) const { return !(*this == other); }

    friend ostream& operator<<(ostream& os, const Index2D& other) {
        os << "(" << other.i << " " << other.j << ")";
        return os;
    }
};

std::vector<int> StringToVector(std::string& str) {
    std::stringstream ss(str);
    std::vector<int> vec;
    int val;
    while (ss >> val) {
        vec.push_back(val);
    }

    return vec;
}

void Printer(std::vector<int>& vec) {
    for (auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void Printer(std::string::const_iterator it_start,
    std::string::const_iterator it_end) {
    std::cout << "Printer: " << std::string(it_start, it_end) << std::endl;
}

void Printer(Grid& grid) {
    for (auto& row : grid) {
        for (auto& ele : row) {
            std::cout << ele << " ";
        }
        std::cout << std::endl;
    }
}

void partTwo() {

}


bool findExpressionLeftToRight(vector<long long>& nums, int index, long long current, long long& target) {
    if (index == nums.size()) {
        // Base case: if we've used all numbers, check if the current result matches the target
        if (current == target) {
            return true;
        }
        return false;
    }

    // Try adding the next number (left-to-right)
    if (findExpressionLeftToRight(nums, index + 1, current + nums[index], target)) {
        return true;
    }

    // Try multiplying the next number (left-to-right)
    if (findExpressionLeftToRight(nums, index + 1, current * nums[index], target)) {
        return true;
    }

    // Try concatinating the next number (left-to-right)
    if (findExpressionLeftToRight(nums, index + 1, stoll(to_string(current) +
        to_string(nums[index])), target)) {
        return true;
    }

    return false; // If neither addition nor multiplication works
}

void partOne() {
    std::ifstream file("day7.txt");
    std::regex numberRegex(R"(\d+)");
    string input{};

    long long count{};
    while (getline(file, input))
    {
        // Regular expression to match numbers
        std::sregex_iterator it(input.begin(), input.end(), numberRegex);
        std::sregex_iterator end;

        long long target = stoll(it->str());
        std::vector<long long> numbers;
        std::transform(++it, end, std::back_inserter(numbers),
            [](const std::smatch& match) {
                return std::stoi(match.str()); // Convert to integer and insert into vector
            });
        // cout << result << " ";
        // Printer(numbers);
        if (findExpressionLeftToRight(numbers, 1, numbers[0], target))
        {
            count += target;
        }
    }

    cout << "Answer: " << count << endl;

}

int main() {
    partOne();
    // partTwo();
    return 0;
}