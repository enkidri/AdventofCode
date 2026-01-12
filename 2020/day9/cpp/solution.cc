#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

void partTwo(const string &file_path, int premable_size) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // parse
  vector<uint64_t> data;
  while (getline(file, input)) {
    uint64_t n = stoll(input);
    data.push_back(n);
  }

  uint64_t invalid_number;
  size_t invalid_idx;
  deque<uint64_t> window(data.begin(), data.begin() + premable_size);
  for (size_t i = premable_size; i < data.size(); i++) {
    uint64_t target = data[i];
    vector<uint64_t> visited;
    bool is_protected = false;
    for (const auto &val : window) {
      uint64_t complement = target - val;
      if (find(visited.begin(), visited.end(), complement) != visited.end()) {
        is_protected = true;
        break;
      }
      visited.push_back(val);
    }

    // Update the window
    window.pop_front();
    window.push_back(target);

    if (!is_protected) {
      invalid_idx = i;
      invalid_number = target;
      break;
    }
  }

  uint64_t target_number = invalid_number;
  size_t r_idx = invalid_idx - 1;
  size_t l_idx = invalid_idx - 2;
  uint64_t current_sum = data[r_idx] + data[l_idx];
  while (current_sum != invalid_number && l_idx >= 0 && r_idx >= 0) {
    if (r_idx == l_idx) {
      l_idx--;
      current_sum += data[l_idx];
    }

    if (current_sum > target_number) {
      current_sum -= data[r_idx];
      r_idx--;
    } else if (current_sum < target_number) {
      l_idx--;
      current_sum += data[l_idx];
    } else {
      break;
    }
  }

  uint64_t max_val = 0;
  uint64_t min_val = UINT64_MAX;
  for (size_t i = l_idx; i <= r_idx; i++) {
    max_val = max(max_val, data[i]);
    min_val = min(min_val, data[i]);
  }
  answer = min_val + max_val;

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << answer << " Time: " << elapsed.count() << " ms" << endl;
}

void partOne(const string &file_path, int premable_size) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // parse
  vector<uint64_t> data;
  while (getline(file, input)) {
    uint64_t n = stoll(input);
    data.push_back(n);
  }

  // solve
  deque<uint64_t> window(data.begin(), data.begin() + premable_size);
  for (size_t i = premable_size; i < data.size(); i++) {
    uint64_t target = data[i];
    vector<uint64_t> visited;
    bool is_protected = false;
    for (const auto &val : window) {
      uint64_t complement = target - val;
      if (find(visited.begin(), visited.end(), complement) != visited.end()) {
        is_protected = true;
        break;
      }
      visited.push_back(val);
    }

    // Update the window
    window.pop_front();
    window.push_back(target);

    if (!is_protected) {
      answer = target;
      break;
    }
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
  partOne(test_path, 5);
  cout << "Input: ";
  partOne(my_path, 25);

  cout << endl;

  cout << "=========PART 2=========" << endl;
  cout << "Test: ";
  partTwo(test_path, 5);
  cout << "Input: ";
  partTwo(my_path, 25);

  return 0;
};
