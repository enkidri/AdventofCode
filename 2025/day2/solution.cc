#include <math.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

using namespace std;

void partTwo(const string& file_path) {
  ifstream file(file_path);
  string input;
  uint64_t invalid_id_sum = 0;
  regex re(R"((\d+)-(\d+))");
  smatch match;

  // Read the entire file as a string
  ostringstream buffer;
  buffer << file.rdbuf();
  input = buffer.str();

  // Regex parse
  auto begin = sregex_iterator(input.begin(), input.end(), re);
  auto end = sregex_iterator();
  for (auto it = begin; it != end; it++) {
    smatch m = *it;
    long long start = std::stoll(m[1]);
    long long end = std::stoll(m[2]);

    for (long long id = start; id <= end; id++) {
      string s = to_string(id);
      int N = s.size();
      for (int step = 1; step < N; step++) {
        string to_match = s.substr(0, step);
        if (N % step == 0) {
          int TOT_NUM_SEQUENCE = N / step;
          int current_num_seq = 1;
          for (int j = step; j <= N - step; j += step) {
            string to_check = s.substr(j, step);
            if (to_match == to_check) {
              current_num_seq++;
            } else {
              // if any non-repeating sequence then its valid
              break;
            }
          }

          if (current_num_seq == TOT_NUM_SEQUENCE) {
            invalid_id_sum += id;
            break;
          }
        }
      }
    }
  }

  cout << "PART2: " << invalid_id_sum << endl;
}

void partOne(const string& file_path) {
  ifstream file(file_path);
  string input;
  uint64_t invalid_id_sum = 0;
  regex re(R"((\d+)-(\d+))");
  smatch match;

  // Read the entire file as a string
  ostringstream buffer;
  buffer << file.rdbuf();
  input = buffer.str();

  // Regex parse
  auto begin = sregex_iterator(input.begin(), input.end(), re);
  auto end = sregex_iterator();
  for (auto it = begin; it != end; it++) {
    smatch m = *it;
    long long start = std::stoll(m[1]);
    long long end = std::stoll(m[2]);

    for (long long id = start; id <= end; id++) {
      string s = to_string(id);
      int N = s.size();
      if (N % 2 == 0) {
        int middle = N / 2;
        string first_half = s.substr(0, middle);
        string second_half = s.substr(middle);
        if (first_half == second_half) {
          invalid_id_sum += id;
        }
      }
    }
  }

  cout << "PART1: " << invalid_id_sum << endl;
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
