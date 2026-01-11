#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

struct Instruction {
  string op;
  int arg;
};

struct Result {
  bool terminated = false;
  uint64_t value;
};

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // parse
  vector<Instruction> instructions;
  while (getline(file, input)) {
    stringstream ss(input);
    vector<string> vec((istream_iterator<string>(ss)),
                       istream_iterator<string>());
    string *op = &vec[0];
    int arg = stoi(vec[1]);
    instructions.push_back({*op, arg});
  }

  // solve
  size_t goal_idx = instructions.size();
  function<Result(size_t, uint64_t, unordered_set<size_t>)> run =
      [&](size_t to_visit, uint64_t global_value,
          unordered_set<size_t> visited) -> Result {
    // cout << "Start idx: " << to_visit;
    while (visited.find(to_visit) == visited.end()) {
      if (to_visit < 0 || to_visit > goal_idx) {
        break;
      }

      visited.insert(to_visit);

      const Instruction &instr = instructions[to_visit];
      if (instr.op == "jmp") {
        to_visit = to_visit + instr.arg;
      } else if (instr.op == "nop") {
        to_visit = to_visit + 1;
      } else if (instr.op == "acc") {
        to_visit = to_visit + 1;
        global_value += instr.arg;
      }

      // Check if should terminate
      if (to_visit == goal_idx) {
        // cout << endl;
        return {true, global_value};
      }
    }
    // cout << " cycle detected " << endl;

    return {false, 0};
  };

  uint64_t global_value = 0;
  size_t to_visit = 0;
  unordered_set<size_t> visited;
  while (visited.find(to_visit) == visited.end()) {
    const Instruction &instr = instructions[to_visit];

    visited.insert(to_visit);

    Result res;
    if (instr.op == "nop") {
      // Try running the program as if it was a jmp instr
      Result res = run(to_visit + instr.arg, global_value, visited);

      // Run normal
      to_visit = to_visit + 1;
    } else if (instr.op == "jmp") {
      // Try running the program as if it was a nop instr
      res = run(to_visit + 1, global_value, visited);

      // Run normal
      to_visit = to_visit + instr.arg;
    } else if (instr.op == "acc") {
      to_visit = to_visit + 1;
      global_value += instr.arg;
    }

    if (res.terminated) {
      global_value = res.value;
      break;
    }
  }

  answer += global_value;

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << answer << " Time: " << elapsed.count() << " ms" << endl;
}

void partOne(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // parse
  vector<Instruction> instructions;
  while (getline(file, input)) {
    stringstream ss(input);
    vector<string> vec((istream_iterator<string>(ss)),
                       istream_iterator<string>());
    string *op = &vec[0];
    int arg = stoi(vec[1]);
    instructions.push_back({*op, arg});
  }

  // solve
  uint64_t global_value = 0;
  size_t to_visit = 0;
  unordered_set<size_t> visited;
  while (visited.find(to_visit) == visited.end()) {
    visited.insert(to_visit);

    const Instruction &instr = instructions[to_visit];
    if (instr.op == "nop") {
      to_visit = to_visit + 1;
    } else if (instr.op == "jmp") {
      to_visit = to_visit + instr.arg;
    } else if (instr.op == "acc") {
      to_visit = to_visit + 1;
      global_value += instr.arg;
    }
  }

  answer = global_value;

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
