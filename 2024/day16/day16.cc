#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Index2D {
  int i;
  int j;

  Index2D operator+(const Index2D &other) { return {i + other.i, j + other.j}; }
  bool operator==(const Index2D &other) { return i == other.i && j == other.j; }
  bool operator!=(const Index2D &other) { return !(*this == other); }
};

struct State {
  Index2D idx;
  int dir;
};
struct ElementInfo {
  Index2D idx;
  vector<State> prev;
  int dir;
  int cost;
  int turns;
};

bool operator<(const ElementInfo &left, const ElementInfo &right) {
  return left.cost > right.cost;
}

char &GetVal(vector<string> &grid, const Index2D &idx) {
  return grid[idx.i][idx.j];
}

int &GetVal(vector<vector<int>> &score_grid, const Index2D &idx) {
  return score_grid[idx.i][idx.j];
}

ElementInfo &GetVal(vector<vector<ElementInfo>> &score_grid,
                    const Index2D &idx) {
  return score_grid[idx.i][idx.j];
}

ElementInfo &GetVal(vector<vector<map<int, ElementInfo>>> &score_grid,
                    const Index2D &idx, int dir) {
  return score_grid[idx.i][idx.j][dir];
}

void Print(vector<vector<ElementInfo>> &vec) {
  for (auto &row : vec) {
    for (auto &ele : row) {
      cout << setw(5) << ele.cost << "  ";
    }
    cout << endl;
  }
  cout << endl;
}

void Print(vector<string> &vec) {
  for (auto &row : vec) {
    for (auto &ele : row) {
      cout << ele << "  ";
    }
    cout << endl;
  }
  cout << endl;
}

void PartTwo() {
  ifstream file("../input.txt");
  string input;

  vector<string> grid;
  Index2D p;
  Index2D g;

  int i = 0;
  while (getline(file, input)) {
    grid.push_back(input);
    for (int j = 0; j < input.size(); j++) {
      if (input[j] == 'S') {
        p.i = i;
        p.j = j;
      }
      if (input[j] == 'E') {
        g.i = i;
        g.j = j;
      }
    }
    i++;
  }
  vector<vector<map<int, ElementInfo>>> scores_grid(
      grid.size(),
      vector<map<int, ElementInfo>>(
          grid[0].size(),
          map<int, ElementInfo>({{0, {}}, {1, {}}, {2, {}}, {3, {}}})));

  // Perform BFS with DP
  vector<Index2D> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // 0, 1, 2, 3
  priority_queue<ElementInfo> q;
  q.push(ElementInfo{p, vector<State>{{p, 0}}, 0, 0, 0});
  while (!q.empty()) {
    ElementInfo current = q.top();
    q.pop();

    // Check if arrived at goal
    if (GetVal(grid, current.idx) == 'E')
      continue;

    // Check valid directions
    for (int i = current.dir - 1; i <= current.dir + 1; i++) {
      int idx = ((i % dirs.size()) + dirs.size()) % dirs.size();
      Index2D p_new = current.idx + dirs[idx];
      char ele = GetVal(grid, p_new);
      if (ele == '#')
        continue;

      ElementInfo ele_new;
      if (idx == current.dir) {
        ele_new = ElementInfo{p_new, vector<State>{{current.idx, current.dir}},
                              idx, current.cost + 1, current.turns};
      } else {
        ele_new = ElementInfo{p_new, vector<State>{{current.idx, current.dir}},
                              idx, current.cost + 1001, current.turns + 1};
      }

      if (GetVal(scores_grid, ele_new.idx, ele_new.dir).cost != 0) {
        if (ele_new.cost < GetVal(scores_grid, ele_new.idx, ele_new.dir).cost) {
          GetVal(scores_grid, ele_new.idx, ele_new.dir) = ele_new;
          q.push(ele_new);
        } else if (ele_new.cost ==
                   GetVal(scores_grid, ele_new.idx, ele_new.dir).cost) {
          GetVal(scores_grid, ele_new.idx, ele_new.dir)
              .prev.push_back({current.idx, current.dir});
        } else {
          continue;
        }
      } else {
        GetVal(scores_grid, ele_new.idx, ele_new.dir) = ele_new;
        q.push(ele_new);
      }
    }
  }

  int min_cost = INT32_MAX;
  ElementInfo goal;
  for (auto it = scores_grid[g.i][g.j].begin();
       it != scores_grid[g.i][g.j].end(); it++) {
    if (it->second.cost != 0 && it->second.cost < min_cost) {
      min_cost = it->second.cost;
      goal = it->second;
    }
  }
  cout << "Answer: " << min_cost << endl;
  // Print(scores_grid);

  // Find intersecting point from goal point
  int answer = 0;

  queue<ElementInfo> qu;
  qu.push(goal);
  while (!qu.empty()) {
    ElementInfo current = qu.front();
    qu.pop();
    // cout << current.i << " " << current.j << endl;

    char &c = GetVal(grid, current.idx);

    if (c != 'O') {
      answer++;
      c = 'O';
    }

    if (current.idx == p)
      continue;

    for (auto &prev : current.prev) {
      qu.push(GetVal(scores_grid, prev.idx, prev.dir));
    }
  }

  cout << "Answer: " << answer << endl;
}

