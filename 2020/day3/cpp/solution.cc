#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Index {
  int i;
  int j;

  Index operator+(const Index &other) const {
    return {i + other.i, j + other.j};
  }
};

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // Parse
  vector<string> map;
  while (getline(file, input)) {
    map.push_back(input);
  }

  // Solve
  size_t N = map.size();
  size_t M = map[0].size();
  vector<Index> moves = {{1, 1}, {1, 3}, {1, 5}, {1, 7}, {2, 1}};
  Index current{0, 0};
  vector<uint64_t> tree_counts;
  for (const auto &move : moves) {
    uint64_t count = 0;
    for (size_t i = 0; i < ((N - 1) / move.i); i++) {
      Index next = current + move;
      current = next;

      if (map.at(next.i).at(next.j % M) == '#') {
        count++;
      }
    }
    current = {0, 0};
    tree_counts.push_back(count);
  }

  answer = accumulate(tree_counts.begin(), tree_counts.end(), 1ULL,
                      multiplies<uint64_t>());

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << answer << " Time: " << elapsed.count() << " ms" << endl;
}

void partOne(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // Parse
  vector<string> map;
  while (getline(file, input)) {
    map.push_back(input);
  }

  // Solve
  size_t N = map.size();
  size_t M = map[0].size();
  Index current{0, 0};
  const Index move{1, 3};
  for (size_t i = 0; i < N - 1; i++) {
    Index next = current + move;
    current = next;

    if (map.at(next.i).at(next.j % M) == '#') {
      answer++;
    }
  }

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
