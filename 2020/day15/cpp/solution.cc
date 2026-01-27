#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  vector<int> starting_nums;
  while (getline(file, input, ',')) {
    starting_nums.push_back(stoi(input));
  }

  // Process the starting numbers
  const size_t num_iter = 30000000;
  vector<int> memo(num_iter, -1);
  for (size_t i = 0; i < starting_nums.size() - 1; i++) {
    memo[starting_nums[i]] = i;
  }
  int last_spoken_val = starting_nums.back();

  for (size_t i = starting_nums.size(); i < num_iter; i++) {
    int prev_idx = memo[last_spoken_val];
    memo[last_spoken_val] = i - 1;
    if (prev_idx == -1) {
      last_spoken_val = 0;
    } else {
      last_spoken_val = (i - 1) - prev_idx;
    }
  }

  answer = last_spoken_val;

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

  vector<int> starting_nums;
  while (getline(file, input, ',')) {
    starting_nums.push_back(stoi(input));
  }

  // solve

  // Process the starting numbers
  unordered_map<int, uint64_t> memo;
  for (size_t i = 0; i < starting_nums.size() - 1; i++) {
    memo[starting_nums[i]] = i;
  }
  int last_spoken_val = starting_nums.back();

  const size_t num_iter = 2020;
  memo.reserve(num_iter);
  for (size_t i = starting_nums.size(); i < num_iter; i++) {
    auto it = memo.find(last_spoken_val);
    if (it == memo.end()) {
      memo[last_spoken_val] = i - 1;
      last_spoken_val = 0;
    } else {
      last_spoken_val = (i - 1) - it->second;
      it->second = i - 1;
    }
  }

  answer = last_spoken_val;

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
}

int main() {
  string test_path = "test.txt";
  string test_path1 = "test1.txt";
  string test_path2 = "test2.txt";
  string test_path3 = "test3.txt";
  string test_path4 = "test4.txt";
  string test_path5 = "test5.txt";
  string test_path6 = "test6.txt";
  string my_path = "input.txt";

  cout << "========PART 1========" << endl;
  cout << "Test: ";
  partOne(test_path);
  partOne(test_path1);
  partOne(test_path2);
  partOne(test_path3);
  partOne(test_path4);
  partOne(test_path5);
  partOne(test_path6);
  cout << "Input: ";
  partOne(my_path);

  cout << endl;

  cout << "=========PART 2=========" << endl;
  cout << "Test: ";
  partTwo(test_path);
  partTwo(test_path1);
  partTwo(test_path2);
  partTwo(test_path3);
  partTwo(test_path4);
  partTwo(test_path5);
  partTwo(test_path6);
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
