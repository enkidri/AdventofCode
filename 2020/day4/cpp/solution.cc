#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <utility>
#include <vector>

using namespace std;

bool is_ok_byr(const string &input) {
  if (input.size() == 4) {
    for (char c : input) {
      if (!isdigit(c))
        return false;
    }
  } else {
    return false;
  }
  return (stoi(input) >= 1920 && stoi(input) <= 2002);
}

bool is_ok_iyr(const string &input) {
  if (input.size() == 4) {
    for (char c : input) {
      if (!isdigit(c))
        return false;
    }
  } else {
    return false;
  }
  return (stoi(input) >= 2010 && stoi(input) <= 2020);
}

bool is_ok_eyr(const string &input) {
  if (input.size() == 4) {
    for (char c : input) {
      if (!isdigit(c))
        return false;
    }
  } else {
    return false;
  }
  return (stoi(input) >= 2020 && stoi(input) <= 2030);
}

bool is_ok_hgt(const string &input) {
  if (input.size() >= 4 && input.size() <= 5) {
    string unit = input.substr(input.size() - 2, 2);
    string number = input.substr(0, input.size() - 2);
    if (unit == "cm") {
      if (!(stoi(number) >= 150 && stoi(number) <= 193)) {
        return false;
      }
    } else if (unit == "in") {
      if (!(stoi(number) >= 59 && stoi(number) <= 76)) {
        return false;
      }
    } else {
      return false;
    }
  } else {
    return false;
  }

  return true;
}

bool is_ok_hcl(const string &input) {
  if (input[0] == '#' && input.size() == 7) {
    for (int i = 1; i < input.size(); i++) {
      char c = input[i];
      if (!((c >= 48 && c <= 57) || (c >= 97 && c <= 122))) {
        return false;
      }
    }
  } else {
    return false;
  }
  return true;
}

bool is_ok_ecl(const string &input) {
  static vector<string> to_match = {"amb", "blu", "brn", "gry",
                                    "grn", "hzl", "oth"};
  return find(to_match.begin(), to_match.end(), input) != to_match.end();
}

bool is_ok_pid(const string &input) {
  if (input.size() == 9) {
    for (const auto &c : input) {
      if (!(c >= 48 && c <= 57)) {
        return false;
      }
    }
  } else {
    return false;
  }
  return true;
}

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  // Parse
  regex re(R"(([^\s]+):([^\s]+))");
  vector<map<string, string>> maps;
  map<string, string> map;
  while (getline(file, input)) {
    if (input.empty()) {
      maps.push_back(map);
      map.clear();
    }

    auto start = sregex_iterator(input.begin(), input.end(), re);
    auto end = sregex_iterator();
    for (auto it = start; it != end; it++) {
      map[(*it)[1].str()] = (*it)[2].str();
    }
  }
  // Add the final map
  maps.push_back(map);

  // Check if valid
  const vector<string> required_keys = {"byr", "iyr", "eyr", "hgt",
                                        "hcl", "ecl", "pid"};
  string optional_key = "cid";
  for (const auto &map : maps) {
    bool required_keys_present = true;
    for (const auto &key : required_keys) {
      if (!map.count(key)) {
        required_keys_present = false;
        break;
      }
    }

    bool is_valid_format = true;
    if (required_keys_present) {
      for (const auto &pair : map) {
        if (pair.first == "byr") {
          if (!(is_ok_byr(pair.second))) {
            is_valid_format = false;
            break;
          }
        } else if (pair.first == "iyr") {
          if (!(is_ok_iyr(pair.second))) {
            is_valid_format = false;
            break;
          }
        } else if (pair.first == "eyr") {
          if (!(is_ok_eyr(pair.second))) {
            is_valid_format = false;
            break;
          }
        } else if (pair.first == "hgt") {
          if (!(is_ok_hgt(pair.second))) {
            is_valid_format = false;
            break;
          }
        } else if (pair.first == "hcl") {
          if (!(is_ok_hcl(pair.second))) {
            is_valid_format = false;
            break;
          }
        } else if (pair.first == "ecl") {
          if (!(is_ok_ecl(pair.second))) {
            is_valid_format = false;
            break;
          }
        } else if (pair.first == "pid") {
          if (!(is_ok_pid(pair.second))) {
            is_valid_format = false;
            break;
          }
        }
      }
    }

    if (is_valid_format && required_keys_present) {
      answer++;
    }
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
  regex re(R"(([^\s]+):([^\s]+))");
  vector<map<string, string>> maps;
  map<string, string> map;
  while (getline(file, input)) {
    if (input.empty()) {
      maps.push_back(std::move(map));
      map.clear();
    }

    auto start = sregex_iterator(input.begin(), input.end(), re);
    auto end = sregex_iterator();
    for (auto it = start; it != end; it++) {
      map[(*it)[1].str()] = (*it)[2].str();
    }
  }
  // Add the final map
  maps.push_back(std::move(map));

  // Check if valid
  const vector<string> required_keys = {"byr", "iyr", "eyr", "hgt",
                                        "hcl", "ecl", "pid"};
  string optional_key = "cid";
  for (const auto &map : maps) {
    bool required_keys_present = true;
    for (const auto &key : required_keys) {
      if (!map.count(key)) {
        required_keys_present = false;
        break;
      }
    }

    if (required_keys_present) {
      answer++;
    }
  }

  auto end = chrono::high_resolution_clock::now();
  auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
  cout << answer << " Time: " << elapsed.count() << " ms" << endl;
}

int main() {
  string test_path = "test.txt";
  string test1_path = "test1.txt";
  string test2_path = "test2.txt";
  string my_path = "input.txt";

  cout << "========PART 1========" << endl;
  cout << "Test: ";
  partOne(test_path);
  cout << "Input: ";
  partOne(my_path);

  cout << endl;

  cout << "=========PART 2=========" << endl;
  cout << "Test 1: ";
  partTwo(test1_path); // Should return 0
  cout << "Test 2: ";
  partTwo(test2_path); // Should return 4
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
