#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct Index2D {
  int i;
  int j;
};

struct Height {
  int height = 0;
  Index2D previous;
  Index2D current;
};

using Grid = vector<vector<Height>>;

ostream& operator<<(ostream& os, const Index2D& index) {
  os << "(" << index.i << ", " << index.j << ")";
  return os;
}

bool operator==(const Index2D& index1, const Index2D& index2) {
  return index1.i == index2.i && index1.j == index2.j;
}

bool operator==(const Height& index1, const Height& index2) {
  return index1.current == index2.current;
}

bool ValidDirection(Grid& grid, Index2D& idx0, Index2D& idx1) {
  int N = grid.size();
  int M = grid[0].size();
  if (idx1.j >= 0 && idx1.i >= 0 && idx1.i < M && idx1.j < N) {
    int h0 = grid[idx0.j][idx0.i].height;
    int h1 = grid[idx1.j][idx1.i].height;
    if (h0 + 1 == h1) {
      return true;
    }
  }

  return false;
}

int AddToMem(Grid& grid, Grid& mem, Index2D trail_head) {
  stack<Index2D> visited_locations;
  vector<Height> goal_locations;
  visited_locations.push(trail_head);
  // Right, down, left, up
  vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  while (!visited_locations.empty()) {
    // Start from top, go to bottom. Add previous to mem. Increment if bottom
    //  found.
    Index2D current_location = visited_locations.top();
    cout << "Current location: " << current_location << endl;
    visited_locations.pop();
    if (grid[current_location.j][current_location.i].height == 9) {
      Height goal{};
      goal.current = current_location;
      if (find(goal_locations.begin(), goal_locations.end(), goal) ==
          goal_locations.end()) {
        goal_locations.push_back(goal);
      }
      continue;
    }
    // else if (mem[current_location.j][current_location.i].height != 0) {
    //   Height goal = {mem[current_location.j][current_location.i].height};
    //   goal.current = current_location;
    //   if (find(goal_locations.begin(), goal_locations.end(), goal) ==
    //       goal_locations.end()) {
    //     goal_locations.push_back(goal);
    //   }
    //   continue;
    // }

    for (auto& dir : directions) {
      Index2D next_location = {current_location.i + dir.first,
                               current_location.j + dir.second};
      if (ValidDirection(grid, current_location, next_location)) {
        visited_locations.push(next_location);
        grid[next_location.j][next_location.i].previous = current_location;
        cout << "Next location: " << next_location << endl;
      }
    }
  }

  // Mark visited path to trail head
  for (auto& trail_head : goal_locations) {
    Index2D current_idx = trail_head.current;
    int current_height = grid[current_idx.j][current_idx.i].height;
    while (true) {
      if (trail_head.height) {
        mem[current_idx.j][current_idx.i].height += trail_head.height;
      } else {
        mem[current_idx.j][current_idx.i].height += 1;
      }
      if (current_height == 0) {
        break;
      }

      // Update iteration information
      current_idx = grid[current_idx.j][current_idx.i].previous;
      current_height = grid[current_idx.j][current_idx.i].height;
    }
  }
  return 0;
}
void Printer(Grid& grid) {
  for (auto& row : grid) {
    for (auto& ele : row) {
      cout << ele.height << "  ";
    }
    cout << endl;
  }
  cout << endl;
}
void partOne(char* file_path) {
  ifstream file(file_path);
  string input;

  // Parse the input
  Grid grid;
  vector<Index2D> trail_heads;
  vector<Index2D> tops;
  int j = 0;
  while (getline(file, input)) {
    vector<Height> row;
    for (int i = 0; i < input.size(); i++) {
      if (input[i] == '0') {
        trail_heads.push_back(Index2D{i, j});
      } else if (input[i] == '9') {
        tops.push_back(Index2D{i, j});
      }
      row.push_back(Height{input[i] - '0'});
    }
    grid.push_back(row);
    j++;
  }

  // Perform the search
  Grid mem(grid.size(), vector<Height>(grid[0].size(), Height{}));
  for (int idx = 0; idx < trail_heads.size(); idx++) {
    // Add previously added locations
    AddToMem(grid, mem, trail_heads[idx]);
  }

  Printer(grid);
  Printer(mem);

  // Calculate the score
  int answer = 0;
  for (auto& trail_head : trail_heads) {
    answer += mem[trail_head.j][trail_head.i].height;
  }

  cout << "Answer: " << answer << endl;
}

int main(int argc, char* argv[]) {
  partOne(argv[1]);

  return 0;
}