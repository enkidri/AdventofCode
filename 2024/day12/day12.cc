#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct Region {
  int area = 0;
  int perimeter = 0;
};

// The number of corners corresponds to the number of sides
int GetCorners(char region, const int i, const int j,
               const vector<vector<char>>& regions) {
  // Use lowercase letter as a mark to indicate that it has been visited. Add
  // to perimeter based on this information.
  vector<pair<int, int>> dir = {{-1, 0}, {0, -1}, {-1, 0}, {0, 1},
                                {0, -1}, {1, 0},  {0, 1},  {1, 0}};
  vector<pair<int, int>> diagonals = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
  int number_of_corners = 0;
  // Check concave and convex corners
  for (size_t idx = 0, idx1 = 0; idx < dir.size() - 1; idx += 2, idx1++) {
    pair<int, int>& dir1 = dir[idx];
    pair<int, int>& dir2 = dir[idx + 1];
    pair<int, int>& d = diagonals[idx1];
    bool first_concave;
    bool second_concave;
    bool is_diagonal;

    int ni = i + dir1.first;
    int nj = j + dir1.second;
    // cout << dir1.first << " " << dir1.second << endl;
    if (ni < 0 || ni >= regions.size() || nj < 0 || nj >= regions[0].size()) {
      first_concave = true;
    } else {
      first_concave = regions[ni][nj] != region;
    }

    int ndi = i + d.first;
    int ndj = j + d.second;
    if (ndi >= 0 && ndi < regions.size() && ndj >= 0 &&
        ndj < regions[0].size()) {
      is_diagonal = regions[ndi][ndj] != region;
    }

    int mi = i + dir2.first;
    int mj = j + dir2.second;
    if (mi < 0 || mi >= regions.size() || mj < 0 || mj >= regions[0].size()) {
      second_concave = true;
    } else {
      second_concave = regions[mi][mj] != region;
    }

    if ((first_concave && second_concave) ||
        (!first_concave && !second_concave && is_diagonal))
      number_of_corners++;
  }

  return number_of_corners;
}

int FreePerimeter(char region, int i, int j,
                  const vector<vector<char>>& regions) {
  int perimeter_length = 0;
  vector<pair<int, int>> dir = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  for (auto d : dir) {
    int di = d.first, dj = d.second;
    int ni = i + di, nj = j + dj;
    if (ni < regions.size() && ni >= 0 && nj < regions[0].size() && nj >= 0) {
      if (regions[ni][nj] != region) {
        perimeter_length++;
      }
    } else {
      perimeter_length++;
    }
  }

  return perimeter_length;
}

Region GetRegionData2(char region, int i, int j,
                      vector<vector<char>>& regions) {
  Region data;
  const vector<vector<char>> regions_copy = regions;
  int N = regions.size(), M = regions[0].size();
  queue<pair<int, int>> s;
  s.push({i, j});
  while (!s.empty()) {
    pair<int, int> p = s.front();
    int ci = p.first;
    int cj = p.second;
    s.pop();

    int left = cj, right = cj;
    // Move to the right and left as far as possible
    while (left >= 0 && regions[ci][left] == region) left--;
    while (right < M && regions[ci][right] == region) right++;

    for (int idx = left + 1; idx < right; idx++) {
      // Do something about regions[ci][idx]
      data.area++;
      data.perimeter += GetCorners(region, ci, idx, regions_copy);
      regions[ci][idx] = '-';
      // Check row below to fill
      if (ci < N - 1 && regions[ci + 1][idx] == region) s.push({ci + 1, idx});
      // Check row above to fill
      if (ci > 0 && regions[ci - 1][idx] == region) s.push({ci - 1, idx});
    }
  }

  return data;
}

Region GetRegionData(char region, int i, int j, vector<vector<char>>& regions) {
  Region data;
  vector<vector<char>> regions_copy = regions;  // Large copy but whatever
  int N = regions.size(), M = regions[0].size();
  queue<pair<int, int>> s;
  vector<pair<int, int>> passed;
  s.push({i, j});
  while (!s.empty()) {
    pair<int, int> p = s.front();
    int ci = p.first;
    int cj = p.second;
    s.pop();

    int left = cj, right = cj;
    // Move to the right and left as far as possible
    while (left >= 0 && regions[ci][left] == region) left--;
    while (right < M && regions[ci][right] == region) right++;

    for (int idx = left + 1; idx < right; idx++) {
      // Do something about regions[ci][idx]
      data.area++;
      data.perimeter += FreePerimeter(region, ci, idx, regions_copy);
      regions[ci][idx] = '-';
      // Check row below to fill
      if (ci < N - 1 && regions[ci + 1][idx] == region) s.push({ci + 1, idx});
      // Check row above to fill
      if (ci > 0 && regions[ci - 1][idx] == region) s.push({ci - 1, idx});
    }
  }

  return data;
}

void Printer(vector<vector<char>>& grid) {
  for (auto& row : grid) {
    for (auto& ele : row) {
      cout << ele;
    }
    cout << endl;
  }
  cout << endl;
}

void PartTwo() {
  ifstream file("input.txt");
  string input;
  vector<vector<char>> regions;

  while (getline(file, input)) {
    vector<char> line;
    for (char c : input) {
      line.push_back(c);
    }
    regions.push_back(line);
  }

  int score = 0;
  for (size_t i = 0; i < regions.size(); i++) {
    for (size_t j = 0; j < regions[0].size(); j++) {
      if (isalpha(regions[i][j])) {
        char c = regions[i][j];
        Region data = GetRegionData2(regions[i][j], i, j, regions);
        cout << c << " Area: " << data.area << " Perimeter: " << data.perimeter
             << endl;
        score += data.area * data.perimeter;
      }
    }
  }

  cout << "Answer: " << score << endl;
}

void PartOne() {
  ifstream file("input.txt");
  string input;
  vector<vector<char>> regions;

  while (getline(file, input)) {
    vector<char> line;
    for (char c : input) {
      line.push_back(c);
    }
    regions.push_back(line);
  }

  int score = 0;
  for (size_t i = 0; i < regions.size(); i++) {
    for (size_t j = 0; j < regions[0].size(); j++) {
      if (regions[i][j] != '-') {
        char c = regions[i][j];
        Region data = GetRegionData(regions[i][j], i, j, regions);
        cout << c << " Area: " << data.area << " Perimeter: " << data.perimeter
             << endl;
        score += data.area * data.perimeter;
      }
    }
  }

  cout << "Answer: " << score << endl;
}

int main() {
  /* PartOne(); */
  PartTwo();

  return 0;
}
