#include <math.h>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int mod(int a, int b) { return ((a % b) + b) % b; }

void partTwo() {
  ifstream file("input.txt");
  string input;

  int current_position = 50;
  int MAX_POINTS = 100;
  int password = 0;
  while (file >> input) {
    // Parse the input
    char dir = input[0];
    int steps = std::stoi(input.substr(1));

    // Rotate
    if (dir == 'R') {
      int reminder = MAX_POINTS - current_position;
      // int reminder = mod(-current_position, MAX_POINTS);
      if (reminder == 0) {
        reminder = 100;
      }
      if (reminder <= steps) {
        password += 1 + (steps - reminder) / MAX_POINTS;
      }
      current_position = mod(current_position + steps, MAX_POINTS);
    } else if (dir == 'L') {
      int reminder = mod(current_position, MAX_POINTS);
      if (reminder == 0) {
        reminder = 100;
      }
      if (reminder <= steps) {
        password += 1 + (steps - reminder) / MAX_POINTS;
      }
      current_position = mod(current_position - steps, MAX_POINTS);
    } else {
      cout << "Error" << endl;
    }
  }

  // Should be 6599
  cout << "PART2 - The password is : " << password << endl;
}

void partOne() {
  ifstream file("input.txt");
  string input;

  int current_position = 50;
  int MAX_POINTS = 100;
  int password = 0;
  while (file >> input) {
    // Parse the input
    char dir = input[0];
    int steps = std::stoi(input.substr(1));

    // Rotate
    if (dir == 'R') {
      current_position += steps;
      current_position %= MAX_POINTS;

    } else if (dir == 'L') {
      current_position -= steps;
      current_position %= MAX_POINTS;
      if (current_position < 0) {
        current_position += MAX_POINTS;
      }
    } else {
      cout << "Error" << endl;
    }

    if (current_position == 0) password++;
  }

  cout << "PART1 - The password is : " << password << endl;
}

int main() {
  partOne();
  partTwo();

  return 0;
};
