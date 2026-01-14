#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

uint64_t compute_num_arrangement(const vector<uint64_t> &joltage_rating) {
  unordered_map<uint64_t, uint64_t> memo;
  int goal_idx = joltage_rating.size() - 1;

  function<uint64_t(size_t)> dfs = [&](size_t current_idx) -> uint64_t {
    if (current_idx == goal_idx)
      return 1;

    if (memo.count(current_idx)) {
      return memo.at(current_idx);
    }

    uint64_t total_path = 0ULL;
    for (size_t i = current_idx + 1;
         i <= current_idx + 3 && i < joltage_rating.size(); i++) {
      if (joltage_rating[i] <= joltage_rating[current_idx] + 3) {
        total_path += dfs(i);
      } else {
        break;
      }
    }

    memo[current_idx] = total_path;

    return total_path;
  };
  return dfs(0);
}

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  vector<uint64_t> joltage_ratings;
  while (getline(file, input)) {
    joltage_ratings.push_back(stoll(input));
  }

  // Add the initial
  joltage_ratings.push_back(0);

  // solve
  sort(joltage_ratings.begin(), joltage_ratings.end());
  joltage_ratings.push_back(joltage_ratings.back() + 3);
  answer = compute_num_arrangement(joltage_ratings);

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

  // parse
  vector<uint64_t> joltage_ratings;
  while (getline(file, input)) {
    joltage_ratings.push_back(stoll(input));
  }

  // Add the initial joltage
  joltage_ratings.push_back(0);

  // solve
  sort(joltage_ratings.begin(), joltage_ratings.end());
  uint64_t device_joltage_rating = joltage_ratings.back();
  unordered_map<uint64_t, uint64_t> memo;
  for (size_t i = 0, j = 1; j < joltage_ratings.size(); i++, j++) {
    uint64_t diff = joltage_ratings[j] - joltage_ratings[i];
    memo[diff]++;
  }

  // Add the final adapter (mine)
  memo[3]++;

  answer = memo[1] * memo[3];

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
}

int main() {
  string test1_path = "test1.txt";
  string test2_path = "test2.txt";
  string my_path = "input.txt";

  cout << "========PART 1========" << endl;
  cout << "Test 1: ";
  partOne(test1_path); // should be 35
  cout << "Test 2: ";
  partOne(test2_path); // should be 220
  cout << "Input: ";
  partOne(my_path);

  // cout << endl;

  // cout << "=========PART 2=========" << endl;
  cout << "Test 1: ";
  partTwo(test1_path); // should be 8
  cout << "Test 2: ";
  partTwo(test2_path); // should be 19208
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
