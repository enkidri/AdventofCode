#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

using namespace std;

struct Button {
  int X;
  int Y;
};

struct Prize {
  int X;
  int Y;
};

struct ClawMachineRule {
  Button A;
  Button B;
  Prize coord;
};

void PartOne() {
  ifstream file("input.txt");
  string input;

  regex re(R"((\d+))");

  vector<ClawMachineRule> claw_machines;
  while (!file.eof()) {
    ClawMachineRule claw;

    getline(file, input);
    sregex_iterator it(input.begin(), input.end(), re);
    claw.A.X = stoi(it->str());
    claw.A.Y = stoi(it->str());
    cout << claw.A.Y << " " << claw.A.Y << endl;

    getline(file, input);
    it = sregex_iterator(input.begin(), input.end(), re);
    claw.B.X = stoi((it++)->str());
    claw.B.Y = stoi(it->str());
    cout << claw.B.Y << " " << claw.B.Y << endl;

    getline(file, input);
    it = sregex_iterator(input.begin(), input.end(), re);
    claw.coord.X = stoi((it++)->str());
    claw.coord.Y = stoi(it->str());
    cout << claw.coord.Y << " " << claw.coord.Y << endl;

    claw_machines.push_back(claw);

    // Make sure to parse the empty row
    if (!file.eof()) getline(file, input);
  }
}

int main() {
  PartOne();

  return 0;
}
