#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

using namespace std;

// I needed quite a few hints by LLMs here to be fair.

struct Rule {
  vector<vector<int>> next;
  optional<char> c;
};

bool compute_valid_messages(const string &message,
                            const map<int, Rule> &rules) {
  map<pair<int, int>, vector<int>> memo;

  function<vector<int>(int, int)> dfs = [&](int current_idx,
                                            int rule_id) -> vector<int> {
    pair<int, int> state = {current_idx, rule_id};
    if (memo.count(state)) {
      return memo[state];
    }
    const Rule &rule = rules.at(rule_id);
    if (rule.c.has_value()) {
      if (current_idx < message.size() &&
          message[current_idx] == rule.c.value()) {
        return memo[state] = {current_idx + 1};
      } else {
        return memo[state] = {};
      }
    }

    vector<int> results;
    for (const vector<int> &sequence : rule.next) {
      vector<int> current_starts = {current_idx};
      for (int sub_rule : sequence) {
        vector<int> next_positions;
        for (int pos : current_starts) {
          auto starts = dfs(pos, sub_rule);
          next_positions.insert(next_positions.end(), starts.begin(),
                                starts.end());
        }
        sort(next_positions.begin(), next_positions.end());
        next_positions.erase(
            unique(next_positions.begin(), next_positions.end()),
            next_positions.end());
        current_starts = std::move(next_positions);
        if (current_starts.empty())
          break;
      }
      results.insert(results.end(), current_starts.begin(),
                     current_starts.end());
    }

    return memo[state] = results;
  };
  vector<int> ends = dfs(0, 0);
  bool is_valid = false;
  for (int val : ends) {
    if (val == message.size()) {
      is_valid = true;
    }
  }

  return is_valid;
}

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  map<int, Rule> rules;
  vector<string> messages;
  bool parse_rules = true;
  while (getline(file, input)) {
    if (input.empty()) {
      parse_rules = false;
    }

    if (parse_rules) {
      size_t rule_idx_size = input.find(':');
      size_t rule_idx = stoi(input.substr(0, rule_idx_size));
      string body = input.substr(rule_idx_size + 1);

      Rule rule;
      if (body.find('"') != string::npos) {
        rule.c = body[body.find('"') + 1];
      } else {
        stringstream ss(body);
        string segment;
        while (getline(ss, segment, '|')) {
          vector<int> n;
          stringstream ss2(segment);
          int sub_id;
          while (ss2 >> sub_id) {
            n.push_back(sub_id);
          }
          rule.next.push_back(n);
        }
      }
      rules[rule_idx] = rule;
    } else {
      messages.push_back(input);
    }
  }

  // Add new rules
  rules[8].next = {{42}, {42, 8}};
  rules[11].next = {{42, 31}, {42, 11, 31}};

  // solve
  for (const string &message : messages) {
    bool is_valid = compute_valid_messages(message, rules);
    if (is_valid) {
      answer++;
    }
  }

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

  map<int, Rule> rules;
  vector<string> messages;
  bool parse_rules = true;
  while (getline(file, input)) {
    if (input.empty()) {
      parse_rules = false;
    }

    if (parse_rules) {
      size_t rule_idx_size = input.find(':');
      size_t rule_idx = stoi(input.substr(0, rule_idx_size));
      string body = input.substr(rule_idx_size + 1);

      Rule rule;
      if (body.find('"') != string::npos) {
        rule.c = body[body.find('"') + 1];
      } else {
        stringstream ss(body);
        string segment;
        while (getline(ss, segment, '|')) {
          vector<int> n;
          stringstream ss2(segment);
          int sub_id;
          while (ss2 >> sub_id) {
            n.push_back(sub_id);
          }
          rule.next.push_back(n);
        }
      }
      rules[rule_idx] = rule;
    } else {
      messages.push_back(input);
    }
  }

  // solve
  for (const string &message : messages) {
    bool is_valid = compute_valid_messages(message, rules);
    if (is_valid) {
      answer++;
    }
  }

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
}

int main() {
  string test_path = "test.txt";
  string test_path1 = "test1.txt";
  string my_path = "input.txt";

  cout << "========PART 1========" << endl;
  cout << "Test: ";
  partOne(test_path);
  cout << "Input: ";
  partOne(my_path);
  cout << endl;
  cout << "=========PART 2=========" << endl;
  cout << "Test: ";
  partTwo(test_path1);
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
