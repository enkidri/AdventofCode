#include <math.h>

#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

struct Point2D {
  int i;
  int j;

  bool operator<(const Point2D& other) const {
    if (i != other.i) return i < other.i;
    return j < other.j;
  }
};

template <class T>
struct Grid {
  using value_type = T;

  void addRow(vector<value_type>& row) { grid.push_back(row); }

  void print() {
    for (auto& row : grid) {
      for (value_type ele : row) {
        cout << ele << " ";
      }
      cout << endl;
    }
  }

  value_type get(size_t row, size_t col) {
    size_t N = grid.size();
    size_t M = grid[0].size();
    if (row >= 0 && row < N && col >= 0 && col < M) {
      return grid.at(row).at(col);
    }

    return 'o';
  }

  void set(size_t row, size_t col, value_type val) {
    size_t N = grid.size();
    size_t M = grid[0].size();
    if (row >= 0 && row < N && col >= 0 && col < M) {
      grid.at(row).at(col) = val;
    }
  }

  void update(vector<vector<value_type>>& new_grid) { grid = new_grid; }

  vector<vector<value_type>> grid;
};

void partTwo(const string& file_path) {
  ifstream file(file_path);
  string input;
  set<Point2D> removed_positions;

  // Parse file to grid
  Grid<char> current_grid;
  while (file >> input) {
    vector<char> row;
    for (char c : input) {
      row.push_back(c);
    }
    current_grid.addRow(row);
  }

  // The vector to keep track of the next roll
  set<Point2D> v;

  // All possible 8 orientation
  vector<Point2D> adj_pos{{-1, -1}, {-1, 0}, {-1, 1}, {0, 1},
                          {1, 1},   {1, 0},  {1, -1}, {0, -1}};

  // Create grid that will be updated
  Grid<char> grid_update{current_grid};

  // Get the initial state
  int N = current_grid.grid.size();
  int M = current_grid.grid[0].size();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      char current_val = current_grid.get(i, j);
      if (current_val == '@') {
        // Check the adjecent positions and count;
        vector<Point2D> next_rolls;
        for (auto& pos : adj_pos) {
          Point2D to_check{i + pos.i, j + pos.j};
          char v = current_grid.get(to_check.i, to_check.j);
          if (v == '@') {
            next_rolls.push_back(to_check);
          }
        }

        // If num adjecent rolls are <4 then count
        if (next_rolls.size() < 4) {
          // Add a roll to track at the next grid state
          for (auto& pos : next_rolls) {
            v.insert(pos);
          }

          // Update the grid with new state
          grid_update.set(i, j, 'x');

          // Count
          removed_positions.insert(Point2D{i, j});
        }
      }
    }
  }

  // Update the current grid
  current_grid = grid_update;

  while (!v.empty()) {
    set<Point2D> v_next;
    for (const Point2D& current : v) {
      // Vector to store adjecent cells
      set<Point2D> next_rolls;

      // Check the adjecent cells if removable
      for (auto& pos : adj_pos) {
        Point2D to_check{current.i + pos.i, current.j + pos.j};
        char v = current_grid.get(to_check.i, to_check.j);
        if (v == '@') {
          next_rolls.insert(to_check);
        }
      }

      // If num adjecent rolls are <4 then count
      if (next_rolls.size() < 4) {
        // Add a roll to track at the next grid state
        for (const Point2D& val : next_rolls) {
          v_next.insert(val);
        }

        // Update the grid with new state
        grid_update.set(current.i, current.j, 'x');

        // Count
        removed_positions.insert(current);
      }
    }

    v = v_next;
    current_grid = grid_update;
  }
  // current_grid.print();

  int num_accessable_rolls = removed_positions.size();
  cout << "PART2: " << num_accessable_rolls << endl;
}

void partOne(const string& file_path) {
  ifstream file(file_path);
  string input;
  int num_accessable_rolls = 0;

  // Parse file to grid
  Grid<char> grid;
  while (file >> input) {
    vector<char> row;
    for (char c : input) {
      row.push_back(c);
    }
    grid.addRow(row);
  }

  // Do actual work
  vector<Point2D> adj_pos{{-1, -1}, {-1, 0}, {-1, 1}, {0, 1},
                          {1, 1},   {1, 0},  {1, -1}, {0, -1}};
  int N = grid.grid.size();
  int M = grid.grid[0].size();
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      char current_val = grid.get(i, j);
      if (current_val == '@') {
        // Check the adjecent positions and count;
        int rolls_count = 0;
        for (auto& pos : adj_pos) {
          Point2D to_check{i + pos.i, j + pos.j};
          char v = grid.get(to_check.i, to_check.j);
          if (v == '@') {
            rolls_count++;
          }
        }

        // If num adjecent rolls are <4 then count
        if (rolls_count < 4) {
          num_accessable_rolls++;
        }
      }
    }
  }

  cout << "PART1: " << num_accessable_rolls << endl;
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
