#include <cinttypes>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

using namespace std;

using Grid = vector<string>;

struct Index2D {
  int i;
  int j;

  Index2D operator+(const Index2D &other) { return {i + other.i, j + other.j}; }

  bool operator==(const Index2D &other) { return i == other.i && j == other.j; }

  friend ostream &operator<<(ostream &os, const Index2D &idx) {
    os << "(" << idx.i << ", " << idx.j << ")";
    return os;
  }
};

struct BoxInfo {
  Index2D left;
  Index2D right;
  bool found = false;
  bool obstacle_in_pos = false;

  bool operator==(const BoxInfo &other) {
    return left == other.left && right == other.right;
  }

  friend ostream &operator<<(ostream &os, const BoxInfo &b) {
    os << "L:" << b.left << " " << "R:" << b.right;
    return os;
  }
};

class Factory {
public:
  Factory() = default;

  void MoveRobot(char d) {
    Index2D p_new = robot_position + dirs[d];

    // Skip update if p_new is at a wall.
    if (*AtGrid(p_new) == '#')
      return;

    // Just move robot if grid position is empty
    if (*AtGrid(p_new) == '.') {
      *AtGrid(p_new) = '@';
      *AtGrid(robot_position) = '.';
      robot_position = p_new;
      return;
    }

    // Get the line cluster of obstacles
    Index2D p_robot = p_new;
    while (*AtGrid(p_new) == 'O') {
      p_new = p_new + dirs[d];
    }

    // Check if the cluster is movable
    bool is_movable = (*AtGrid(p_new) == '.');

    // Update all the positions
    if (is_movable) {
      *AtGrid(robot_position) = '.';
      *AtGrid(p_robot) = '@';
      *AtGrid(p_new) = 'O';
      robot_position = p_robot;
    }
  }

  void LargeMoveRobot(char d) {
    Index2D p_new = robot_position + dirs[d];

    // Skip update if p_new is at a wall.
    if (*AtGrid(p_new) == '#')
      return;

    // Just move robot if grid position is empty
    if (*AtGrid(p_new) == '.') {
      *AtGrid(p_new) = '@';
      *AtGrid(robot_position) = '.';
      robot_position = p_new;
      return;
    }

    // Check if its clear ahead of the larger boxes
    if (d == '<' || d == '>') {
      Index2D p_robot_new = p_new;
      vector<char *> to_be_moved;
      while (*AtGrid(p_new) == ']' || *AtGrid(p_new) == '[') {
        to_be_moved.push_back(AtGrid(p_new));
        p_new = p_new + dirs[d];
      }
      to_be_moved.push_back(AtGrid(p_new));

      bool is_movable = (*AtGrid(p_new) == '.');
      // Move all characters to the empty space
      if (is_movable) {
        // TODO: Iterate through to_be_moved backwards
        for (auto it = to_be_moved.rbegin(); it + 1 != to_be_moved.rend();
             it++) {
          char *current = *it;
          char *next = *(it + 1);
          *current = *next;
        }

        // Update map
        *AtGrid(robot_position) = '.';
        *to_be_moved.front() = '@';
        robot_position = p_robot_new;
      }
    } else {
      Index2D p_robot_new = p_new;
      queue<BoxInfo> q;
      vector<BoxInfo> v;
      bool is_movable = true;
      q.push(GetBoxFromPos(p_new));
      while (!q.empty()) {
        BoxInfo b = q.front();
        q.pop();
        v.push_back(b);
        // cout << v.back() << endl;

        // Check if the way is clear else add to queue
        BoxInfo b_left = GetBoxFromPos(b.left + dirs[d]);
        BoxInfo b_right = GetBoxFromPos(b.right + dirs[d]);

        if (b_left.obstacle_in_pos || b_right.obstacle_in_pos) {
          is_movable = false;
          break;
        }

        if (b_left == b_right && b_left.found && b_right.found) {
          q.push(b_left);
          v.push_back(b_left);
        } else {
          if (b_left.found) {
            q.push(b_left);
          }
          if (b_right.found) {
            q.push(b_right);
          }
        }
      }

      if (is_movable) {
        // Set these positions as empty
        for (auto it = v.begin(); it != v.end(); it++) {
          BoxInfo box = *it;
          *AtGrid(box.left) = '.';
          *AtGrid(box.right) = '.';
        }

        // Move to new position
        for (auto it = v.begin(); it != v.end(); it++) {
          BoxInfo box = *it;
          *AtGrid(box.left + dirs[d]) = '[';
          *AtGrid(box.right + dirs[d]) = ']';
        }
        *AtGrid(robot_position) = '.';
        *AtGrid(p_robot_new) = '@';
        robot_position = p_robot_new;
      }
    }
  }

