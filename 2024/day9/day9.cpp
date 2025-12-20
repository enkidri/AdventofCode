#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

int64_t ToInt(char c) { return c - '0'; }

char ToChar(int64_t i) { return i + '0'; }

struct FileUnit {
  int64_t size;
  int64_t a1;
  int64_t an;
  int64_t id;
};

void print(vector<FileUnit>& v) {
  for (auto& unit : v) {
    cout << "Size: " << unit.size << " a1: " << unit.a1 << " an: " << unit.an
         << " id: " << unit.id << endl;
  }
  cout << endl;
}

void partTwo() {
  string input;
  getline(ifstream("../input_day9.txt"), input);
  vector<FileUnit> occupied;
  vector<FileUnit> free;
  int64_t start_arr = 0;
  for (int i = 0, id = 0; i < input.size(); i += 2, id++) {
    int64_t file_size = input[i] - '0';
    int64_t a1 = start_arr, an = a1 + file_size - 1;
    FileUnit file = {file_size, a1, an, id};
    occupied.push_back(file);

    int64_t free_size = input[i + 1] - '0';
    a1 = an + 1;
    an = a1 + free_size - 1;
    file = {free_size, a1, an, -1};
    free.push_back(file);

    start_arr = an + 1;
  }
  free.pop_back();
  // print(occupied);
  // print(free);

  vector<FileUnit> occupied_in_free;
  while (true) {
    bool match_found = false;
    for (int i = 0; i < free.size(); i++) {
      int64_t free_size = free[i].size;
      if (free_size <= 0) continue;
      for (int j = occupied.size() - 1; j >= 0; j--) {
        int64_t file_size = occupied[j].size;
        if (file_size <= free_size && j >= i && file_size != 0) {
          // Add whats to be occupied ín the free space
          int64_t a1 = free[i].a1, an = a1 + file_size - 1;
          FileUnit to_be_occupied = {file_size, a1, an, occupied[j].id};
          occupied_in_free.push_back(to_be_occupied);

          // Adjust whats now free in the free space
          free[i].a1 = an + 1;
          free[i].size = free[i].an - free[i].a1 + 1;

          // Mark that an occupied object has been used
          occupied[j].size = 0;

          match_found = true;
          break;
        }
        if (match_found) break;
      }
    }

    // If we cant insert anything into a empty file space then we are finished
    if (!match_found) {
      break;
    }
  }

  // Caluclate the checksum for occupied
  int64_t checksum = 0;
  for (auto& file : occupied) {
    checksum += file.id * file.size * (file.a1 + file.an) / 2;
  }

  // Calculate the checksom for occupied in free space
  for (auto& file : occupied_in_free) {
    checksum += file.id * file.size * (file.a1 + file.an) / 2;
  }

  cout << "Answer: " << checksum << endl;
}

void partOne() {
  string input;
  getline(ifstream("../input_day9.txt"), input);
  input += '0';
  int64_t checksum = 0;

  int64_t i_end = input.size() - 2;
  int64_t end_size = ToInt(input[i_end]);
  int64_t start_arr = 0;
  for (int64_t i_start = 0, current_idx = 0;
       i_start < input.size() && i_end >= 0; i_start += 2, current_idx++) {
    cout << "i_start: " << i_start << " " << "current_idx: " << current_idx
         << endl;
    int64_t file_size = ToInt(input[i_start]);
    int64_t a1 = start_arr, an = a1 + file_size - 1;
    cout << a1 << " " << an << endl;
    checksum += current_idx * file_size * (a1 + an) / 2;
    cout << "Checksum: " << checksum << endl;
    if (file_size == 0 || i_end <= i_start) break;

    int64_t free_size = ToInt(input[i_start + 1]);
    end_size = ToInt(input[i_end]);
    cout << "free: " << free_size << " end: " << end_size << " i_end: " << i_end
         << endl;
    a1 = an + 1;
    int64_t reminder = free_size - end_size;
    cout << "reminder: " << reminder << endl;
    if (reminder <= 0) {
      an = a1 + free_size - 1;
      checksum += (i_end / 2) * free_size * (a1 + an) / 2;
      input[i_end] = ToChar(-reminder);
      cout << "end: " << ToInt(input[i_end]) << endl;
    } else {
      bool should_end = false;
      while (reminder > 0) {
        an = a1 + end_size - 1;
        checksum += (i_end / 2) * end_size * (a1 + an) / 2;
        input[i_end] = ToChar(0);
        cout << "checksum: " << checksum << endl;
        i_end -= 2;
        if (i_end <= i_start) {
          should_end = true;
          break;
        }
        end_size = ToInt(input[i_end]);
        reminder -= end_size;
        a1 = an + 1;
      }

      if (should_end) continue;

      reminder += end_size;
      an = a1 + reminder - 1;
      checksum += (i_end / 2) * reminder * (a1 + an) / 2;
      input[i_end] -= reminder;
    }
    cout << a1 << " " << an << endl;
    cout << "Checksum: " << checksum << endl;
    cout << endl;
    start_arr = an + 1;
  }

  cout << "Answer: " << checksum << endl;
}

int main() {
  //   partOne();
  partTwo();
  return 0;
}