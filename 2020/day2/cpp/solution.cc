#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // Parse
  regex re_range(R"((\d+)-(\d+))");
  regex re_letter(R"()");
  while (getline(file, input)) {
    stringstream ss(input);
    string range;
    string letter;
    string password; // Password to check

    ss >> range >> letter >> password;

    // Parse the range
    int upper;
    int lower;

    smatch m;
    if (regex_search(range, m, re_range)) {
      lower = stoi(m[1].str());
      upper = stoi(m[2].str());
    }

    // Parse the target letter
    char target = letter[0];

    // Check if the password is valid
    answer += ((password.at(lower - 1) == target) !=
               (password.at(upper - 1) == target));
  }

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
  regex re_range(R"((\d+)-(\d+))");
  regex re_letter(R"()");
  while (getline(file, input)) {
    stringstream ss(input);
    string range;
    string letter;
    string password; // Password to check

    ss >> range >> letter >> password;

    // Parse the range
    int upper;
    int lower;

    smatch m;
    if (regex_search(range, m, re_range)) {
      lower = stoi(m[1].str());
      upper = stoi(m[2].str());
    }

    // Parse the target letter
    char target = letter[0];

    // Check if the password is valid
    int target_count = 0;
    for (auto c : password) {
      if (c == target) {
        target_count++;
      }
    }

    answer += (target_count >= lower && target_count <= upper);
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
