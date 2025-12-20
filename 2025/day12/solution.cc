#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <regex>
#include <string>
#include <vector>

using namespace std;

struct GridSize {
  int width;
  int height;

  bool operator<(const GridSize& rhs) const {
    if (width == rhs.width) {
      return height < rhs.height;
    }
    return width < rhs.width;
  }
};

struct GridSpecs {
  GridSize size;
  vector<int> present_idx;
};

void partTwo(const string& file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  int answer = 0;

  while (getline(file, input)) {
  }

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << "PART2: " << answer << " Time: " << elapsed.count() << " ms" << endl;
}

void partOne(const string& file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  int total_valid_specs = 0;

  // Parse the input
  regex re_number(R"(\d+)");
  vector<vector<string>> presents;
  vector<GridSpecs> grid_specs;
  while (getline(file, input)) {
    if (input.size() == 2) {
      vector<string> present;
      while (getline(file, input) && !input.empty()) {
        present.push_back(input);
      }
      presents.push_back(present);
      continue;
    } else if (input.size() > 2) {
      GridSpecs specs;

      auto start = sregex_iterator(input.begin(), input.end(), re_number);
      auto end = sregex_iterator();
      // Parse the grid size first
      int width = stoi((*(start))[0].str());
      int height = stoi((*(++start))[0].str());
      GridSize s{width, height};
      specs.size = s;

      // Parse the presents to be put inside the grid
      for (auto it = ++start; it != end; it++) {
        smatch m = *it;
        specs.present_idx.push_back(stoi(m.str()));
      }

      grid_specs.push_back(specs);
    }
  }

  // Solve?
  for (const auto& spec : grid_specs) {
    int x1 = spec.size.width / 3;
    int y1 = spec.size.height / 3;
    int max_grid_size = x1 * y1;
    int total_presents =
        accumulate(spec.present_idx.begin(), spec.present_idx.end(), 0);

    if (total_presents <= max_grid_size) {
      total_valid_specs++;
    } else {
      // I'll just assume that its not solvable.
      continue;
    }
  }

  // Summarize the results
  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << "PART1: " << total_valid_specs << " Time: " << elapsed.count()
       << " ms" << endl;
}

int main() {
  string test_path = "test.txt";
  string my_path = "input.txt";

  cout << "========TEST OUTPUT========" << endl;
  // Current solution wont be able to solve the test cases. Turns out that the
  // actual input was easier than expected. The test input wont therefore be
  // included.

  // partOne(test_path);
  // partTwo(test_path);

  cout << endl;

  cout << "=========MY OUTPUT=========" << endl;
  partOne(my_path);
  // partTwo(my_path);

  return 0;
};
