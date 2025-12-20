#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <regex>
#include <vector>

using namespace std;

int TILE_WIDTH = 101;
int TILE_HEIGHT = 103;

pair<int, int> GetPositionAfterSimulation(pair<int, int> &position,
                                          pair<int, int> &velocity, int step) {
  int x2 =
      (((position.first + velocity.first * step) % TILE_WIDTH) + TILE_WIDTH) %
      TILE_WIDTH;
  int y2 = (((position.second + velocity.second * step) % TILE_HEIGHT) +
            TILE_HEIGHT) %
           TILE_HEIGHT;
  return {x2, y2};
}

int GetQuadrantIndex(pair<int, int> &position) {
  int x_m = TILE_WIDTH / 2;
  int y_m = TILE_HEIGHT / 2;
  if (position.first >= 0 && x_m > position.first && position.second >= 0 &&
      position.second < y_m) {
    // First quad
    return 0;
  } else if (position.first > x_m && position.first < TILE_WIDTH &&
             position.second >= 0 && position.second < y_m) {
    // Second quad
    return 1;
  } else if (position.first >= 0 && position.first < x_m &&
             position.second > y_m && position.second < TILE_HEIGHT) {
    // Third quad
    return 2;
  } else if (position.first > x_m && position.first < TILE_WIDTH &&
             position.second > y_m && position.second < TILE_HEIGHT) {
    // Fourth quad
    return 3;
  }

  return -1;
}
int GetRowClusterSize(vector<char> &row) {
  bool cluster_found = false;
  int cluster_size = 0;
  int highest_cluster_size = 0;
  for (auto &val : row) {
    if (val != '.') {
      cluster_found = true;
    } else {
      cluster_found = false;
    }

    if (cluster_found)
      cluster_size++;
    else {
      highest_cluster_size = max(highest_cluster_size, cluster_size);
      cluster_size = 0;
      cluster_found = false;
    }
  }

  return highest_cluster_size;
}

void Print(vector<vector<char>> &map) {
  for (auto &row : map) {
    for (auto &ele : row) {
      cout << ele;
    }
    cout << endl;
  }
}

void PartTwo() {
  ifstream file("../input.txt");
  string input;
  regex re(R"(-?(\d+))");
  vector<pair<int, int>> positions;
  vector<pair<int, int>> velocities;
  while (getline(file, input)) {
    sregex_iterator it(input.begin(), input.end(), re);
    pair<int, int> position{stoi(it->str()), stoi((++it)->str())};
    pair<int, int> velocity{stoi((++it)->str()), stoi((++it)->str())};
    positions.push_back(position);
    velocities.push_back(velocity);
  }

  int MIN_CLUSTER_SIZE = 10;
  int second_now = 0;
  while (true) {
    vector<vector<char>> map(TILE_HEIGHT, vector<char>(TILE_WIDTH, '.'));
    for (size_t i = 0; i < positions.size(); i++) {
      pair<int, int> &p1 = positions[i];
      pair<int, int> &v = velocities[i];
      pair<int, int> p2 = GetPositionAfterSimulation(p1, v, second_now);
      map[p2.second][p2.first] = 'x';
    }

    int highest_cluster_size = 0;
    for (auto &row : map) {
      int cluster_size = GetRowClusterSize(row);
      // cout << cluster_size << endl;
      highest_cluster_size = max(highest_cluster_size, cluster_size);
    }

    if (highest_cluster_size >= MIN_CLUSTER_SIZE) {
      Print(map);
      break;
    }

    second_now++;
    // cout << "Current time: " << second_now << endl;
  }

  cout << "Answer: " << second_now << endl;
}
void PartOne() {
  ifstream file("../input.txt");
  string input;
  regex re(R"(-?(\d+))");
  vector<int> quadrant_totals(4, 0);
  while (getline(file, input)) {
    sregex_iterator it(input.begin(), input.end(), re);
    pair<int, int> position{stoi(it->str()), stoi((++it)->str())};
    pair<int, int> velocity{stoi((++it)->str()), stoi((++it)->str())};
    pair<int, int> p = GetPositionAfterSimulation(position, velocity, 100);
    int i = GetQuadrantIndex(p);
    if (i != -1)
      quadrant_totals[i]++;
  }
  int answer = accumulate(quadrant_totals.begin(), quadrant_totals.end(), 1,
                          multiplies<int>());
  cout << "Answer: " << answer << endl;
}

int main() {
  // PartOne();
  PartTwo();

  return 0;
}
