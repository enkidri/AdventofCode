#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  vector<size_t> x_indices;
  x_indices.reserve(36);
  uint64_t mask_or = 0;
  uint64_t addr, val;
  unordered_map<uint64_t, uint64_t> mem;
  while (getline(file, input)) {
    if (input[0] == 'm' && input[1] == 'a') {
      mask_or = 0;
      x_indices.clear();
      string mask = input.substr(7);
      for (size_t i = 0; i < mask.size(); i++) {
        char c = mask[i];
        mask_or = mask_or << 1;

        if (c == '1') {
          mask_or |= 1;
        } else if (c == 'X') {
          x_indices.push_back(mask.size() - 1 - i);
        }
      }
    } else {
      int N = x_indices.size();
      sscanf(input.c_str(), "mem[%ld] = %ld", &addr, &val);

      // apply rule 1 and 2
      uint64_t res = addr | mask_or;

      // Apply rule 3
      for (int p = 0; p < (1ULL << N); p++) {
        uint64_t new_addr = res;
        for (size_t i = 0; i < N; i++) {
          uint64_t bit = (p >> i) & 1;

          if (bit) {
            new_addr |= (1ULL << x_indices[i]);
          } else {
            new_addr &= ~(1ULL << x_indices[i]);
          }
        }
        mem[new_addr] = val;
      }
    }
  }

  answer =
      accumulate(mem.begin(), mem.end(), 0ULL,
                 [](uint64_t current_total, const pair<uint64_t, uint64_t> &p) {
                   return current_total + p.second;
                 });

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

  uint64_t mask_and = 0, mask_or = 0;
  uint64_t addr, val;
  unordered_map<uint64_t, uint64_t> mem;
  while (getline(file, input)) {
    if (input[0] == 'm' && input[1] == 'a') {
      mask_and = 0;
      mask_or = 0;
      string mask = input.substr(7);
      for (auto c : mask) {
        mask_and = mask_and << 1;
        mask_or = mask_or << 1;

        if (c == 'X') {
          mask_and |= 1;
        } else if (c == '1') {
          mask_or |= 1;
        }
      }
    } else {
      sscanf(input.c_str(), "mem[%ld] = %ld", &addr, &val);
      uint64_t result = (val & mask_and) | mask_or;
      mem[addr] = result;
    }
  }

  answer =
      accumulate(mem.begin(), mem.end(), 0ULL,
                 [](uint64_t current_total, const pair<uint64_t, uint64_t> &p) {
                   return current_total + p.second;
                 });

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
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
  cout << "Test: ";
  partTwo(test1_path);
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
