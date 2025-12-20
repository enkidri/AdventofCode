#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct Registers {
  long long int A;
  long long int B;
  long long int C;
};

struct Computer {
  Registers regs{};
  vector<long long int> program;
  vector<long long int> output;

  long long int GetComboOperand(long long int input) {
    if (input <= 3 && input >= 0) {
      return input;
    } else if (input == 4) {
      return regs.A;
    } else if (input == 5) {
      return regs.B;
    } else if (input == 6) {
      return regs.C;
    } else if (input == 7) {
      cout << "Operand 7 reserverad" << endl;
      return -1;
    }

    cout << "Not valid input" << endl;
    return -1;
  }

  string GetOutput() {
    string output_string{};
    for (auto &val : output) {
      output_string += to_string(val) + ",";
    }
    if (!output_string.empty())
      output_string.pop_back();

    return output_string;
  }

  void PrintOutput() {
    for (auto &val : output) {
      cout << val << " ";
    }
    cout << endl;
  }

  void PrintRegisters() {
    cout << regs.A << endl;
    cout << regs.B << endl;
    cout << regs.C << endl;
  }

  void adv(long long int input) {
    regs.A = regs.A / (1 << GetComboOperand(input));
  }

  void bxl(long long int input) { regs.B = (regs.B ^ input); }

  void bst(long long int input) { regs.B = GetComboOperand(input) % 8; }

  long long int jnz(long long int input, long long int instruction_pointer) {
    if (regs.A != 0) {
      return input;
    }
    return instruction_pointer + 2;
  }

  void bxc(long long int input) { regs.B = regs.B ^ regs.C; }

  void out(long long int input) {
    output.push_back(GetComboOperand(input) % 8);
  }

  void bdv(long long int input) {
    regs.B = regs.A / (1 << GetComboOperand(input));
  }

  void cdv(long long int input) {
    regs.C = regs.A / (1 << GetComboOperand(input));
  }

  void ResetRun() { output.clear(); }

  void RunProgram() {
    long long int instruction_pointer = 0;
    while (instruction_pointer <= program.size() - 1) {
      // Switch case on the opcode
      long long int opcode = program[instruction_pointer];
      long long int value = program[instruction_pointer + 1];
      switch (opcode) {
      case 0:
        adv(value);
        instruction_pointer += 2;
        break;
      case 1:
        bxl(value);
        instruction_pointer += 2;
        break;
      case 2:
        bst(value);
        instruction_pointer += 2;
        break;
      case 3:
        instruction_pointer = jnz(value, instruction_pointer);
        break;
      case 4:
        bxc(value);
        instruction_pointer += 2;
        break;
      case 5:
        out(value);
        instruction_pointer += 2;
        break;
      case 6:
        bdv(value);
        instruction_pointer += 2;
        break;
      case 7:
        cdv(value);
        instruction_pointer += 2;
      default:
        break;
      }
    }
  }

  bool TestRunIsValid(const long long int &A_current, int instr_ptr) {
    long long int A_prev = regs.A;
    vector<long long int> program_prev = program;

    // Save previous values
    regs.A = A_current;
    program = {program.begin() + instr_ptr, program.end()};
    output.clear();

    RunProgram();
    bool test_ok = (program == output);

    // Restore previous program values
    regs.A = A_prev;
    program = program_prev;
    output.clear();

    return test_ok;
  }
};

void PartTwo() {
  ifstream file("../input.txt");
  string input;
  string temp;
  Computer pc{};

  // Parse
  getline(file, input);
  istringstream iss(input);
  iss >> temp >> temp >> pc.regs.A;
  getline(file, input);
  iss = istringstream(input);
  iss >> temp >> temp >> pc.regs.B;
  getline(file, input);
  iss = istringstream{input};
  iss >> temp >> temp >> pc.regs.C;

  getline(file, input);
  getline(file, input);
  regex re("\\d");
  auto it = sregex_iterator(input.begin(), input.end(), re);
  auto it_end = sregex_iterator();
  for (auto i = it; i != it_end; i++) {
    pc.program.push_back(stoi(i->str()));
  }

  // TEST
  // long long int A_test = 164278899142333;
  // pc.regs.A = A_test;
  // pc.RunProgram();
  // cout << pc.GetOutput() << endl;
  // pc.ResetRun();

  // Execute the program
  long long int A_min{INT64_MAX};
  stack<pair<long long int, int>> s;
  s.push({0, static_cast<int>(pc.program.size() - 1)});
  while (!s.empty()) {
    pair<long long int, int> curr_val = s.top();
    s.pop();
    long long int A_current = curr_val.first;
    int instr_ptr = curr_val.second;

    if (instr_ptr == -1) {
      // Do a test run
      if (pc.TestRunIsValid(A_current, 0)) {
        A_min = min(A_min, A_current);
      }
      continue;
    }

    for (long long int candidate = 0; candidate < 8; candidate++) {
      long long int A_new = A_current << 3;
      A_new = A_new | (candidate & 07);

      long long int B = A_new & 07;
      B = B ^ 1;
      long long int C = A_new >> B;
      B = B ^ 5;
      B = B ^ C;
      B = B & 07;
      if (B == pc.program[instr_ptr]) {
        s.push({A_new, instr_ptr - 1});
      }
    }
  }

  cout << "Program output:" << endl;
  pc.regs.A = A_min;
  pc.RunProgram();
  cout << pc.GetOutput() << endl;

  cout << "Answer: " << A_min << endl;
}

void PartOne() {
  ifstream file("../input.txt");
  string input;
  string temp;
  Computer pc{};
  vector<long long int> program;

  // Parse
  getline(file, input);
  istringstream iss(input);
  iss >> temp >> temp >> pc.regs.A;
  getline(file, input);
  iss = istringstream(input);
  iss >> temp >> temp >> pc.regs.B;
  getline(file, input);
  iss = istringstream{input};
  iss >> temp >> temp >> pc.regs.C;

  getline(file, input);
  getline(file, input);
  regex re("\\d");
  auto it = sregex_iterator(input.begin(), input.end(), re);
  auto it_end = sregex_iterator();
  for (auto i = it; i != it_end; i++) {
    program.push_back(stoi(i->str()));
  }

  // Execute the program
  long long int instruction_pointer = 0;
  while (instruction_pointer <= program.size() - 1) {
    // Switch case on the opcode
    long long int opcode = program[instruction_pointer];
    long long int value = program[instruction_pointer + 1];
    switch (opcode) {
    case 0:
      pc.adv(value);
      instruction_pointer += 2;
      break;
    case 1:
      pc.bxl(value);
      instruction_pointer += 2;
      break;
    case 2:
      pc.bst(value);
      instruction_pointer += 2;
      break;
    case 3:
      instruction_pointer = pc.jnz(value, instruction_pointer);
      break;
    case 4:
      pc.bxc(value);
      instruction_pointer += 2;
      break;
    case 5:
      pc.out(value);
      instruction_pointer += 2;
      break;
    case 6:
      pc.bdv(value);
      instruction_pointer += 2;
      break;
    case 7:
      pc.cdv(value);
      instruction_pointer += 2;
    default:
      break;
    }
  }

  pc.PrintRegisters();

  string answer = pc.GetOutput();
  if (!answer.empty())
    cout << "Answer: " << pc.GetOutput() << endl;
  else
    cout << "No output" << endl;
}

int main() {
  // PartOne();
  PartTwo();

  return 0;
}
