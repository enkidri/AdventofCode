#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Region {
  int area = 0;
  int perimeter = 0;
};

bool IsOccupiedPosition(char current_region, int i, int j,
                        vector<vector<char>> &regions) {
  if (i < regions.size() && j < regions[0].size() && i >= 0 && j >= 0) {
    if (regions[i][j] == current_region) {
      return true;
    }
  }
  return false;
}

void PartOne() {
  ifstream file("../input.txt");
  string input;
  vector<vector<char>> regions;
  map<char, Region> region_data;

  int i = 0;
  while (getline(file, input)) {
    vector<char> region_line;
    for (size_t j = 0; j < input.size(); j++) {
      region_line.push_back(input[j]);

      // Add to area size
      region_data[input[j]].area++;
      // Add to perimeter size
      int perimeter_size = 4;
      if (IsOccupiedPosition(input[j], i - 1, j, regions)) {
        perimeter_size--;
        region_data[input[j]].perimeter--;
      }
      if (IsOccupiedPosition(input[j], i, j - 1, regions)) {
        perimeter_size--;
        region_data[input[j]].perimeter--;
      }
      region_data[input[j]].perimeter += perimeter_size;
    }
    regions.push_back(region_line);
    i++;
  }

  int score = 0;
  for (auto &data : region_data) {
    Region &info = data.second;
    cout << data.first << " " << info.area << " " << info.perimeter << endl;
    score += info.area * info.perimeter;
  }

  cout << "Answer: " << score << endl;
}

int main() {
  PartOne();

  return 0;
}
