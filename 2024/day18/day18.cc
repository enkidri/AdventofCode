#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <vector>

using namespace std;

constexpr int I_MAX = 70;
constexpr int J_MAX = 70;

struct Index2D {
  int i;
  int j;

  Index2D operator+(const Index2D &other) { return {i + other.i, j + other.j}; }
};

ostream &operator<<(ostream &os, const Index2D &other) {
  return os << "(" << other.j << "," << other.i << ")" << endl;
}

struct Grid {
  vector<vector<char>> grid{I_MAX + 1, vector<char>(J_MAX + 1, '.')};
  vector<Index2D> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

  void SetAt(Index2D idx, char value) { grid[idx.i][idx.j] = value; }

  vector<Index2D> GetValidMovesAt(Index2D idx) const {
    vector<Index2D> valid_moves;
    for (auto &dir : dirs) {
      Index2D idx_n = idx + dir;
      if (idx_n.i >= 0 && idx_n.i <= I_MAX && idx_n.j >= 0 &&
          idx_n.j <= J_MAX) {
        if (grid[idx_n.i][idx_n.j] != '#') {
          valid_moves.push_back(idx_n);
        }
      }
    }

    return valid_moves;
  }
};

struct State {
  Index2D idx;
  int score;
};

bool operator<(const State &lhs, const State &rhs) {
  return lhs.score > rhs.score;
}

template <typename T> void Print(const vector<vector<T>> &vec) {
  for (auto &row : vec) {
    for (auto &ele : row) {
      cout << ele << " ";
    }
    cout << endl;
  }

  cout << endl;
}

bool PathIsFound(const Grid &grid) {
  vector<vector<int>> values{I_MAX + 1, vector<int>(J_MAX + 1, -1)};
  priority_queue<State> q;
  q.push({Index2D{0, 0}, 0});
  values[0][0] = 0;
  while (!q.empty()) {
    State current = q.top();
    q.pop();

    if (current.idx.i == I_MAX && current.idx.j == J_MAX) {
      break;
    }

    vector<Index2D> valid_moves = grid.GetValidMovesAt(current.idx);
    for (auto &move : valid_moves) {
      int score_move = values[move.i][move.j];
      if (score_move == -1) {
        values[move.i][move.j] = current.score + 1;
        q.push({move, current.score + 1});
      }
    }
  }
  // Print(values);
  return values[I_MAX][J_MAX] != -1;
}

void PartTwo() {
  ifstream file("../input.txt");
  string input;
  regex re("(\\d+),(\\d+)");
  vector<Index2D> corrupted_bytes;
  Grid grid;

  while (getline(file, input)) {
    smatch match;
    regex_match(input, match, re);
    int d1 = stoi(match[1].str());
    int d2 = stoi(match[2].str());
    corrupted_bytes.push_back({d2, d1});
  }

  // Add simulated bits

  int max_iter = corrupted_bytes.size();
  Index2D blocking_byte{};
  for (int current = 0; current < max_iter; current++) {
    Index2D &idx = corrupted_bytes[current];
    grid.SetAt(idx, '#');
    if (!PathIsFound(grid)) {
      blocking_byte = idx;
      break;
    }
  }
  // Print(grid.grid);

  cout << "Answer: " << blocking_byte << endl;
}

void PartOne() {
  ifstream file("../input.txt");
  string input;
  regex re("(\\d+),(\\d+)");
  vector<Index2D> corrupted_bytes;
  Grid grid;

  while (getline(file, input)) {
    smatch match;
    regex_match(input, match, re);
    int d1 = stoi(match[1].str());
    int d2 = stoi(match[2].str());
    corrupted_bytes.push_back({d2, d1});
  }

  // Add simulated bits

  int max_iter = 1024;
  for (int current = 0; current < max_iter; current++) {
    Index2D &idx = corrupted_bytes[current];
    grid.SetAt(idx, '#');
  }
  // Print(grid.grid);

  vector<vector<int>> values{I_MAX + 1, vector<int>(J_MAX + 1, -1)};
  queue<State> q;
  q.push({Index2D{0, 0}, 0});
  values[0][0] = 0;
  while (!q.empty()) {
    State current = q.front();
    int score_current = current.score;
    q.pop();

    if (current.idx.i == I_MAX && current.idx.j == J_MAX) {
      continue;
    }

    vector<Index2D> valid_moves = grid.GetValidMovesAt(current.idx);
    if (current.idx.i == 2 && current.idx.j == 1) {
      cout << valid_moves.size() << endl;
    }
    for (auto &move : valid_moves) {
      int score_move = values[move.i][move.j];
      if (score_current + 1 < score_move || score_move == -1) {
        values[move.i][move.j] = score_current + 1;
        q.push({move, score_current + 1});
      }
    }
  }
  // Print(values);

  cout << "Answer: " << values[I_MAX][J_MAX] << endl;
}

int main() {
  // PartOne();
  PartTwo();
}
