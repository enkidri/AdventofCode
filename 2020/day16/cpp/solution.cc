#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

struct Range {
  int lower;
  int upper;
};

struct ClassRange {
  Range a;
  Range b;
  bool checked = false;
};

using TicketValues = vector<int>;

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // parse
  bool parse_class = true, parse_my_ticket = false, parse_nearby_ticket = false;

  vector<ClassRange> ranges;
  vector<TicketValues> ticket_values;
  TicketValues my_ticket_values;
  while (getline(file, input)) {
    if (input.empty() && parse_class) {
      parse_class = false;
      parse_my_ticket = true;
      continue;
    } else if (input.empty() && parse_my_ticket) {
      parse_my_ticket = false;
      parse_nearby_ticket = true;
      continue;
    } else if ((input[0] == 'n' && input[1] == 'e') ||
               (input[0] == 'y' && input[1] == 'o')) {
      continue;
    }

    if (parse_class) {
      ClassRange r;
      sscanf(input.c_str(), "%*[^:]: %d-%d or %d-%d", &r.a.lower, &r.a.upper,
             &r.b.lower, &r.b.upper);
      ranges.push_back(r);
    } else {
      TicketValues v;
      stringstream ss(input);
      string num;
      while (getline(ss, num, ',')) {
        v.push_back(stoi(num));
      }

      if (parse_my_ticket)
        my_ticket_values = std::move(v);
      else {
        ticket_values.push_back(v);
      }
    }
  }

  // solve
  int n = ticket_values.size();
  int m = ticket_values[0].size();

  size_t i = 0;
  for (auto &vec : ticket_values) {
    for (const auto &ele : vec) {
      bool invalid_found = true;
      for (const auto &range : ranges) {
        if ((range.a.lower <= ele && range.a.upper >= ele) ||
            (range.b.lower <= ele && range.b.upper >= ele)) {
          invalid_found = false;
          break;
        }
      }
      if (invalid_found) {
        vec.clear();
      }
    }
  }

  vector<set<int>> possibilities(ranges.size());
  for (int j = 0; j < m; j++) {
    for (int k = 0; k < ranges.size(); k++) {
      ClassRange &range = ranges[k];

      bool is_within_range = true;
      for (int i = 0; i < n; i++) {
        // skip invalid lines
        if (ticket_values[i].empty()) {
          continue;
        }

        int ele = ticket_values[i][j];
        if ((range.a.lower > ele || range.a.upper < ele) &&
            (range.b.lower > ele || range.b.upper < ele)) {
          is_within_range = false;
          break;
        }
      }

      if (is_within_range) {
        possibilities[k].insert(j);
      }
    }
  }

  vector<int> class_order(ranges.size(), -1);
  for (int count = 1; count < ranges.size(); count++) {
    for (int j = 0; j < possibilities.size(); j++) {
      if (possibilities[j].size() == 1) {
        class_order[j] = *possibilities[j].begin();

        for (int rest = 0; rest < possibilities.size(); rest++) {
          possibilities[rest].erase(class_order[j]);
        }
      }
    }
  }

  answer = 1;
  for (int i = 0; i < 6; i++) {
    answer *= my_ticket_values[class_order[i]];
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

  // parse
  bool parse_class = true, parse_my_ticket = false, parse_nearby_ticket = false;
  vector<ClassRange> ranges;
  vector<TicketValues> ticket_values;
  while (getline(file, input)) {
    if (input.empty() && parse_class) {
      parse_class = false;
      parse_my_ticket = true;
      continue;
    } else if (input.empty() && parse_my_ticket) {
      parse_my_ticket = false;
      parse_nearby_ticket = true;
      continue;
    } else if (input[0] == 'n' && input[1] == 'e') {
      continue;
    }

    if (parse_class) {
      ClassRange r;
      sscanf(input.c_str(), "%*[^:]: %d-%d or %d-%d", &r.a.lower, &r.a.upper,
             &r.b.lower, &r.b.upper);
      ranges.push_back(r);
    } else if (parse_my_ticket) {
      continue;
    } else {
      TicketValues v;
      stringstream ss(input);
      string num;
      while (getline(ss, num, ',')) {
        v.push_back(stoi(num));
      }
      ticket_values.push_back(v);
    }
  }

  // solve
  uint64_t ticket_scanner_error = 0;
  for (const auto &vec : ticket_values) {
    for (const auto &ele : vec) {
      bool invalid_found = true;
      for (const auto &range : ranges) {
        if ((range.a.lower <= ele && range.a.upper >= ele) ||
            (range.b.lower <= ele && range.b.upper >= ele)) {
          invalid_found = false;
          break;
        }
      }
      if (invalid_found) {
        ticket_scanner_error += ele;
      }
    }
  }

  answer = ticket_scanner_error;

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
  // cout << "Test: ";
  // partTwo(test_path1); // No test case
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
