#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class CrabCupSimulator {
public:
  CrabCupSimulator(string &s) {
    labels = std::move(s);
    current_cup = labels[0];
    held_cups = labels.substr(1, 3);
  }

  size_t get_destination_idx() {
    unordered_map<char, size_t> label_map;
    char top_label = '0';
    for (size_t i = 0; i < labels.size(); i++) {
      if (held_cups.find(labels[i]) == string::npos) {
        label_map[labels[i]] = i;
        top_label = max(labels[i], top_label);
      }
    }

    // current label doesnt wrap around
    for (char i = current_cup - 1; i > '0'; i--) {
      auto it = label_map.find(i);
      if (it != label_map.end()) {
        return it->second;
      }
    }

    // current label does wrap around
    return label_map[top_label];
  }

  void step() {
    string new_labels;
    const size_t dest_idx = get_destination_idx();
    new_labels += labels[dest_idx];
    new_labels += held_cups;
    const size_t num_char_add = labels.size() - new_labels.size();
    for (int i = 0, j = 1; i < num_char_add; j++) {
      size_t current_idx = (dest_idx + j) % labels.size();
      if (held_cups.find(labels[current_idx]) == string::npos) {
        new_labels += labels[current_idx];
        i++;
      }
    }

    // update the current state
    labels = new_labels;
    size_t current_idx = labels.find(current_cup);
    if (current_idx == string::npos) {
      std::runtime_error("Error. Couldnt find the correct label.");
    }
    current_cup = labels[(current_idx + 1) % labels.size()];
    held_cups.clear();
    for (int i = 0; i < 3; i++) {
      size_t idx = (current_idx + 2 + i) % labels.size();
      held_cups += labels[idx];
    }
  }

  string get_final_label() {
    size_t idx = labels.find('1');
    string f1 = labels.substr(0, idx);
    string f2 = labels.substr(idx);
    return (f2 + f1).substr(1);
  }

  void print() {
    cout << "Move: " << move_count << endl;
    cout << "Labels: " << labels << endl;
    cout << "Held cups: " << held_cups << endl;
    cout << "Current cup: " << current_cup << endl;
    cout << "Destination: " << labels[get_destination_idx()] << endl;
    cout << "============" << endl;
  }

  void simulate(int num_iter, bool debug = false) {
    if (debug) {
      print();
    }

    for (size_t i = 0; i < num_iter; i++) {
      step();
      move_count++;
      if (debug) {
        print();
      }
    }
  }

private:
  string labels;
  string held_cups;
  char current_cup;
  int move_count = 1;
};

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  while (getline(file, input)) {
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
  string answer;

  if (!getline(file, input)) {
    std::runtime_error("Error. File couldnt be read.");
  }

  // perform the algorithm
  CrabCupSimulator handler(input);
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

  // cout << endl;

  // cout << "=========PART 2=========" << endl;
  // cout << "Test: ";
  // partTwo(test_path);
  // cout << "Input: ";
  // partTwo(my_path);

  return 0;
};