  char *AtGrid(Index2D index) { return &grid[index.i][index.j]; }

  void PrintGrid() {
    for (auto &row : grid) {
      for (auto &ele : row) {
        cout << ele;
      }
      cout << endl;
    }
    cout << endl;
  }

  int SumGpsCoord() {
    int gps_sum = 0;
    for (int i = 0; i < grid.size(); i++) {
      for (int j = 0; j < grid[0].size(); j++) {
        if (grid[i][j] == 'O') {
          gps_sum += 100 * i + j;
        }
      }
    }

    return gps_sum;
  }

  int LargerSumGpsCoord() {
    int gps_sum = 0;
    for (int i = 0; i < grid.size(); i++) {
      for (int j = 0; j < grid[0].size(); j++) {
        if (grid[i][j] == '[') {
          gps_sum += 100 * i + j;
        }
      }
    }

    return gps_sum;
  }

  Grid grid;
  Index2D robot_position;

private:
  BoxInfo GetBoxFromPos(Index2D idx) {
    BoxInfo box{};
    if (*AtGrid(idx) == ']') {
      box.right = idx;
      box.left = Index2D{idx.i, idx.j - 1};
      box.found = true;
    }
    if (*AtGrid(idx) == '[') {
      box.left = idx;
      box.right = Index2D{idx.i, idx.j + 1};
      box.found = true;
    }
    if (*AtGrid(idx) == '#') {
      box.obstacle_in_pos = true;
    }

    return box;
  }

  map<char, Index2D> dirs = {
      {'^', {-1, 0}}, {'<', {0, -1}}, {'v', {1, 0}}, {'>', {0, 1}}};
};

void PartTwo() {
  ifstream file("../input.txt");
  string input;

  // Parse the input
  Factory factory;
  string commands;
  int i = 0;
  while (getline(file, input)) {
    if (input.empty())
      break;

    string row;
    for (int j = 0; j < input.size(); j++) {
      if (input[j] == '@') {
        factory.robot_position = {i, 2 * j};
        row += "@.";
      } else if (input[j] == 'O') {
        row += "[]";
      } else if (input[j] == '.') {
        row += "..";
      } else if (input[j] == '#') {
        row += "##";
      }
    }
    factory.grid.push_back(row);
    i++;
  }

  while (getline(file, input)) {
    commands += input;
  }

  // Perform the algorithm
  int answer = 0;
  for (auto &direction : commands) {
    factory.LargeMoveRobot(direction);
  }
  factory.PrintGrid();
  answer = factory.LargerSumGpsCoord();
  cout << "Answer: " << answer << endl;
}

void PartOne() {
  ifstream file("../input.txt");
  string input;

  // Parse the input
  Factory factory;
  string commands;
  int i = 0;
  while (getline(file, input)) {
    if (input.empty())
      break;
    vector<char> row;
    factory.grid.push_back(input);
    for (int j = 0; j < input.size(); j++) {
      if (input[j] == '@')
        factory.robot_position = {i, j};
    }
    i++;
  }

  while (getline(file, input)) {
    commands += input;
  }

  // Perform the algorithm
  for (auto &direction : commands) {
    factory.MoveRobot(direction);
    // factory.PrintGrid();
  }

  int answer = factory.SumGpsCoord();
  cout << "Answer: " << answer << endl;
}

int main() {
  // PartOne();
  PartTwo();

  return 0;
}
