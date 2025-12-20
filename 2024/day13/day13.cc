#include <algorithm>
#include <bits/stdint-uintn.h>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <vector>

using namespace std;

struct Button {
  int64_t X;
  int64_t Y;
};

struct Position {
  int64_t X;
  int64_t Y;

  Position operator+(const Button &other) const {
    return {X + other.X, Y + other.Y};
  }

  bool operator==(const Position &other) const {
    return (X == other.X && Y == other.Y);
  }

  bool operator<(const Position &other) const {
    return (X < other.X || Y < other.Y);
  }

  bool operator>(const Position &other) const { return other < *this; }
};

struct ClawMachineRule {
  Button A;
  Button B;
  Position prize;
};

const int64_t COST_OF_A = 3;
const int64_t COST_OF_B = 1;
const int64_t MAX_DEPTH = 100;
const int64_t CONVERSION_ERROR = 10000000000000;
map<Position, int64_t> memo;

int64_t GetMinimalTokenCost(const ClawMachineRule &claw) {
  for (size_t i_A = 1; i_A <= MAX_DEPTH; i_A++) {
    for (size_t i_B = 1; i_B <= MAX_DEPTH; i_B++) {
      int64_t X = claw.A.X * i_A + claw.B.X * i_B;
      int64_t Y = claw.A.Y * i_A + claw.B.Y * i_B;
      if (claw.prize == Position{X, Y})
        return i_A * COST_OF_A + i_B * COST_OF_B;
    }
  }

  // No solution found
  return 0;
}

int64_t GetMinimalTokenCostAlgebra(const ClawMachineRule &claw,
                                   bool use_max_depth = true) {
  int64_t d = claw.A.X * claw.B.Y - claw.B.X * claw.A.Y;
  if (d == 0)
    return 0;

  int64_t dx = claw.prize.X * claw.B.Y - claw.prize.Y * claw.B.X;
  int64_t dy = claw.A.X * claw.prize.Y - claw.A.Y * claw.prize.X;
  if ((dx % d != 0) || (dy % d != 0))
    return 0;

  int64_t X = dx / d;
  int64_t Y = dy / d;

  if ((X < MAX_DEPTH && Y < MAX_DEPTH && use_max_depth) || (!use_max_depth)) {
    return COST_OF_A * X + COST_OF_B * Y;
  }
  return 0;
}

void PartTwo() {
  ifstream file("../input.txt");
  string input;

  regex re(R"((\d+))");

  // Parse the claw machine data
  vector<ClawMachineRule> claw_machines;
  while (!file.eof()) {
    ClawMachineRule claws;

    getline(file, input);
    sregex_iterator it(input.begin(), input.end(), re);
    claws.A.X = stoi((it++)->str());
    claws.A.Y = stoi(it->str());
    cout << claws.A.X << " " << claws.A.Y << endl;

    getline(file, input);
    it = sregex_iterator(input.begin(), input.end(), re);
    claws.B.X = stoi((it++)->str());
    claws.B.Y = stoi(it->str());
    cout << claws.B.X << " " << claws.B.Y << endl;

    getline(file, input);
    it = sregex_iterator(input.begin(), input.end(), re);
    claws.prize.X = stoi((it++)->str());
    claws.prize.Y = stoi(it->str());
    cout << claws.prize.X << " " << claws.prize.Y << endl;

    claw_machines.push_back(claws);

    // Make sure to parse the empty row
    if (!file.eof())
      getline(file, input);
  }
  cout << endl;

  // Perform the algorithm
  uint64_t answer = 0;
  for (auto &claw : claw_machines) {
    claw.prize.X += CONVERSION_ERROR;
    claw.prize.Y += CONVERSION_ERROR;
    int64_t min_cost = GetMinimalTokenCostAlgebra(claw, false);
    cout << min_cost << endl;
    answer += min_cost;
  }

  cout << "Answer: " << answer << endl;
}

void PartOne() {
  ifstream file("../input.txt");
  string input;

  regex re(R"((\d+))");

  // Parse the claw machine data
  vector<ClawMachineRule> claw_machines;
  while (!file.eof()) {
    ClawMachineRule claws;

    getline(file, input);
    sregex_iterator it(input.begin(), input.end(), re);
    claws.A.X = stoi((it++)->str());
    claws.A.Y = stoi(it->str());
    cout << claws.A.X << " " << claws.A.Y << endl;

    getline(file, input);
    it = sregex_iterator(input.begin(), input.end(), re);
    claws.B.X = stoi((it++)->str());
    claws.B.Y = stoi(it->str());
    cout << claws.B.X << " " << claws.B.Y << endl;

    getline(file, input);
    it = sregex_iterator(input.begin(), input.end(), re);
    claws.prize.X = stoi((it++)->str());
    claws.prize.Y = stoi(it->str());
    cout << claws.prize.X << " " << claws.prize.Y << endl;

    claw_machines.push_back(claws);

    // Make sure to parse the empty row
    if (!file.eof())
      getline(file, input);
  }
  cout << endl;

  // Perform the algorithm
  int answer = 0;
  for (auto &claw : claw_machines) {
    int64_t min_cost = GetMinimalTokenCostAlgebra(claw);
    int cost = GetMinimalTokenCost(claw);
    cout << min_cost << endl;
    /* cout << "Brute force: " << cost << endl; */
    answer += min_cost;
  }

  cout << "Answer: " << answer << endl;
}

int main() {
  /* PartOne(); */
  PartTwo();

  return 0;
}
