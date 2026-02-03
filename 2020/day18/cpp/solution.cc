#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  auto do_op = [](stack<uint64_t> &num, stack<char> &ops) {
    char op = ops.top();
    ops.pop();
    uint64_t right = num.top();
    num.pop();
    uint64_t left = num.top();
    num.pop();
    uint64_t result = (op == '*') ? left * right : left + right;
    num.push(result);
  };
  while (getline(file, input)) {
    stack<uint64_t> num;
    stack<char> ops;
    for (char c : input) {
      if (isdigit(c)) {
        num.push(c - '0');
      } else if (c == '(') {
        ops.push(c);
      } else if (c == ')') {
        while (ops.top() != '(') {
          do_op(num, ops);
        }
        ops.pop();
      } else if (c == '+' || c == '*') {
        while (!ops.empty() && ops.top() != '(' && ops.top() == '+') {
          do_op(num, ops);
        }
        ops.push(c);
      }
    }
    while (!ops.empty())
      do_op(num, ops);

    answer += num.top();
  }

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
  auto do_op = [](stack<uint64_t> &num, stack<char> &ops) {
    char op = ops.top();
    ops.pop();
    uint64_t right = num.top();
    num.pop();
    uint64_t left = num.top();
    num.pop();
    uint64_t result = (op == '*') ? left * right : left + right;
    num.push(result);
  };
  while (getline(file, input)) {
    stack<uint64_t> num;
    stack<char> ops;
    for (char c : input) {
      if (isdigit(c)) {
        num.push(c - '0');
      } else if (c == '(') {
        ops.push(c);
      } else if (c == ')') {
        while (ops.top() != '(') {
          do_op(num, ops);
        }
        ops.pop();
      } else if (c == '+' || c == '*') {
        while (!ops.empty() && ops.top() != '(') {
          do_op(num, ops);
        }
        ops.push(c);
      }
    }
    while (!ops.empty())
      do_op(num, ops);

    answer += num.top();
  }

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
}

int main() {
  string test_path = "test.txt";
  string test_path1 = "test1.txt";
  string test_path2 = "test2.txt";
  string test_path3 = "test3.txt";
  string test_path4 = "test4.txt";
  string my_path = "input.txt";

  cout << "========PART 1========" << endl;
  cout << "Test: ";
  partOne(test_path);
  partOne(test_path1);
  partOne(test_path2);
  partOne(test_path3);
  partOne(test_path4);
  cout << "Input: ";
  partOne(my_path);

  cout << endl;

  cout << "=========PART 2=========" << endl;
  cout << "Test: ";
  partTwo(test_path);
  partTwo(test_path1);
  partTwo(test_path2);
  partTwo(test_path3);
  partTwo(test_path4);
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
