#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Instruction {
  char dir;
  int count;
};

struct State {
  int x;
  int y;
  int heading;
};

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // parse
  vector<Instruction> instructions;
  while (getline(file, input)) {
    char dir = input[0];
    int count = stoi(input.substr(1));
    instructions.push_back({dir, count});
  }

  // solve
  vector<char> dirs = {'S', 'W', 'N', 'E'};
  int N = dirs.size();
  State current{0, 0, 3};
  State waypoint{10, 1};
  for (const auto &instruction : instructions) {
    // Update current heading
    if (instruction.dir == 'L' || instruction.dir == 'R') {
      int num_changes = abs(instruction.count / 90);

      // rotate the waypoint
      State new_waypoint = waypoint;
      if (instruction.dir == 'R') {
        for (int _ = 0; _ < num_changes; _++) {
          new_waypoint = {new_waypoint.y, -new_waypoint.x};
        }
      } else {
        for (int _ = 0; _ < num_changes; _++) {
          new_waypoint = {-new_waypoint.y, new_waypoint.x};
        }
      }

      waypoint = new_waypoint;
      // Finished and continue
      continue;
    }

    // Assign dir to move
    if (instruction.dir == 'F') {
      current.x += waypoint.x * instruction.count;
      current.y += waypoint.y * instruction.count;
      continue;
    }

    // Move
    if (instruction.dir == 'N') {
      waypoint.y += instruction.count;
    } else if (instruction.dir == 'S') {
      waypoint.y -= instruction.count;
    } else if (instruction.dir == 'E') {
      waypoint.x += instruction.count;
    } else if (instruction.dir == 'W') {
      waypoint.x -= instruction.count;
    }
  }

  // Compute the manhattan distance
  answer = abs(current.x) + abs(current.y);

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

  vector<Instruction> instructions;
  while (getline(file, input)) {
    char dir = input[0];
    int count = stoi(input.substr(1));
    instructions.push_back({dir, count});
  }

  // solve
  vector<char> dirs = {'S', 'W', 'N', 'E'};
  int N = dirs.size();
  State current{0, 0, 3};
  for (const auto &instruction : instructions) {
    char dir_to_move;

    // Update current heading
    if (instruction.dir == 'L' || instruction.dir == 'R') {
      int num_changes = (instruction.dir == 'L') ? -instruction.count / 90
                                                 : instruction.count / 90;
      int num_idx_change = ((current.heading + num_changes) % N + N) % N;
      current.heading = num_idx_change;
      continue;
    }

    // Assign dir to move
    if (instruction.dir == 'F') {
      dir_to_move = dirs[current.heading];
    } else {
      dir_to_move = instruction.dir;
    }

    // Move
    if (dir_to_move == 'N') {
      current.y += instruction.count;
    } else if (dir_to_move == 'S') {
      current.y -= instruction.count;
    } else if (dir_to_move == 'E') {
      current.x += instruction.count;
    } else if (dir_to_move == 'W') {
      current.x -= instruction.count;
    }
  }

  // Compute the manhattan distance
  answer = abs(current.x) + abs(current.y);

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
