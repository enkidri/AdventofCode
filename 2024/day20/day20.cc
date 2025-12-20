#include <bits/stdc++.h>

using namespace std;

using Grid = vector<vector<bool>>;

struct Index2D {
  int i;
  int j;

  Index2D operator+(const Index2D &other) { return {i + other.i, j + other.j}; }
  bool operator==(const Index2D &other) { return i == other.i && j == other.j; }
};

tuple<Grid, Index2D, Index2D> GetInput() {
  ifstream file("../input.txt");
  string input;

  Grid map;
  Index2D start;
  Index2D end;
  int i{}, j{};
  while (getline(file, input)) {
    vector<bool> row;
    j = 0;
    for (auto &c : input) {
      if (c == '#') {
        row.push_back(false);
      } else if (c == '.') {
        row.push_back(true);
      } else if (c == 'S') {
        row.push_back(false);
        start = {i, j};
      } else if (c == 'E') {
        row.push_back(true);
        end = {i, j};
      }
      j++;
    }
    map.push_back(row);
    i++;
  }

  return {map, start, end};
}

bool IsValidMove(const Grid &valid_pos, Index2D &pos) {
  int N = valid_pos.size();
  int M = valid_pos[0].size();
  return pos.i >= 0 && pos.j >= 0 && pos.i < N && pos.j < M &&
         valid_pos[pos.i][pos.j];
}

void Print(Grid &grid) {
  for (auto &row : grid) {
    for (auto ele : row) {
      cout << ele;
    }
    cout << endl;
  }
}

void PartOne() {
  auto [valid_pos, start, end] = GetInput();
  vector<vector<int>> scores(valid_pos.size(),
                             vector<int>(valid_pos[0].size(), 0));

  vector<Index2D> dirs{{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
  queue<Index2D> q;
  q.push(start);
  while (!q.empty()) {
    Index2D current = q.back();
    q.pop();

    if (current == end) {
      continue;
    }

    // Check valid directions
    for (auto &dir : dirs) {
      Index2D next_pos = current + dir;
      int next_score = scores[current.i][current.j] + 1;
      if (IsValidMove(valid_pos, next_pos)) {
        if (scores[next_pos.i][next_pos.j] == 0) {
          q.push(next_pos);
          scores[next_pos.i][next_pos.j] = next_score;
        }
      }
    }
  }

  cout << "Fastest time to the goal is " << scores[end.i][end.j] << endl;
}

int main() {
  PartOne();
  return 0;
}
