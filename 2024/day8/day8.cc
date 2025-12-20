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

map<char, vector<Index2D>> ParseInput(ifstream& file) {
  string input;
  map<char, vector<Index2D>> m;
  int i{};
  while (getline(file, input)) {
    for (int j = 0; j < input.size(); j++) {
      if (input[j] != '.') {
        m[input[j]].push_back(Index2D{i, j});
      }
    }
    ++i;
  }

  return m;
}

void partTwo() {}

void partOne() {
  ifstream file("day8.txt");
  map<char, vector<Index2D>> m = ParseInput(file);

  cout << "Answer: " << endl;
}

int main() {
  partOne();
  // partTwo();
  return 0;
}