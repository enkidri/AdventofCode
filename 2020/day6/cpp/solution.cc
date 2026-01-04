#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

using Group = vector<string>;

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // Parse
  vector<Group> groups;
  Group group;
  while (getline(file, input)) {
    if (input.empty() && !group.empty()) {
      groups.push_back(group);
      group.clear();
      continue;
    }

    group.push_back(input);
  }
  // Push the last group
  groups.push_back(group);

  // Solve
  for (const auto &group : groups) {
    map<char, int> m;
    for (const auto &answers : group) {
      for (const auto &ans : answers) {
        m[ans]++;
      }
    }

    for (auto [key, item] : m) {
      if (item == group.size()) {
        answer++;
      }
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

  // Parse
  vector<Group> groups;
  Group group;
  while (getline(file, input)) {
    if (input.empty() && !group.empty()) {
      groups.push_back(group);
      group.clear();
      continue;
    }

    group.push_back(input);
  }
  // Push the last group
  groups.push_back(group);

  // Solve
  for (const auto &group : groups) {
    string flat_group;
    for (const auto &answers : group) {
      flat_group += answers;
    }

    set<char> s(flat_group.begin(), flat_group.end());
    answer += s.size();
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
  partOne(my_path);

  cout << endl;

  cout << "=========PART 2=========" << endl;
  cout << "Test: ";
  partTwo(test_path);
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
