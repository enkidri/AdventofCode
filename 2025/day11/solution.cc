#include <math.h>

#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <stack>
#include <string>

using namespace std;

uint64_t compute_num_paths(const string& a, const string& b,
                           map<string, vector<string>>& m) {
  map<string, uint64_t> memo;
  map<string, bool> visiting;

  function<uint64_t(const string&)> dfs = [&](const string& u) -> uint64_t {
    if (u == b) {
      return 1ULL;
    }

    // cycle detected → infinite paths
    if (visiting[u]) {
      return UINT64_MAX;
    }

    if (memo.count(u)) {
      return memo[u];
    }

    visiting[u] = true;
    uint64_t total = 0ULL;

    for (const auto& v : m[u]) {
      uint64_t paths = dfs(v);

      if (paths == UINT64_MAX) {
        visiting[u] = false;
        return UINT64_MAX;
      }

      total += paths;
    }

    visiting[u] = false;
    memo[u] = total;
    return total;
  };

  return dfs(a);
}

void partTwo(const string& file_path) {
  ifstream file(file_path);
  string input;
  uint64_t num_paths = 0;

  // Parse the data
  map<string, vector<string>> m;
  while (getline(file, input)) {
    // Parse the key
    string key;
    istringstream ss(input);
    ss >> key;
    key.pop_back();

    // Parse the items
    string item;
    while (ss >> item) {
      m[key].push_back(item);
    }
  }
  file.close();

  // Compute the number of paths between the three segments
  // svr-fft, fft-dac, dac - out
  //
  uint64_t num_paths1 = compute_num_paths("svr", "fft", m);
  uint64_t num_paths2 = compute_num_paths("fft", "dac", m);
  uint64_t num_paths3 = compute_num_paths("dac", "out", m);

  uint64_t num_paths4 = compute_num_paths("svr", "dac", m);
  uint64_t num_paths5 = compute_num_paths("dac", "fft", m);
  uint64_t num_paths6 = compute_num_paths("fft", "out", m);

  num_paths = num_paths1 * num_paths2 * num_paths3 +
              num_paths4 * num_paths5 * num_paths6;

  cout << "PART2: " << num_paths << endl;
}

void partOne(const string& file_path) {
  ifstream file(file_path);
  string input;

  uint64_t num_paths = 0;

  // Parse the data
  map<string, vector<string>> m;
  while (getline(file, input)) {
    // Parse the key
    string key;
    istringstream ss(input);
    ss >> key;
    key.pop_back();

    // Parse the items
    string item;
    while (ss >> item) {
      m[key].push_back(item);
    }
  }
  file.close();

  // reverse the map
  map<string, vector<string>> m_reversed;
  for (const auto& [key, values] : m) {
    for (const auto& value : values) {
      m_reversed[value].push_back(key);
    }
  }

  // solve from the bottom
  map<string, uint64_t> visited;
  queue<string> to_visit;
  to_visit.push("out");
  visited["out"] = 1;
  while (!to_visit.empty()) {
    string current = to_visit.front();
    to_visit.pop();

    if (current == "you") {
      continue;
    }

    for (const auto& entry : m_reversed[current]) {
      if (visited.find(entry) == visited.end()) {
        to_visit.push(entry);
        visited[entry] = visited[current];
      } else {
        visited[entry] += visited[current];
      }
    }
  }

  // Compute the answer finally by looking at the nodes around "you"
  for (auto& value : m["you"]) {
    num_paths += visited[value];
  }

  cout << "PART1: " << num_paths << endl;
}

int main() {
  string test_path1 = "test.txt";
  string test_path2 = "test1.txt";
  string my_path = "input.txt";

  cout << "========TEST OUTPUT========" << endl;
  partOne(test_path1);
  partTwo(test_path2);

  cout << endl;

  cout << "=========MY OUTPUT=========" << endl;
  partOne(my_path);
  partTwo(my_path);

  return 0;
};
