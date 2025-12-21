#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t product = 0;

  vector<int> values;
  while (getline(file, input)) {
    values.push_back(stoll(input));
  }

  bool is_found = false;
  for (int i = 0; i < values.size(); i++) {
    for (int j = i + 1; j < values.size(); j++) {
      for (int k = j + 1; k < values.size(); k++) {
        if (values[i] + values[j] + values[k] == 2020) {
          product = values[i] * values[j] * values[k];
          is_found = true;
          break;
        }
      }
    }

    if (is_found)
      break;
  }

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << "PART2: " << product << " Time: " << elapsed.count() << " ms" << endl;
}

void partOne(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t product = 0;

  vector<int> values;
  while (getline(file, input)) {
    values.push_back(stoll(input));
  }

  bool is_found = false;
  for (int i = 0; i < values.size(); i++) {
    for (int j = i + 1; j < values.size(); j++) {
      if (values[i] + values[j] == 2020) {
        product = values[i] * values[j];
        is_found = true;
        break;
      }
    }

    if (is_found)
      break;
  }

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << "PART1: " << product << " Time: " << elapsed.count() << " ms" << endl;
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
