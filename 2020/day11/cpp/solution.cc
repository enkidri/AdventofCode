#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Index {
  int i;
  int j;

  Index operator+(const Index &rhs) const { return {i + rhs.i, j + rhs.j}; }
};

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // parse
  vector<string> grid;
  vector<Index> seat_indices;
  int i = 0, j = 0;
  while (getline(file, input)) {
    grid.push_back(input);
    for (char c : input) {
      if (c == 'L')
        seat_indices.push_back({i, j});
      j++;
    }
    i++;
    j = 0;
  }

  vector<Index> offsets = {{-1, -1}, {0, -1}, {1, -1}, {1, 0},
                           {1, 1},   {0, 1},  {-1, 1}, {-1, 0}};

  vector<string> next_grid = grid;
  bool changed = true;
  while (changed) {
    changed = false;
    answer = 0;

    for (const Index &idx : seat_indices) {
      int num_occupied = 0;
      char c = grid[idx.i][idx.j];

      // Update num_occupied
      for (const auto &offset : offsets) {
        Index current_idx = idx + offset;
        while (current_idx.i >= 0 && current_idx.j >= 0 &&
               current_idx.i < grid.size() && current_idx.j < grid[0].size()) {
          if (grid[current_idx.i][current_idx.j] == '#') {
            num_occupied++;
            break;
          } else if (grid[current_idx.i][current_idx.j] == 'L') {
            break;
          } else {
            current_idx = current_idx + offset;
          }
        }
      }

      // Update grid state
      if (c == 'L' && num_occupied == 0) {
        next_grid[idx.i][idx.j] = '#';
        answer++;
        changed = true;
      } else if (c == '#' && num_occupied >= 5) {
        next_grid[idx.i][idx.j] = 'L';
        changed = true;
      } else {
        next_grid[idx.i][idx.j] = c;
        if (c == '#')
          answer++;
      }
    }
    grid = next_grid;
  }

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
}

void partOne(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // parse
  vector<string> grid;
  while (getline(file, input)) {
    grid.push_back(input);
  }

  // solve
  // NOTE: The solution could be improved by:
  //  * Just iterating over occupied seats
  //  * Using a 1D vector for cache locality
  //  * Pad the 1D vector to avoid if statements (avoids branching)
  //  * Use a next_grid & grid swap instead of to_change vector
  vector<Index> neighbours = {{-1, -1}, {0, -1}, {1, -1}, {1, 0},
                              {1, 1},   {0, 1},  {-1, 1}, {-1, 0}};
  auto change_state = [&](char c, const Index &current) {
    uint8_t num_occupied = 0;
    for (const auto &neighbour : neighbours) {
      Index to_check = current + neighbour;
      if (to_check.i < 0 || to_check.j < 0 || to_check.i >= grid.size() ||
          to_check.j >= grid.at(0).size()) {
        continue;
      }

      num_occupied += (grid.at(to_check.i).at(to_check.j) == '#');
    }

    return (c == 'L' && num_occupied == 0) || (c == '#' && num_occupied >= 4);
  };

  vector<Index> to_change;
  to_change.reserve(grid.size() * grid.at(0).size());
  to_change.push_back({}); // Push a dummy value
  while (to_change.size() > 0) {
    to_change.clear();
    answer = 0;
    // Do a iteration
    for (size_t i = 0; i < grid.size(); i++) {
      for (size_t j = 0; j < grid.at(0).size(); j++) {
        char c = grid.at(i).at(j);
        if (c == 'L' || c == '#') {
          answer += grid.at(i).at(j) == '#';
          bool state_changed =
              change_state(c, {static_cast<int>(i), static_cast<int>(j)});
          if (state_changed) {
            to_change.push_back({static_cast<int>(i), static_cast<int>(j)});
          }
        }
      }
    }

    // Update the grid
    for (const auto &pos : to_change) {
      grid[pos.i][pos.j] = grid[pos.i][pos.j] == '#' ? 'L' : '#';
    }
  }

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
}

int main() {
  string test_path = "test.txt";
  string my_path = "input.txt";

  cout << "========PART 1========" << endl;
  cout << "Test: ";
  partOne(test_path);
  cout << "Input: ";
  partOne(my_path);

  cout << endl;

  cout << "=========PART 2=========" << endl;
  cout << "Test: ";
  partTwo(test_path);
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
