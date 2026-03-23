#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

using Side = uint16_t;

struct Tile {
  int index;
  Side top = 0;
  Side right = 0;
  Side bottom = 0;
  Side left = 0;
  vector<string> tile_buffer;

  friend ostream &operator<<(ostream &os, const Tile &tile) {
    for (const auto &row : tile.tile_buffer) {
      os << row << "\n";
    }
    return os;
  }
};

uint16_t reverse_bits(uint16_t n) {
  int ans = 0;
  for (int _ = 0; _ < 10; _++) {
    ans <<= 1;
    ans |= n & 1;
    n >>= 1;
  }
  return ans;
}

Tile flip_tile(const Tile &tile) {
  Tile flipped = tile; // Copy the whole tile (index, buffer, etc.)

  // Flip each row in the buffer
  for (std::string &row : flipped.tile_buffer) {
    // std::reverse is optimized and safe
    std::reverse(row.begin(), row.end());
  }

  // Update the edge metadata
  flipped.top = reverse_bits(tile.top);
  flipped.bottom = reverse_bits(tile.bottom);

  // Left and Right swap sides
  flipped.left = tile.right;
  flipped.right = tile.left;

  return flipped;
}

Tile rotate_tile(const Tile &tile, int num_rot) {
  // num_rot : the number of clockwise rotation
  assert(tile.tile_buffer.size() == tile.tile_buffer[0].size());

  int n = tile.tile_buffer.size();
  Tile current_tile = tile;
  vector<string> new_buffer;
  for (int _ = 0; _ < num_rot; _++) {
    for (int i = 0; i < n; i++) {
      string row;
      for (int j = n - 1; j >= 0; j--) {
        char c = current_tile.tile_buffer.at(j).at(i);
        row.push_back(c);
      }
      new_buffer.push_back(row);
    }
    current_tile.tile_buffer = std::move(new_buffer);
    new_buffer.clear();
    // Move the edges
    // Inside the for (int r = 0; r < num_rot; r++) loop:
    uint16_t old_top = current_tile.top;
    uint16_t old_right = current_tile.right;
    uint16_t old_bottom = current_tile.bottom;
    uint16_t old_left = current_tile.left;

    current_tile.top = reverse_bits(old_left); // Left (T->B) becomes Top (L->R)
    current_tile.right = old_top; // Top (L->R) becomes Right (T->B)
    current_tile.bottom =
        reverse_bits(old_right);    // Right (T->B) becomes Bottom (L->R)
    current_tile.left = old_bottom; // Bottom (L->R) becomes Left (T->B)
  }

  return current_tile;
}

bool dragon_in_window(const vector<string> &image,
                      const vector<pair<int, int>> &sea_monster, int i, int j) {
  int window_width = 20;
  int window_height = 3;
  if (i + window_height >= image.size() ||
      j + window_width >= image[0].size()) {
    return false;
  }

  for (const auto &coord : sea_monster) {
    auto [di, dj] = coord;
    if (image[i + di][j + dj] != '#') {
      return false;
    }
  }

  return true;
}

