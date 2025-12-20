#include <math.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void partTwo(const string& file_path) {
  ifstream file(file_path);
  string input;
  uint64_t total_joltage = 0;

  int MAX_BANK_SIZE = 12;
  while (file >> input) {
    string bank = input;

    string joltage = "";

    int N = bank.size();
    int M = N - MAX_BANK_SIZE + 1;
    int idx = 0;
    for (int _ = 0; _ < MAX_BANK_SIZE; _++) {
      string s = bank.substr(idx, M);

      // Track the max value and index of substr
      int max_val = 0;
      int max_val_i = 0;

      // Find the idx and max val
      for (size_t i = 0; i < s.size(); i++) {
        int val = s[i] - '0';
        if (val > max_val) {
          max_val = val;
          max_val_i = i;
        }
      }

      // Update variables
      joltage += to_string(max_val);
      M -= max_val_i;
      idx += max_val_i + 1;
    }
    total_joltage += stoll(joltage);
  }

  cout << "PART2: " << total_joltage << endl;
}

void partOne(const string& file_path) {
  ifstream file(file_path);
  string input;
  uint64_t total_joltage = 0;

  while (file >> input) {
    string bank = input;
    int N = bank.size();
    uint64_t current_max_joltage = 0;
    for (int i = 0; i < N; i++) {
      int left = 10 * (bank[i] - '0');
      for (int j = i + 1; j < N; j++) {
        int right = bank[j] - '0';
        uint64_t joltage = left + right;
        if (joltage > current_max_joltage) {
          current_max_joltage = joltage;
        }
      }
    }
    total_joltage += current_max_joltage;
  }

  cout << "PART1: " << total_joltage << endl;
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
