#include <chrono>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // parse
  vector<uint64_t> bus_ids;
  vector<tuple<uint64_t, uint64_t>> bus_id;
  uint64_t goal_timestamp;
  getline(file, input);
  goal_timestamp = stoll(input);

  uint64_t current_num = -1;
  int count = 0;
  while (getline(file, input, ',')) {
    if (input[0] != 'x') {
      bus_ids.push_back(stoi(input));
    } else if (input[0] == 'x') {
      bus_ids.push_back(-1);
    }
  }

  // solve
  uint64_t time = 0;
  uint64_t step = bus_ids[0];
  for (size_t i = 1; i < bus_ids.size(); i++) {
    if (bus_ids[i] == -1) {
      continue;
    }
    uint64_t offset = i;
    while ((time + offset) % bus_ids[i] != 0) {
      time += step;
    }
    step *= bus_ids[i];
  }

  answer = time;

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

  vector<int> bus_id;
  int goal_timestamp;
  getline(file, input);
  goal_timestamp = stoi(input);
  while (getline(file, input, ',')) {
    if (input[0] != 'x') {
      bus_id.push_back(stoi(input));
    }
  }

  // solve
  int best_id = 0;
  int current_time_to_wait = INT32_MAX;
  for (const int id : bus_id) {
    int passed_time = goal_timestamp % id;
    int time_to_wait = id - passed_time;
    int next_time = goal_timestamp + time_to_wait;
    if (time_to_wait < current_time_to_wait) {
      current_time_to_wait = time_to_wait;
      best_id = id;
    }
  }

  answer = best_id * current_time_to_wait;

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
