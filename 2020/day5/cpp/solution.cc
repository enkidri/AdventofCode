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
  uint64_t answer = 0;

  auto binary_partition = [](const string &input, int lower, int upper,
                             char p_low, char p_up) -> int {
    for (const auto &c : input) {
      uint8_t center = lower + (upper - lower - 1) / 2;
      if (c == p_low) {
        upper = center;
      } else if (c == p_up) {
        lower = center + 1;
      } else {
        cerr << "Something wrong with the parsing" << endl;
      }
    }

    // I could just return either lower or upper
    return lower;
  };

  vector<bool> occupation_idx(128 * 8);
  while (getline(file, input)) {
    // To decode
    string row_code = input.substr(0, 7);
    string col_code = input.substr(7);

    // Solve
    uint64_t row = binary_partition(row_code, 0, 127, 'F', 'B');
    uint64_t col = binary_partition(col_code, 0, 7, 'L', 'R');
    uint64_t id = row * 8 + col;
    occupation_idx[id] = true;
  }

  for (int i = 8; i < occupation_idx.size() - 8; i++) {
    if (occupation_idx[i - 1] && !occupation_idx[i] && occupation_idx[i + 1]) {
      answer = i;
      break;
    }
  }

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << answer << " Time: " << elapsed.count() << " ms" << endl;
}

void partOne(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  auto binary_partition = [](const string &input, int lower, int upper,
                             char p_low, char p_up) -> int {
    for (const auto &c : input) {
      uint8_t center = lower + (upper - lower - 1) / 2;
      if (c == p_low) {
        upper = center;
      } else if (c == p_up) {
        lower = center + 1;
      } else {
        cerr << "Something wrong with the parsing" << endl;
      }
    }

    // I could just return either lower or upper
    return lower;
  };

  while (getline(file, input)) {
    // To decode
    string row_code = input.substr(0, 7);
    string col_code = input.substr(7);

    // Solve
    uint64_t row = binary_partition(row_code, 0, 127, 'F', 'B');
    uint64_t col = binary_partition(col_code, 0, 7, 'L', 'R');
    uint64_t id = row * 8 + col;
    answer = max(id, answer);
  }

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << answer << " Time: " << elapsed.count() << " ms" << endl;
}

int main() {
  string test_path = "test.txt";
  string my_path = "input.txt";

  cout << "========PART 1========" << endl;
  cout << "Test: ";
  partOne(test_path);
  cout << "Input: ";
  partOne(my_path); // Should return 820

  cout << endl;

  cout << "=========PART 2=========" << endl;
  // cout << "Test: ";
  // partTwo(test_path); // No test data for this day
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