void partTwo(const string &file_path) {
  auto start = chrono::high_resolution_clock::now();
  ifstream file(file_path);
  string input;
  uint64_t answer = 0;

  vector<string> to_parse;
  vector<Tile> tiles;
  Tile tile;
  unordered_map<uint16_t, vector<int>> edge_map;
  unordered_map<int, Tile> tile_map;
  while (getline(file, input)) {
    if (input.empty()) {
      // Parse the top and the bottom
      for (int j = 0; j < to_parse[0].size(); j++) {
        if (to_parse[0][j] == '#')
          tile.top |= 1 << j;
        if (to_parse[to_parse.size() - 1][j] == '#') {
          tile.bottom |= 1 << j;
        }
      }

      // Parse the sides
      for (int i = 0; i < to_parse.size(); i++) {
        if (to_parse[i][0] == '#') {
          tile.left |= 1 << i;
        }

        if (to_parse[i].back() == '#') {
          tile.right |= 1 << i;
        }
      }
      tile.tile_buffer = to_parse;
      tiles.push_back(tile);
      tile_map[tile.index] = tile;

      // update edge map
      uint16_t side_patterns[4] = {tile.top, tile.bottom, tile.right,
                                   tile.left};
      for (uint16_t pattern : side_patterns) {
        edge_map[pattern].push_back(tile.index);
        uint16_t rev = reverse_bits(pattern);
        if (rev != pattern) {
          edge_map[rev].push_back(tile.index);
        }
      }

      // Clear states
      tile = {};
      to_parse.clear();
      continue;
    }

    if (input[0] == 'T') {
      tile.index = stoi(input.substr(5, 4));
    } else {
      to_parse.push_back(input);
    }
  }

  // find corners
  vector<const Tile *> corner_index;
  for (const Tile &tile : tiles) {
    // check each edge
    int outside_edge = 0;

    uint16_t side_patterns[4] = {tile.top, tile.bottom, tile.right, tile.left};
    for (uint16_t pattern : side_patterns) {
      if (edge_map[pattern].size() == 1) {
        outside_edge++;
      }
    }

    // check conditions
    if (outside_edge == 2) {
      corner_index.push_back(&tile);
    }
  }

  // Create image data structure
  int n = sqrt(tiles.size());
  vector<vector<Tile>> image(n, vector<Tile>(n, Tile{}));

  // define upper left corner to start with
  Tile start_corner =
      *corner_index[0]; // build the image from this anchor point

  // find the unmatched corners
  uint16_t side_patterns[4] = {start_corner.top, start_corner.right,
                               start_corner.bottom, start_corner.left};
  size_t i_target = 3;
  for (size_t i = 0; i < 4; i++) {
    size_t j = (i + 1) % 4;
    if (edge_map[side_patterns[i]].size() == 1 &&
        edge_map[side_patterns[j]].size() == 1) {
      // rotate here
      start_corner = rotate_tile(start_corner, i_target - i);
    }
  }

  // set the first corner and fill out the first row by connecting the right
  // edge with the left edge of the next tile
  image[0][0] = start_corner;
  for (int i = 0; i < n - 1; i++) {
    Tile &current_tile = image[0][i];
    if (edge_map[current_tile.right].size() != 2) {
      throw invalid_argument("the right edge should have a neighbour");
    }
    // find the next tile
    Tile next_tile;
    for (auto tile_index : edge_map.at(current_tile.right)) {
      if (tile_index != current_tile.index) {
        next_tile = tile_map.at(tile_index);
      }
    }

    // rotate or flip next tile to match
    bool match_found = false;
    for (int flip = 0; flip < 2; flip++) {
      for (int num_rot = 0; num_rot < 4; num_rot++) {
        if (next_tile.left == current_tile.right) {
          match_found = true;
          break;
        } else {
          next_tile = rotate_tile(next_tile, 1);
        }
      }
      if (match_found)
        break;
      next_tile = flip_tile(next_tile);
    }
    image[0][i + 1] = next_tile;
  }

  // fill out the rest of the image
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n; j++) {
      Tile &current = image[i][j];
      if (edge_map[current.bottom].size() != 2) {
        throw invalid_argument("the bottom edge should have a neighbour");
      }
      // find the next tile
      Tile next_tile;
      for (auto tile_index : edge_map[current.bottom]) {
        if (tile_index != current.index) {
          next_tile = tile_map[tile_index];
        }
      }

      // rotate or flip next tile to match
      bool match_found = false;
      for (int flip = 0; flip < 2; flip++) {
        for (int num_rot = 0; num_rot < 4; num_rot++) {
          if (next_tile.top == current.bottom) {
            match_found = true;
            break;
          } else {
            next_tile = rotate_tile(next_tile, 1);
          }
        }
        if (match_found)
          break;
        next_tile = flip_tile(next_tile);
      }
      image[i + 1][j] = next_tile;
      // cout << endl;
      // cout << current;
      // cout << endl;
      // cout << next_tile;
      // cout << endl;
    }
  }

  // // assemble the entire image
  int tile_size = image[0][0].tile_buffer.size();
  vector<string> full_image;
  for (int r = 0; r < n; r++) {
    for (int i = 1; i < tile_size - 1; i++) {
      string row_str;
      for (int c = 0; c < n; c++) {
        row_str += image[r][c].tile_buffer[i].substr(1, tile_size - 2);
      }
      full_image.push_back(row_str);
    }
  }

  // debug view the full_image
  // cout << "full image size: " << full_image.size() << " "
  //      << full_image[0].size() << endl;
  // cout << endl;
  // Tile big_image;
  // big_image.tile_buffer = full_image;
  // big_image = flip_tile(big_image);
  // big_image = rotate_tile(big_image, 3);
  // for (auto r : big_image.tile_buffer) {
  //   cout << r << endl;
  // }

  // find the sea monster in the image
  Tile image_tile;
  image_tile.tile_buffer = full_image;
  vector<pair<int, int>> sea_monster = {
      {0, 18}, {1, 0}, {1, 5}, {1, 6}, {1, 11}, {1, 12}, {1, 17}, {1, 18},
      {1, 19}, {2, 1}, {2, 4}, {2, 7}, {2, 10}, {2, 13}, {2, 16}};
  for (int flips = 0; flips < 2; flips++) {
    for (int num_rot = 0; num_rot < 4; num_rot++) {
      // explore current rotation
      int water_roughness = 0;
      int num_dragons = 0;
      for (int i = 0; i < image_tile.tile_buffer.size(); i++) {
        for (int j = 0; j < image_tile.tile_buffer[0].size(); j++) {
          if (image_tile.tile_buffer.at(i).at(j) == '#') {
            water_roughness++;
          }
          if (dragon_in_window(image_tile.tile_buffer, sea_monster, i, j)) {
            num_dragons++;
          }
        }
      }

      if (num_dragons > 0) {
        answer = water_roughness - num_dragons * sea_monster.size();
        break;
      }

      // try next rotation
      image_tile = rotate_tile(image_tile, 1);
    }
    if (answer != 0) {
      break;
    }
    // try flip
    image_tile = flip_tile(image_tile);
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
  uint64_t answer = 1;

  vector<string> to_parse;
  vector<Tile> tiles;
  Tile tile;
  unordered_map<uint16_t, vector<int>> edge_map;
  while (getline(file, input)) {
    if (input.empty()) {
      // Parse the top and the bottom
      for (int j = 0; j < to_parse[0].size(); j++) {
        if (to_parse[0][j] == '#')
          tile.top |= 1 << j;
        if (to_parse[to_parse.size() - 1][j] == '#') {
          tile.bottom |= 1 << j;
        }
      }

      // Parse the sides
      for (int i = 0; i < to_parse.size(); i++) {
        if (to_parse[i][0] == '#') {
          tile.left |= 1 << i;
        }

        if (to_parse[i].back() == '#') {
          tile.right |= 1 << i;
        }
      }
      tiles.push_back(tile);

      // update edge map
      uint16_t side_patterns[4] = {tile.top, tile.bottom, tile.right,
                                   tile.left};
      for (uint16_t pattern : side_patterns) {
        edge_map[pattern].push_back(tile.index);
        uint16_t rev = reverse_bits(pattern);
        if (rev != pattern) {
          edge_map[rev].push_back(tile.index);
        }
      }

      // Clear states
      tile = {};
      to_parse.clear();
      continue;
    }

    if (input[0] == 'T') {
      tile.index = stoi(input.substr(5, 4));
    } else {
      to_parse.push_back(input);
    }
  }

  // solve
  for (const Tile &tile : tiles) {
    // check each edge
    int outside_edge = 0;

    uint16_t side_patterns[4] = {tile.top, tile.bottom, tile.right, tile.left};
    for (uint16_t pattern : side_patterns) {
      if (edge_map[pattern].size() == 1) {
        outside_edge++;
      }
    }

    // check conditions
    if (outside_edge == 2) {
      answer *= tile.index;
    }
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
  // partOne(test_path);
  // cout << "Input: ";
  // partOne(my_path);

  cout << endl;

  cout << "=========PART 2=========" << endl;
  cout << "Test: ";
  partTwo(test_path);
  cout << "Input: ";
  partTwo(my_path);

  return 0;
};
