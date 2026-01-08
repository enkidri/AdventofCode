#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct BagContents {
  uint64_t num = 0;
  string bag;
};

uint64_t compute_num_bags(const string &start,
                          const map<string, vector<BagContents>> &m) {
  map<string, uint64_t> memo;
  function<uint64_t(const string &)> dfs = [&](const string &s) -> uint64_t {
    if (!m.count(s))
      return 0ULL;

    if (memo.count(s)) {
      return memo.at(s);
    }

    uint64_t num_bags = 0;
    for (const BagContents &content : m.at(s)) {
      if (content.bag == start)
        continue;

      uint64_t count = dfs(content.bag);
      num_bags += content.num + content.num * count;
    }

    memo[s] = num_bags;

    return num_bags;
  };

  return dfs(start);
}

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  map<string, vector<BagContents>> m;
  while (getline(file, input)) {
    stringstream ss(input);
    vector<string> words((istream_iterator<string>(ss)),
                         istream_iterator<string>());
    if (words.size() < 2) {
      cerr << "Line to short" << endl;
      break;
    }

    string bag = words.at(0) + " " + words.at(1);
    if (words.at(4) == "no") {
      continue;
    } else {
      for (int i = 5, j = i + 1; i < words.size(); i += 4, j += 4) {
        uint64_t num = stoll(words.at(i - 1));
        string content = words.at(i) + " " + words.at(j);

        m[bag].push_back(BagContents{num, content});
      }
    }
  }

  // Solve
  answer = compute_num_bags("shiny gold", m);

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << answer << " Time: " << elapsed.count() << " ms" << endl;
}

void partOne(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  map<string, vector<string>> m;
  while (getline(file, input)) {
    stringstream ss(input);
    vector<string> words((istream_iterator<string>(ss)),
                         istream_iterator<string>());
    if (words.size() < 2) {
      cerr << "Line to short" << endl;
      break;
    }
    string bag = words.at(0) + " " + words.at(1);
    if (words.at(4) == "no") {
      m[bag].emplace({}); // If bag is empty then vector is empty
      continue;
    } else {
      for (int i = 5, j = i + 1; i < words.size(); i += 4, j += 4) {
        string content = words.at(i) + " " + words.at(j);
        m[bag].push_back(content);
      }
    }
  }

  // solve
  // reverse the map
  map<string, vector<string>> m_reversed;
  for (const auto &pair : m) {
    for (const auto &ele : pair.second) {
      m_reversed[ele].push_back(pair.first);
    }
  }

  // count
  string target_bag = "shiny gold";
  vector<string> visited;
  queue<string> q;
  q.push(target_bag);
  while (!q.empty()) {
    string current = q.front();
    q.pop();

    for (const auto &next : m_reversed[current]) {
      if (next == target_bag)
        continue;

      if (find(visited.begin(), visited.end(), next) == visited.end()) {
        visited.push_back(next);
        q.push(next);
      }
    }
  }

  answer = visited.size();

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << answer << " Time: " << elapsed.count() << " ms" << endl;
}

int main() {
  string test_path = "test.txt";
  string test1_path = "test1.txt";
  string my_path = "input.txt";

  cout << "========PART 1========" << endl;
  cout << "Test: ";
  partOne(test_path);
  cout << "Input: ";
  partOne(my_path);

  cout << endl;
  cout << "=========PART 2=========" << endl;
  cout << "Test 1: ";
  partTwo(test_path); // should be 32
  cout << "Test 2: ";
  partTwo(test1_path); // should be 126
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
