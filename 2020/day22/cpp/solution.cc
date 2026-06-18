#include <chrono>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>

using namespace std;

class GameState {
public:
  virtual ~GameState() = default;
  virtual void step() = 0;

  bool is_ended() noexcept {
    return deck1.empty() || deck2.empty() || loop_detected;
  }

  bool is_init() noexcept { return !deck1.empty() && !deck2.empty(); }

  bool deck1_is_winner() {
    // return true if deck1 is the winner, false otherwise
    if (!is_ended()) {
      throw std::runtime_error(
          "Winner check failed. Cannot check winner when game under progress");
    }
    return deck2.empty() || loop_detected;
  }

  string serialize_state() {
    string s;
    for (int x : deck1)
      s += to_string(x) + ",";
    s += "|";
    for (int x : deck2)
      s += to_string(x) + ",";
    return s;
  }

  void update_seen_states(const string &s) { seen_states.insert(s); }

  void print() {
    cout << "========" << endl;
    cout << "Player 1:" << endl;
    for (const auto val : deck1) {
      cout << val << endl;
    }
    cout << endl;
    cout << "Player 2:" << endl;
    for (const auto val : deck2) {
      cout << val << endl;
    }
    cout << "========" << endl;
  }

  deque<int> deck1;
  deque<int> deck2;
  string serialized_decks;
  unordered_set<string> seen_states;
  bool loop_detected = false;
};

class NormalGameState : public GameState {
public:
  void step() override {
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
};

class RecursiveGameState : public GameState {
public:
  void step() override {
    int p1_top = deck1.front();
    deck1.pop_front();
    int p2_top = deck2.front();
    deck2.pop_front();

    // prioritize the new recursive rules
    if (p1_top <= deck1.size() && p2_top <= deck2.size()) {
      // then we recurse!
      RecursiveGameState rstate;
      rstate.deck1.assign(deck1.begin(), deck1.begin() + p1_top);
      rstate.deck2.assign(deck2.begin(), deck2.begin() + p2_top);
      while (!rstate.is_ended()) {
        rstate.step();
      }
      // winner of the recursive game is decided
      if (rstate.deck1_is_winner()) {
        deck1.push_back(p1_top);
        deck1.push_back(p2_top);
      } else {
        deck2.push_back(p2_top);
        deck2.push_back(p1_top);
      }
      // the normal game rules here
    } else if (p1_top > p2_top) {
      deck1.push_back(p1_top);
      deck1.push_back(p2_top);
    } else if (p1_top < p2_top) {
      deck2.push_back(p2_top);
      deck2.push_back(p1_top);
    } else {
      throw std::runtime_error("Scores are equal. Case not covered.");
    }

    // update the history cache
    serialized_decks = serialize_state();
    if (seen_states.count(serialized_decks)) {
      loop_detected = true;
    }
    update_seen_states(serialized_decks);
  }
};

class GameHandler {
public:
  void init(const string &file_path) {
    if (state == nullptr) {
      throw std::runtime_error(
          "Start init failed. Initialize game state/type first.");
    }

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
        state->deck1.push_back(stoi(input));
      } else if (current_player_val == '2') {
        state->deck2.push_back(stoi(input));
      }
    }
  }

  void init_cache() { state->update_seen_states(state->serialize_state()); }

  void set_game_state(std::unique_ptr<GameState> new_state) {
    state = std::move(new_state);
  }

  void start() {
    if (!state->is_init()) {
      throw std::runtime_error(
          "Start executation failed. Initialize deck before running.");
    }

    while (!state->is_ended()) {
      state->step();
    }
  }

  uint64_t compute_score() {
    if (!state->is_ended()) {
      throw std::runtime_error(
          "Computation of score failed. Run the game before computing score.");
    }

    const deque<int> &deck = state->deck1.empty() ? state->deck2 : state->deck1;
    uint64_t score = 0;
    for (int i = deck.size() - 1, j = 1; i >= 0; i--, j++) {
      score += deck[i] * j;
    }

    return score;
  }

private:
  std::unique_ptr<GameState> state = nullptr;
};

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  GameHandler handler;
  handler.set_game_state(std::make_unique<RecursiveGameState>());
  handler.init(file_path);
  handler.init_cache();
  handler.start();
  answer = handler.compute_score();

  auto end = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed = end - start;
  cout << answer << " Time: " << fixed << setprecision(3) << elapsed.count()
       << " ms" << endl;
}

void partOne(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  uint64_t answer = 0;

  GameHandler handler;
  handler.set_game_state(std::make_unique<NormalGameState>());
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

  cout << endl;

  cout << "=========PART 2=========" << endl;
  cout << "Test: ";
  partTwo(test_path);
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
