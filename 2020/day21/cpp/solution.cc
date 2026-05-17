#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct IngredientsCount {
  unordered_map<string, int> table;
  int size = 0;
};

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  string answer;

  std::unordered_map<string, IngredientsCount> ingredients_table;
  const string split_key = " (contains ";
  while (getline(file, input)) {
    size_t split_pos = input.find(split_key);
    string ingredients_part = input.substr(0, split_pos);
    string allergens_part = input.substr(split_pos + split_key.size());
    allergens_part.pop_back();
    string temp;

    vector<string> ingredients;
    stringstream ss_ing(ingredients_part);
    while (ss_ing >> temp) {
      ingredients.push_back(temp);
    }

    vector<string> allergens;
    stringstream ss_all(allergens_part);
    while (getline(ss_all, temp, ',')) {
      temp.erase(0, temp.find_first_not_of(" "));
      for (const auto ingredient : ingredients) {
        ingredients_table[temp].table[ingredient]++;
      }
      ingredients_table[temp].size++;
    }
  }

  // actually solve the problem
  map<string, string> allergen_map;
  while (true) {
    vector<string> ingredients_to_remove;
    vector<string> allergen_to_remove;

    // Find what allergen <-> ingredients that can be uniquely identified
    for (const auto [allergen, counts_entry] : ingredients_table) {
      vector<string> ingredients_in_majority;
      int target_count = counts_entry.size;
      for (const auto [ingredient, count] : counts_entry.table) {
        if (count == target_count) {
          ingredients_in_majority.push_back(ingredient);
        }
      }

      if (ingredients_in_majority.size() == 1 ||
          counts_entry.table.size() == 1) {
        ingredients_to_remove.push_back(ingredients_in_majority.back());
        allergen_to_remove.push_back(allergen);
        allergen_map[allergen] = ingredients_in_majority.back();
      }
    }

    if (allergen_to_remove.empty() || ingredients_to_remove.empty()) {
      break;
    }

    // Remove the identified allergen <-> ingredient from tables
    for (const auto &allergen : allergen_to_remove) {
      ingredients_table.erase(allergen);
    }
    for (auto &[_, counts_entry] : ingredients_table) {
      for (const auto ingredient : ingredients_to_remove) {
        counts_entry.table.erase(ingredient);
      }
    }
  }

  for (const auto p : allergen_map) {
    answer += p.second + ",";
  }
  answer.pop_back();

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

  std::unordered_map<string, IngredientsCount> ingredients_table;
  std::unordered_map<string, int> ingredients_count;
  const string split_key = " (contains ";
  while (getline(file, input)) {
    size_t split_pos = input.find(split_key);
    string ingredients_part = input.substr(0, split_pos);
    string allergens_part = input.substr(split_pos + split_key.size());
    allergens_part.pop_back();
    string temp;

    vector<string> ingredients;
    stringstream ss_ing(ingredients_part);
    while (ss_ing >> temp) {
      ingredients.push_back(temp);
      ingredients_count[temp]++;
    }

    vector<string> allergens;
    stringstream ss_all(allergens_part);
    while (getline(ss_all, temp, ',')) {
      temp.erase(0, temp.find_first_not_of(" "));
      for (const auto ingredient : ingredients) {
        ingredients_table[temp].table[ingredient]++;
      }
      ingredients_table[temp].size++;
    }
  }

  // actually solve the problem
  while (true) {
    vector<string> ingredients_to_remove;
    vector<string> allergen_to_remove;

    // Find what allergen <-> ingredients that can be uniquely identified
    for (const auto [allergen, counts_entry] : ingredients_table) {
      vector<string> ingredients_in_majority;
      int target_count = counts_entry.size;
      for (const auto [ingredient, count] : counts_entry.table) {
        if (count == target_count) {
          ingredients_in_majority.push_back(ingredient);
        }
      }

      if (ingredients_in_majority.size() == 1 ||
          counts_entry.table.size() == 1) {
        ingredients_to_remove.push_back(ingredients_in_majority.back());
        allergen_to_remove.push_back(allergen);
      }
    }

    if (allergen_to_remove.empty() || ingredients_to_remove.empty()) {
      break;
    }

    // Remove the identified allergen <-> ingredient from tables
    for (const auto &ingredient : ingredients_to_remove) {
      ingredients_count.erase(ingredient);
    }
    for (const auto &allergen : allergen_to_remove) {
      ingredients_table.erase(allergen);
    }
    for (auto &[_, counts_entry] : ingredients_table) {
      for (const auto ingredient : ingredients_to_remove) {
        counts_entry.table.erase(ingredient);
      }
    }
  }

  for (const auto [allergen, count] : ingredients_count) {
    answer += count;
  }

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
