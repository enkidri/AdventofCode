#include <chrono>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class GameHandler {
public:
  void init(const string &file_path) {
    ifstream file(file_path);
    string input;

    char current_player_val;
    while (getline(file, input)) {
      if (input.size() >= 9) {
        current_player_val = input[7];
        continue;
      } else if (input.empty()) {
        continue;
      }

      if (current_player_val == '1') {
        deck1.push_back(stoi(input));
      } else if (current_player_val == '2') {
        deck2.push_back(stoi(input));
      }
    }
  }

  // perform one round
  void step() {
    int p1_top = deck1.front();
    deck1.pop_front();
    int p2_top = deck2.front();
    deck2.pop_front();
    // Player 1 win case
    if (p1_top > p2_top) {
      deck1.push_back(p1_top);
      deck1.push_back(p2_top);
    } else if (p1_top < p2_top) {
      deck2.push_back(p2_top);
      deck2.push_back(p1_top);
    } else {
      throw std::runtime_error("Scores are equal. Case not covered.");
    }
  }

  bool is_ended() noexcept { return deck1.empty() || deck2.empty(); }

  void start() {
    if (deck1.empty() || deck2.empty()) {
      throw std::runtime_error(
          "Start executation failed. Initialize deck before running.");
    }

    while (!is_ended()) {
      step();
    }
  }

  uint64_t compute_score() {
    if (deck1.empty() && deck2.empty()) {
      throw std::runtime_error(
          "Computation of score failed. Run the game before computing score.");
    }

    const deque<int> &deck = deck1.empty() ? deck2 : deck1;
    uint64_t score = 0;
    for (int i = deck.size() - 1, j = 1; i >= 0; i--, j++) {
      score += deck[i] * j;
    }

    return score;
  }

  void print() {
    cout << "Player 1:" << endl;
    for (const auto val : deck1) {
      cout << val << endl;
    }
    cout << endl;
    cout << "Player 2:" << endl;
    for (const auto val : deck2) {
      cout << val << endl;
    }
  }

private:
  deque<int> deck1;
  deque<int> deck2;
};

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  while (getline(file, input)) {
  }

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
}

void partOne(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  uint64_t answer = 0;

  GameHandler handler;
  handler.init(file_path);
  handler.start();
  answer = handler.compute_score();

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

  // cout << endl;

  // cout << "=========PART 2=========" << endl;
  // cout << "Test: ";
  // partTwo(test_path);
  // cout << "Input: ";
  // partTwo(my_path);

  return 0;
};
