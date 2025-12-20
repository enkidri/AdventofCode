#include <math.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

struct IdRange {
  uint64_t start;
  uint64_t end;
};

bool intersection(const IdRange& range1, const IdRange& range2) {
  return std::max(range1.start, range2.start) <=
         std::min(range1.end, range2.end);
}

void partTwo(const string& file_path) {
  ifstream file(file_path);
  string input;
  uint64_t num_fresh_ingredients = 0;

  regex re(R"((\d+)-(\d+))");

  // Parse only the ranges
  vector<IdRange> r;
  while (getline(file, input)) {
    // If line break then skip to read the ids
    if (input.empty()) {
      break;
    }

    smatch match;
    if (regex_match(input, match, re)) {
      uint64_t start = stoll(match[1].str());
      uint64_t end = stoll(match[2].str());
      IdRange next_range{start, end};
      r.push_back(next_range);
    }
  }

  sort(r.begin(), r.end(),
       [](const IdRange& a, const IdRange& b) { return a.start < b.start; });

  // Compute the unions
  for (size_t i = 0, j = 1; i + j < r.size();) {
    IdRange& first = r[i];
    IdRange& second = r[i + j];
    if (intersection(first, second)) {
      first.end = max(second.end, first.end);
      second.start = 0;
      second.end = 0;
      j++;
    } else {
      i += j;
      j = 1;
    }
  }

  for (auto& range : r) {
    // cout << range.start << " " << range.end << endl;
    if (range.start != 0 && range.end != 0) {
      num_fresh_ingredients +=
          range.end - range.start + 1;  // Extra 1 because of open bound
    }
  }

  cout << "PART2: " << num_fresh_ingredients << endl;
}

void partOne(const string& file_path) {
  ifstream file(file_path);
  string input;
  int num_fresh_ingredients = 0;

  regex re(R"((\d+)-(\d+))");

  // Parse the ranges and id
  vector<IdRange> r;
  vector<uint64_t> ids;
  bool has_parsed_id = false;
  while (getline(file, input)) {
    // If line break then skip to read the ids
    if (input.empty()) {
      has_parsed_id = true;
      continue;
    }

    if (!has_parsed_id) {
      smatch match;
      if (regex_match(input, match, re)) {
        uint64_t start = stoll(match[1].str());
        uint64_t end = stoll(match[2].str());
        r.push_back({start, end});
      }
    } else {
      ids.push_back(stoll(input));
    }
  }

  // check each id for its range
  for (auto& id : ids) {
    for (auto& range : r) {
      if (id <= range.end && id >= range.start) {
        num_fresh_ingredients++;
        break;
      }
    }
  }

  cout << "PART1: " << num_fresh_ingredients << endl;
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
