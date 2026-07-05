#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class OptimizedCrabCupSim {

public:
  OptimizedCrabCupSim(const string &s) {
    CUP_NUMBERS = 9;
    int max_value = 0;
    for (const auto &c : s) {
      labels.push_back(c - '0');
      max_value = max(labels.back(), max_value);
      labels_map[c - '0'] = --labels.end();
    }

    current_cup_iter = labels.begin();
  }

  OptimizedCrabCupSim(const string &s, bool part_two) {
    CUP_NUMBERS = 1000000;
    int max_value = 0;
    for (const auto &c : s) {
      labels.push_back(c - '0');
      max_value = max(labels.back(), max_value);
      labels_map[c - '0'] = --labels.end();
    }
    // for the extra condition
    for (int i = max_value + 1; i <= CUP_NUMBERS; i++) {
      labels.push_back(i);
      labels_map[i] = --labels.end();
    }

    current_cup_iter = labels.begin();
  }

  vector<int> get_held_cups() {
    vector<int> held_cups;
    held_cups.reserve(3);

    auto it = current_cup_iter;

    for (int i = 1; i <= 3; i++) {
      ++it;

      if (it == labels.end()) {
        it = labels.begin();
      }

      held_cups.push_back(*it);
    }

    return held_cups;
  }

  void remove_held_cups() {
    vector<list<int>::iterator> targets;
    targets.reserve(3);
    auto it = current_cup_iter;

    for (int i = 0; i < 3; i++) {
      ++it;

      if (it == labels.end()) {
        it = labels.begin();
      }

      targets.push_back(it);
    }

    for (auto target : targets) {
      labels.erase(target);
    }
  }

  auto get_destination_iter(const vector<int> &invalid_dest_values) {
    int destination_value = 0;
    for (int i = 1; i <= 4; i++) {
      int val = *current_cup_iter - i;

      if (val < 1) {
        val += CUP_NUMBERS;
      }

      auto it =
          find(invalid_dest_values.begin(), invalid_dest_values.end(), val);
      if (it == invalid_dest_values.end()) {
        destination_value = val;
        break;
      }
    }

    return labels_map[destination_value];
  }

  void step() {
    vector<int> held_cups = get_held_cups();
    remove_held_cups();
    auto dest_iter = get_destination_iter(held_cups);

    ++dest_iter;
    for (int cup : held_cups) {
      auto it = labels.insert(dest_iter, cup);

      // update map
      labels_map[cup] = it;

      ++it;
    }

    // update internal state
    current_cup_iter++;
    if (current_cup_iter == labels.end()) {
      current_cup_iter = labels.begin();
    }
    move_count++;
  }

  void print() {
    cout << "Move: " << move_count << endl;
    cout << "Labels: ";
    for (auto c : labels) {
      cout << c;
    }
    cout << endl;
    cout << "Current cup: " << *current_cup_iter << endl;
    cout << "============" << endl;
  }

  void simulate(int num_iter, bool debug = false) {
    if (debug) {
      print();
    }

    for (size_t i = 0; i < num_iter; i++) {
      step();
      if (debug) {
        print();
      }
    }
  }

  string get_final_label() {
    string s;
    auto start_it = find(labels.begin(), labels.end(), 1);
    for (auto it = ++start_it; it != labels.end(); it++) {
      s += to_string(*it);
    }
    auto end_it = --start_it;
    for (auto it = labels.begin(); it != end_it; it++) {
      s += to_string(*it);
    }
    return s;
  }

  uint64_t get_final_product() {
    auto it = labels_map[1];
    uint64_t val1 = *std::next(it, 1);
    uint64_t val2 = *std::next(it, 2);
    return val1 * val2;
  }

private:
  list<int> labels;
  list<int>::iterator current_cup_iter;
  unordered_map<int, list<int>::iterator> labels_map;
  int move_count = 1;
  int CUP_NUMBERS;
};

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer;

  if (!getline(file, input)) {
    std::runtime_error("Error. File couldnt be read.");
  }

  // perform the algorithm
  OptimizedCrabCupSim handler(input, true);
  handler.simulate(10000000, false);
  answer = handler.get_final_product();

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
}

void partOne(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  string answer;

  if (!getline(file, input)) {
    std::runtime_error("Error. File couldnt be read.");
  }

  // perform the algorithm
  OptimizedCrabCupSim handler(input);
  handler.simulate(100, false);
  answer = handler.get_final_label();

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
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
