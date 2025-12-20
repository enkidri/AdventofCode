#include <algorithm>
#include <bits/stdint-uintn.h>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

map<uint64_t, vector<uint64_t>> mem_map;
static const size_t NUM_ITERATIONS = 75;

void Print(vector<int> &vec) {
  for (auto &v : vec) {
    cout << v << " ";
  }
  cout << endl;
}

bool IsEvenDigitSize(uint64_t value) {
  string str = to_string(value);
  return str.size() % 2 == 0;
}

uint64_t ScoreForNumber(uint64_t val, uint64_t num_iter_left) {
  // Terminate recursion
  if (num_iter_left == 0) {
    return 1;
  }

  // Update memory
  auto it = mem_map.find(val);
  if (it != mem_map.end()) {
    uint64_t mem_value = it->second[num_iter_left];
    if (mem_value != 0) {
      return mem_value;
    }
  } else {
    mem_map[val] = vector<uint64_t>(NUM_ITERATIONS + 1, 0);
  }

  // Do recursion
  uint64_t score = 0;
  if (val == 0) {
    score = ScoreForNumber(1, num_iter_left - 1);
  } else if (IsEvenDigitSize(val)) {
    string value_str = to_string(val);
    uint64_t val1 = stoll(value_str.substr(0, value_str.size() / 2));
    uint64_t val2 =
        stoll(value_str.substr(value_str.size() / 2, value_str.size() / 2));
    score = ScoreForNumber(val1, num_iter_left - 1) +
            ScoreForNumber(val2, num_iter_left - 1);
  } else {
    score = ScoreForNumber(val * 2024, num_iter_left - 1);
  }

  mem_map[val][num_iter_left] = score;

  return score;
}

void PartTwo() {
  // Parse the input
  ifstream file("../input.txt");
  string input;
  getline(file, input);

  istringstream is(input);
  vector<uint64_t> v((istream_iterator<uint64_t>(is)),
                     istream_iterator<uint64_t>());
  uint64_t score = 0;
  // The heavy lifting
  for (auto &val : v) {
    score += ScoreForNumber(val, NUM_ITERATIONS);
  }

  cout << "Answer: " << score << endl;
}

void PartOne() {
  // Parse the input

  ifstream file("../input.txt");
  string input;
  getline(file, input);

  istringstream is(input);
  vector<uint64_t> v((istream_iterator<uint64_t>(is)),
                     istream_iterator<uint64_t>());
  size_t NUM_ITERATIONS = 25;
  for (size_t iter = 0; iter < NUM_ITERATIONS; iter++) {
    // Do the blinking
    vector<uint64_t> new_v;
    for (size_t i = 0; i < v.size(); i++) {
      uint64_t value = v[i];
      if (value == 0) {
        new_v.push_back(1);
      } else if (IsEvenDigitSize(value)) {
        // Split the value
        string value_str = to_string(value);
        uint64_t val1 = stoll(value_str.substr(0, value_str.size() / 2));
        uint64_t val2 =
            stoll(value_str.substr(value_str.size() / 2, value_str.size() / 2));
        new_v.push_back(val1);
        new_v.push_back(val2);
      } else {
        new_v.push_back(value * 2024);
      }
    }
    v = new_v;
  }

  cout << "Answer: " << v.size() << endl;
}

int main() {
  PartOne();
  PartTwo();

  return 0;
}