void PartOne() {
  ifstream file("../input.txt");
  string input;

  vector<string> grid;
  Index2D p;
  Index2D g;

  int i = 0;
  while (getline(file, input)) {
    grid.push_back(input);
    for (int j = 0; j < input.size(); j++) {
      if (input[j] == 'S') {
        p.i = i;
        p.j = j;
      }
      if (input[j] == 'E') {
        g.i = i;
        g.j = j;
      }
    }
    i++;
  }
  vector<vector<ElementInfo>> scores_grid(
      grid.size(), vector<ElementInfo>(grid[0].size(), ElementInfo{}));

  // Perform BFS with DP
  vector<Index2D> dirs = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // 0, 1, 2, 3
  priority_queue<ElementInfo> q;
  q.push(ElementInfo{p, vector<State>{{p, 0}}, 0, 0});
  while (!q.empty()) {
    ElementInfo current = q.top();
    q.pop();

    // Check if arrived at goal
    if (GetVal(grid, current.idx) == 'E')
      continue;

    // Update scores
    // ElementInfo &grid_score = GetVal(scores_grid, current.idx);
    // if (grid_score.cost != 0) {
    //   if (grid_score.cost > current.cost) {
    //     grid_score.cost = current.cost;
    //     grid_score.prev = current.prev;
    //   } else {
    //     continue;
    //   }
    // } else {
    //   grid_score.cost = current.cost;
    //   grid_score.prev = current.prev;
    // }

    // Check valid directions
    for (int i = current.dir - 1; i <= current.dir + 1; i++) {
      int idx = ((i % dirs.size()) + dirs.size()) % dirs.size();
      Index2D p_new = current.idx + dirs[idx];
      char ele = GetVal(grid, p_new);
      if (ele == '#')
        continue;

      ElementInfo ele_new;
      if (idx == current.dir) {
        ele_new = ElementInfo{p_new, vector<State>{{current.idx, current.dir}},
                              idx, current.cost + 1};
      } else {
        ele_new = ElementInfo{p_new, vector<State>{{current.idx, current.dir}},
                              idx, current.cost + 1001};
      }

      if (GetVal(scores_grid, ele_new.idx).cost != 0) {
        if (ele_new.cost < GetVal(scores_grid, ele_new.idx).cost) {
          GetVal(scores_grid, ele_new.idx) = ele_new;
          q.push(ele_new);
        } else if (ele_new.cost == GetVal(scores_grid, ele_new.idx).cost) {
          GetVal(scores_grid, ele_new.idx)
              .prev.push_back({current.idx, current.dir});
        } else {
          continue;
        }
      } else {
        GetVal(scores_grid, ele_new.idx) = ele_new;
        q.push(ele_new);
      }
    }
  }
  cout << "Answer: " << GetVal(scores_grid, g).cost << endl;
  Print(scores_grid);
}

int main() {
  // PartOne();
  PartTwo();

  return 0;
}
