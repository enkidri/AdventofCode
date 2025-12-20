#include <math.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Point2D {
  int i;
  int j;
};

struct Splitter {
  Point2D coord;
  bool visted = false;
};

struct SplitNode {
  int left_count;
  int right_count;
  SplitNode* left;
  SplitNode* right;
  SplitNode(int left_count, int right_count)
      : left_count(left_count),
        right_count(right_count),
        left(nullptr),
        right(nullptr) {}
};

void partTwo(const string& file_path) {
  std::string text;
  std::fstream file(file_path);

  std::unordered_map<uint64_t, uint64_t> timelines;

  // get the start
  getline(file, text);
  timelines[text.find('S')] = 1;

  // while in the manifold
  while (getline(file, text)) {
    std::unordered_map<uint64_t, uint64_t> newTimelines;

    // check all the timelines
    for (std::pair<uint64_t, uint64_t> const& tl : timelines) {
      // if they hit a splitter, add current timelines to left and right of
      // current.
      if (text[tl.first] == '^') {
        newTimelines[tl.first - 1] += tl.second;
        newTimelines[tl.first + 1] += tl.second;
      } else  // no splitter, current timelines continue on.
      {
        newTimelines[tl.first] += tl.second;
      }
    }
    timelines = newTimelines;
  };
  file.close();

  // count how many timelines there are.

  // accumulate returns the type of its initial value, (b, e, initial value,
  // [](){}); so just a plain 0 will return int. 0ULL returns an unsigned long
  // long. 0.0 for double
  uint64_t num_of_timelines = std::accumulate(
      timelines.begin(), timelines.end(), 0ULL,
      [](uint64_t current_sum, std::pair<uint64_t, uint64_t> const& tl) {
        return current_sum + tl.second;
      });

  /*
  //vs this easy for loop.
  for ( std::pair<int, unsigned __int64> const& tl : timelines) {
          result2 += tl.second;
  }
  */
  // 4019607069 TOO LOW
  cout << "PART2: " << num_of_timelines << endl;
}

// I have been defeated
void partTwoOld(const string& file_path) {
  ifstream file(file_path);
  string input;
  int num_of_timelines = 0;

  // Create grid
  vector<string> grid;

  // Find the first splitter
  int i = 0;
  Point2D first_splitter;
  while (getline(file, input)) {
    grid.push_back(input);
    bool first_found = false;
    for (size_t j = 0; j < input.size(); j++) {
      if (input[j] == '^') {
        first_splitter.i = i;
        first_splitter.j = j;
        first_found = true;
      }
    }
    i++;

    if (first_found) break;
  }

  // Create the rest of the grid
  while (getline(file, input)) {
    grid.push_back(input);
  }
  grid.push_back(string(grid[0].size(), '^'));

  // Create a dimension grid
  vector<vector<int>> num_worlds_grid(grid.size(),
                                      vector<int>(grid[0].size(), 0));
  num_worlds_grid[first_splitter.i][first_splitter.j] = 1;

  // debug print
  // for (auto& row : grid) {
  //   cout << row << endl;
  // }

  // Start splitting
  queue<Splitter> to_visit;
  to_visit.push(Splitter{first_splitter, false});
  while (!to_visit.empty()) {
    Splitter current = to_visit.front();
    int current_num_splits = num_worlds_grid[current.coord.i][current.coord.j];
    to_visit.pop();

    Point2D left_start{current.coord.i, current.coord.j - 1};
    Point2D right_start{current.coord.i, current.coord.j + 1};

    // The next left node
    for (int idx = left_start.i + 2; idx < static_cast<int>(grid.size());
         idx++) {
      char c = grid[idx][left_start.j];
      if (c == '^') {
        to_visit.push(Splitter{Point2D{idx, left_start.j}, false});
        num_worlds_grid[idx][left_start.j] += current_num_splits;
        grid[idx][left_start.j] = 'x';
        break;
      } else if (c == 'x') {
        // This node has already been visited
        num_worlds_grid[idx][left_start.j] += current_num_splits;
        break;
      }
    }

    // The next right node
    for (int idx = right_start.i + 2; idx < static_cast<int>(grid.size());
         idx++) {
      char c = grid[idx][right_start.j];
      if (c == '^') {
        to_visit.push(Splitter{Point2D{idx, right_start.j}, false});
        num_worlds_grid[idx][right_start.j] += current_num_splits;
        grid[idx][right_start.j] = 'x';
        break;
      } else if (c == 'x') {
        // This node has already been visited
        num_worlds_grid[idx][right_start.j] += current_num_splits;
        break;
      }
    }
  }

  // debug print
  for (auto& row : grid) {
    cout << row << endl;
  }

  // debug print
  for (auto& row : num_worlds_grid) {
    for (auto& ele : row) {
      cout << setw(2) << ele << " ";
    }
    cout << endl;
  }

  num_of_timelines += accumulate(num_worlds_grid.back().begin(),
                                 num_worlds_grid.back().end(), 0);
  cout << "PART2: " << num_of_timelines << endl;
}

void partOne(const string& file_path) {
  ifstream file(file_path);
  string input;
  int num_of_splits = 1;

  // Create grid
  vector<string> grid;

  // Find the first splitter
  int i = 0;
  Point2D first_splitter;
  while (getline(file, input)) {
    grid.push_back(input);
    bool first_found = false;
    for (size_t j = 0; j < input.size(); j++) {
      if (input[j] == '^') {
        first_splitter.i = i;
        first_splitter.j = j;
        first_found = true;
      }
    }
    i++;

    if (first_found) break;
  }

  // Create the rest of the grid
  while (getline(file, input)) {
    grid.push_back(input);
  }

  // debug print
  // for (auto& row : grid) {
  //   cout << row << endl;
  // }

  // Start splitting
  queue<Splitter> to_visit;
  to_visit.push(Splitter{first_splitter, false});
  while (!to_visit.empty()) {
    Splitter current = to_visit.front();
    to_visit.pop();

    Point2D left_start{current.coord.i, current.coord.j - 1};
    Point2D right_start{current.coord.i, current.coord.j + 1};

    // The next left node
    for (int idx = left_start.i + 1;
         idx < static_cast<int>(grid.size()) && (left_start.j >= 0); idx++) {
      char c = grid[idx][left_start.j];
      if (c == '^') {
        to_visit.push(Splitter{Point2D{idx, left_start.j}, false});
        num_of_splits++;
        grid[idx][left_start.j] = 'x';
        break;
      } else if (c == 'x') {
        // This node has already been visited
        break;
      }
    }

    // The next right node
    for (int idx = right_start.i + 1;
         idx < static_cast<int>(grid.size()) &&
         (right_start.j < static_cast<int>(grid.at(0).size()));
         idx++) {
      char c = grid[idx][right_start.j];
      if (grid[idx][right_start.j] == '^') {
        to_visit.push(Splitter{Point2D{idx, right_start.j}, false});
        num_of_splits++;
        grid[idx][right_start.j] = 'x';
        break;
      } else if (c == 'x') {
        // This node has already been visited
        break;
      }
    }
  }

  // debug print
  // for (auto& row : grid) {
  //   cout << row << endl;
  // }

  // 1569 TOO LOW
  cout << "PART1: " << num_of_splits << endl;
}

int main() {
  string test_path = "test.txt";
  string my_path = "input.txt";

  cout << "========TEST OUTPUT========" << endl;
  partOne(test_path);
  partTwo(test_path);

  cout << endl;

  cout << "=========MY OUTPUT=========" << endl;
  partOne(my_path);
  partTwo(my_path);

  return 0;
};
