#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

uint64_t get_key(int x, int y, int z, int w = 0) {
  uint64_t k = (x + 50); // Use 16 bits per dimension
  k |= (uint64_t)(y + 50) << 16;
  k |= (uint64_t)(z + 50) << 32;
  k |= (uint64_t)(w + 50) << 48;
  return k;
}

tuple<int, int, int, int> get_coord(uint64_t k) {
  // 0xFFFF is the "mask" (16 bits of all 1s: 1111111111111111)
  int x = (int)(k & 0xFFFF) - 50;
  int y = (int)((k >> 16) & 0xFFFF) - 50;
  int z = (int)((k >> 32) & 0xFFFF) - 50;
  int w = (int)((k >> 48) & 0xFFFF) - 50;

  return {x, y, z, w};
}

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  int y = 0, x = 0;
  unordered_set<uint64_t> current_active_cube;
  while (getline(file, input)) {
    for (auto c : input) {
      if (c == '#') {
        current_active_cube.insert(get_key(x, y, 0, 0));
      }
      x++;
    }

    x = 0;
    y++;
  }

  // solve
  int n = 6;
  for (int _ = 0; _ < n; _++) {
    unordered_set<uint64_t> next_active_cube;
    next_active_cube.reserve(current_active_cube.size());
    unordered_map<uint64_t, int> inactive_neighbour_count;
    inactive_neighbour_count.reserve(current_active_cube.size() * 26);
    for (const auto &coord : current_active_cube) {
      // Check within a neighbourhood
      int num_active_neighbours = 0;
      auto [cx, cy, cz, cw] = get_coord(coord);
      for (int x = cx - 1; x <= cx + 1; x++) {
        for (int y = cy - 1; y <= cy + 1; y++) {
          for (int z = cz - 1; z <= cz + 1; z++) {
            for (int w = cw - 1; w <= cw + 1; w++) {
              uint64_t key = get_key(x, y, z, w);
              if (coord == key) {
                continue;
              }

              if (current_active_cube.count(key)) {
                num_active_neighbours++;
              } else {
                inactive_neighbour_count[key]++;
              }
            }
          }
        }
      }

      // Apply rule 1
      if (num_active_neighbours == 2 || num_active_neighbours == 3) {
        next_active_cube.insert(coord);
      }
    }

    // Apply rule 2
    for (const auto &pair : inactive_neighbour_count) {
      if (pair.second == 3) {
        next_active_cube.insert(pair.first);
      }
    }

    current_active_cube = std::move(next_active_cube);
  }

  answer = current_active_cube.size();

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

  int y = 0, x = 0;
  unordered_set<uint64_t> current_active_cube;
  while (getline(file, input)) {
    for (auto c : input) {
      if (c == '#') {
        current_active_cube.insert(get_key(x, y, 0));
      }
      x++;
    }

    x = 0;
    y++;
  }

  // solve
  int n = 6;
  for (int _ = 0; _ < n; _++) {
    unordered_set<uint64_t> next_active_cube;
    next_active_cube.reserve(current_active_cube.size());
    unordered_map<uint64_t, int> inactive_neighbour_count;
    inactive_neighbour_count.reserve(current_active_cube.size() * 26);
    for (const auto &coord : current_active_cube) {
      // Check within a neighbourhood
      int num_active_neighbours = 0;
      auto [cx, cy, cz, _] = get_coord(coord);
      for (int x = cx - 1; x <= cx + 1; x++) {
        for (int y = cy - 1; y <= cy + 1; y++) {
          for (int z = cz - 1; z <= cz + 1; z++) {
            uint64_t key = get_key(x, y, z);
            if (coord == key) {
              continue;
            }

            if (current_active_cube.count(key)) {
              num_active_neighbours++;
            } else {
              inactive_neighbour_count[key]++;
            }
          }
        }
      }

      // Apply rule 1
      if (num_active_neighbours == 2 || num_active_neighbours == 3) {
        next_active_cube.insert(coord);
      }
    }

    // Apply rule 2
    for (const auto &pair : inactive_neighbour_count) {
      if (pair.second == 3) {
        next_active_cube.insert(pair.first);
      }
    }

    current_active_cube = std::move(next_active_cube);
  }

  answer = current_active_cube.size();

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
