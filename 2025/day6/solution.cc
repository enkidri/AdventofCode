#include <math.h>

#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Range {
  int start;
  int end;
};

template <class T>
struct Column {
  vector<T> numbers;
  Range range;
  char op;
};

void partTwo(const string& file_path) {
  ifstream file(file_path);
  string input;
  uint64_t total = 0;

  vector<Column<string>> columns;
  vector<string> input_string_numbers;

  // Parsing the numbers
  regex re(R"(\s*(\d+)\s*)");
  for (int row = 0;; row++) {
    getline(file, input);
    input_string_numbers.push_back(input);
    if (input[0] == '*' || input[0] == '+') break;

    // Parse each number in this line
    auto begin = sregex_iterator(input.begin(), input.end(), re);
    auto end = sregex_iterator();

    size_t idx = 0;
    for (auto it = begin; it != end; it++) {
      string number = it->str();
      int start_idx = it->position(1);
      int end_idx = start_idx + it->length(1) - 1;

      if (columns.size() == idx) {
        columns.push_back(Column<string>{vector<string>{number}});
        columns.at(idx).range.start = start_idx;
        columns.at(idx).range.end = end_idx;
      } else {
        // The numbers
        columns.at(idx).numbers.push_back(number);

        // Ranges
        columns.at(idx).range.start =
            min(start_idx, columns.at(idx).range.start);
        columns.at(idx).range.end = max(end_idx, columns.at(idx).range.end);
      }
      idx++;
    }
  }

  // Debug
  // for (auto& col : columns) {
  //   cout << "Range: (" << col.range.start << " " << col.range.end << ") - ";
  //   for (auto range : col.ranges) {
  //     cout << "(" << range.start << " " << range.end << ") ";
  //   }
  //   cout << endl;
  // }

  // Parsing the operators
  istringstream iss(input);
  char op;
  int idx = 0;
  while (iss >> op) {
    columns.at(idx).op = op;
    idx++;
  }

  // Compute the grand total
  int NUM_ROWS = columns[0].numbers.size();
  for (Column<string>& col : columns) {
    vector<int> new_numbers;
    for (int i = col.range.end; i >= col.range.start; i--) {
      string number = "";
      for (int j = 0; j < NUM_ROWS; j++) {
        char c = input_string_numbers[j][i];
        if (c != ' ') {
          number += c;
        }
      }
      new_numbers.push_back(stoi(number));
    }

    if (col.op == '*') {
      total += accumulate(new_numbers.begin(), new_numbers.end(), 1ULL,
                          multiplies<uint64_t>());
    } else if (col.op == '+') {
      total += accumulate(new_numbers.begin(), new_numbers.end(), 0ULL);
    } else {
      cout << "Unknown operator" << endl;
    }
  }

  cout << "PART2: " << total << endl;
}

void partOne(const string& file_path) {
  ifstream file(file_path);
  string input;
  uint64_t total = 0;

  vector<Column<uint64_t>> columns;

  // Parsing the numbers
  for (int row = 0;; row++) {
    getline(file, input);
    if (input[0] == '*' || input[0] == '+') break;

    uint64_t number;
    istringstream iss(input);
    size_t idx = 0;
    while (iss >> number) {
      if (columns.size() == idx) {
        columns.push_back(Column<uint64_t>{vector<uint64_t>{number}});
      } else {
        columns.at(idx).numbers.push_back(number);
      }
      idx++;
    }
  }

  // Parsing the operators
  istringstream iss(input);
  char op;
  int idx = 0;
  while (iss >> op) {
    columns.at(idx).op = op;
    idx++;
  }

  // Compute the total
  for (auto& col : columns) {
    if (col.op == '*') {
      total += accumulate(col.numbers.begin(), col.numbers.end(), 1ULL,
                          multiplies<uint64_t>());
    } else if (col.op == '+') {
      total += accumulate(col.numbers.begin(), col.numbers.end(), 0ULL);
    } else {
      cout << "Unknown operator" << endl;
    }
  }

  cout << "PART1: " << total << endl;
}

int main() {
  string test_path = "test.txt";
  string my_path = "input.txt";

  cout << "========TEST OUTPUT========" << endl;
  partOne(test_path);
  partTwo(test_path);

  cout << endl;

  cout << "=========MY OUTPUT=========" << endl;
  partOne(my_path);
  partTwo(my_path);

  return 0;
};
