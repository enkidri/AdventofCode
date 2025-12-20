#include <math.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <regex>
#include <set>
#include <string>

#include "ortools/linear_solver/linear_solver.h"

using operations_research::MPConstraint;
using operations_research::MPObjective;
using operations_research::MPSolver;
using operations_research::MPVariable;

using namespace std;

using ButtonPresses = vector<vector<int>>;

vector<string> generateBinaryPermutations(int N) {
  const int totalPermutations = std::pow(2, N);  // 2^4 = 16

  vector<string> permutations;
  // Loop through all 16 possible numbers (0 to 15)
  for (int i = 0; i < totalPermutations; ++i) {
    std::string permutation = "";

    // Check the bits from position N-1 down to 0 (to get the reading order)
    for (int j = N - 1; j >= 0; --j) {
      // Use the bitwise AND operator (i & (1 << j)) to check the j-th bit of i
      if (i & (1 << j)) {
        // Bit is 1
        permutation += '#';
      } else {
        // Bit is 0
        permutation += '.';
      }
    }

    permutations.push_back(permutation);
  }

  return permutations;
}

void partTwo(const string& file_path) {
  ifstream file(file_path);
  string input;
  int lowest_presses_sum = 0;

  regex re_inside_parenthesis(R"(\((.*?)\))");
  regex re_numbers(R"((\d+))");
  regex re_joltage(R"(\{(.*?)\})");

  while (getline(file, input)) {
    ButtonPresses buttons;
    vector<int> joltages;

    // Parse the button configuration
    sregex_iterator next(input.begin(), input.end(), re_inside_parenthesis);
    sregex_iterator end;
    while (next != end) {
      smatch inside = *next;
      string inside_str = inside[1].str();

      vector<int> button_config;

      sregex_iterator next_number(inside_str.begin(), inside_str.end(),
                                  re_numbers);
      sregex_iterator end;
      while (next_number != end) {
        smatch value = *next_number;
        button_config.push_back(stoi(value[1].str()));

        next_number++;
      }

      buttons.push_back(button_config);
      next++;
    }

    // Parse the joltage levels
    smatch m;
    string inside_clams;
    if (regex_search(input, m, re_joltage)) {
      inside_clams = m[1].str();
    }
    sregex_iterator next_number1(inside_clams.begin(), inside_clams.end(),
                                 re_numbers);
    sregex_iterator end1;
    while (next_number1 != end1) {
      smatch value = *next_number1;
      joltages.push_back(stoi(value[1].str()));
      next_number1++;
    }

    // Solve the problem
    int NUM_VARIABLES = buttons.size();
    MPSolver solver("Ax_equals_y", MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);
    const double INF = solver.infinity();

    // Define the variables
    MPVariable* x[NUM_VARIABLES];
    for (int i = 0; i < NUM_VARIABLES; i++) {
      x[i] = solver.MakeIntVar(0.0, INF, "x" + std::to_string(i + 1));
    }

    // Parse the constraints for the data
    map<int, vector<int>> eq_sys;
    for (size_t i = 0; i < buttons.size(); i++) {
      vector<int> button = buttons[i];
      for (size_t j = 0; j < button.size(); j++) {
        eq_sys[button[j]].push_back(i);
      }
    }

    // Set the constraints
    for (size_t i = 0; i < joltages.size(); i++) {
      double y = joltages[i];
      MPConstraint* c = solver.MakeRowConstraint(y, y);
      for (auto& params : eq_sys[i]) {
        c->SetCoefficient(x[params], 1);
      }
    }

    // Objective: minimize sum(x)
    MPObjective* objective = solver.MutableObjective();
    for (int i = 0; i < NUM_VARIABLES; ++i) {
      objective->SetCoefficient(x[i], 1);
    }
    objective->SetMinimization();

    // Run the optimizer
    const MPSolver::ResultStatus result_status = solver.Solve();
    if (result_status == MPSolver::OPTIMAL) {
      lowest_presses_sum += objective->Value();
    } else {
      cout << "No optimal solution found" << endl;
    }
  }

  cout << "PART2: " << lowest_presses_sum << endl;
}

void partOne(const string& file_path) {
  ifstream file(file_path);
  string input;
  int lowest_presses_sum = 0;

  regex re_lights(R"(\[([.#]+)\])");
  regex re_inside_parenthesis(R"(\((.*?)\))");
  regex re_numbers(R"((\d+))");

  while (getline(file, input)) {
    string lights;
    ButtonPresses buttons;

    // Parse the goal light configuration
    smatch m;
    if (regex_search(input, m, re_lights)) {
      lights = m[1].str();
    }

    // Parse the button configuration
    sregex_iterator next(input.begin(), input.end(), re_inside_parenthesis);
    sregex_iterator end;
    while (next != end) {
      smatch inside = *next;
      string inside_str = inside[1].str();

      vector<int> button_config;

      sregex_iterator next_number(inside_str.begin(), inside_str.end(),
                                  re_numbers);
      sregex_iterator end;
      while (next_number != end) {
        smatch value = *next_number;
        button_config.push_back(stoi(value[1].str()));

        next_number++;
      }

      buttons.push_back(button_config);
      next++;
    }

    // ----Solve the problem----
    map<string, vector<string>> states;  // Contains all the permutations of
                                         // the light and possible next states
    vector<string> permutations = generateBinaryPermutations(lights.size());

    // Fill up states with state -> next state pairs
    for (auto current_state : permutations) {
      // Add the next possible states based on the button presses
      for (const auto& button : buttons) {
        string next_state = current_state;
        for (const int& lamp_to_flip : button) {
          // this flips could have been more efficiently represented with a
          // binary value but whatever
          next_state[lamp_to_flip] =
              (next_state[lamp_to_flip] == '#') ? '.' : '#';
        }
        states[current_state].push_back(next_state);
      }
    }

    // BFS
    int min_button_presses = INT32_MAX;
    int goal_found = false;
    string goal_state(lights.size(), '.');
    set<string> visited;
    queue<pair<string, int>> to_visit;  // state and number of presses
    to_visit.push({lights, 0});
    visited.insert(lights);
    while (!to_visit.empty()) {
      // Retrieve current
      auto [current_state, num_presses] = to_visit.front();
      to_visit.pop();

      // Update state
      if (current_state == goal_state) {
        if (num_presses < min_button_presses) {
          min_button_presses = num_presses;
        }
        goal_found = true;
        continue;
      }

      // Mark as visited
      visited.insert(current_state);

      for (auto& next : states[current_state]) {
        if (visited.find(next) == visited.end()) {
          to_visit.push({next, num_presses + 1});
        }
      }
    }

    if (goal_found) {
      lowest_presses_sum += min_button_presses;
    } else {
      cout << "Goal couldnt be found" << endl;
    }
  }

  cout << "PART1: " << lowest_presses_sum << endl;
}

int main() {
  string test_path = "test.txt";
  string my_path = "input.txt";

  cout << "========TEST OUTPUT========" << endl;
  partOne(test_path);
  partTwo(test_path);

  cout << endl;

  cout << "=========MY OUTPUT=========" << endl;
  partOne(my_path);  // Takes like 1 min to run...
  partTwo(my_path);

  return 0;
};
