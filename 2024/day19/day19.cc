#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct State {
  vector<string> strings;
  long long idx;
  vector<long long> idxs;

  bool operator<(const State &rhs) const {
    return idx < rhs.idx && strings < rhs.strings;
  }

  bool operator==(const State &rhs) const { return strings == rhs.strings; }
};

ostream &operator<<(ostream &os, vector<string> &s) {
  for (auto ele : s) {
    os << ele << " ";
  }
  os << endl;
  return os;
}

bool IsCreatable(const vector<string> &towels, const string design) {
  stack<State> s;
  set<int> visited;
  s.push({{}, 0});
  while (!s.empty()) {
    State current = s.top();
    s.pop();

    if (current.idx >= design.size()) {
      // cout << current.strings;
      return true;
    }

    if (visited.find(current.idx) != visited.end()) {
      continue;
    }

    for (auto &towel : towels) {
      string to_check = design.substr(current.idx, towel.size());
      if (towel == to_check) {
        // cout << to_check << endl;
        State new_state = current;
        new_state.strings.push_back(towel);
        new_state.idx += towel.size();
        s.push(new_state);
      }
    }

    visited.insert(current.idx);
  }
  return false;
}

long long GetNumCombinations(const vector<string> &towels,
                             const string design) {
  int n = design.length();
  std::vector<long long> dp(n + 1, 0);
  dp[0] = 1; // Base case: an empty string can be formed in one way (by using no
             // components)

  for (int i = 1; i <= n; ++i) {
    for (const std::string &towel : towels) {
      int componentLength = towel.length();
      if (i >= componentLength &&
          design.substr(i - componentLength, componentLength) == towel) {
        dp[i] += dp[i - componentLength];
      }
    }
  }
  return dp[n];
}

void PartTwo() {
  ifstream file("../input.txt");
  string input;
  regex re("(\\w+)");

  // Parse the towels
  vector<string> towels;
  while (getline(file, input)) {
    if (input.empty())
      break;

    auto begin = sregex_iterator(input.begin(), input.end(), re);
    auto end = sregex_iterator();

    for (; begin != end; begin++) {
      towels.push_back(begin->str());
    }
  }

  // Check number of possible combinations
  string design;
  long long answer{};
  while (getline(file, design)) {
    answer += GetNumCombinations(towels, design);
  }

  cout << "Answer: " << answer << endl;
}

void PartOne() {
  ifstream file("../input.txt");
  string input;
  regex re("(\\w+)");

  // Parse the towels
  vector<string> towels;
  while (getline(file, input)) {
    if (input.empty())
      break;

    auto begin = sregex_iterator(input.begin(), input.end(), re);
    auto end = sregex_iterator();

    for (; begin != end; begin++) {
      towels.push_back(begin->str());
    }
  }

  // Check if proposed towel design is creatable
  string design;
  int num_creatable_designs = 0;
  int count{};
  while (getline(file, design)) {
    if (IsCreatable(towels, design)) {
      num_creatable_designs++;
      // cout << count << ":" << design << endl;
    }
    count++;
  }

  cout << "Answer: " << num_creatable_designs << endl;
}

int main() {
  // PartOne();
  PartTwo();

  return 0;
}
