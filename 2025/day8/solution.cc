#include <math.h>

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <regex>
#include <string>

using namespace std;

struct Point3D {
  int64_t x;
  int64_t y;
  int64_t z;
};

bool operator<(const Point3D& a, const Point3D& b) {
  if (a.x != b.x) return a.x < b.x;
  if (a.y != b.y) return a.y < b.y;
  return a.z < b.z;
}

ostream& operator<<(ostream& os, const Point3D& point) {
  os << "(" << point.x << "," << point.y << "," << point.z << ")";
  return os;
}

struct Circuit {
  vector<Point3D> junctions;
};

double dist(const Point3D& p1, const Point3D& p2) {
  return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

pair<int, int> getMinDist(const vector<vector<double>>& distance_matrix) {
  int min_i = -1;
  int min_j = -1;
  double min_dist = numeric_limits<double>::infinity();
  for (size_t i = 0; i < distance_matrix.size(); i++) {
    auto& row = distance_matrix[i];
    for (size_t j = i + 1; j < distance_matrix[0].size(); j++) {
      double ele = row[j];
      if (ele < min_dist) {
        min_dist = ele;
        min_i = i;
        min_j = j;
      }
    }
  }

  return {min_i, min_j};
}

void partTwo(const string& file_path) {
  ifstream file(file_path);
  string input;
  uint64_t total = 0;

  regex re(R"((\d+),(\d+),(\d+))");

  // Data tracking
  vector<Point3D> points_3d;
  vector<vector<double>> distance_matrix;

  // Parse the input
  while (getline(file, input)) {
    smatch m;
    if (regex_match(input, m, re)) {
      int64_t x = stoi(m[1].str());
      int64_t y = stoi(m[2].str());
      int64_t z = stoi(m[3].str());
      points_3d.push_back({x, y, z});
    }
  }

  // Compute the distance matrix
  for (size_t i = 0; i < points_3d.size(); i++) {
    Point3D current_point = points_3d[i];
    vector<double> distance_row(points_3d.size(), 0);
    for (size_t j = i + 1; j < points_3d.size(); j++) {
      Point3D next_point = points_3d[j];
      distance_row[j] = dist(current_point, next_point);
    }
    distance_matrix.push_back(distance_row);
  }

  // Compute the circuits
  map<Point3D, int> circuit_map;
  vector<uint64_t> circuit_count(points_3d.size(), 0);
  vector<Circuit> circuits(points_3d.size());
  int NUM_JUNCTIONS = points_3d.size();
  Point3D node_next_last;
  Point3D node_last;
  int last_idx;
  while (true) {
    // Get the min dist
    pair<int, int> coord = getMinDist(distance_matrix);
    int i_min = coord.first;
    int j_min = coord.second;

    Point3D pi = points_3d[i_min];
    Point3D pj = points_3d[j_min];

    // Remove the current distance from the distance matrix
    distance_matrix[i_min][j_min] = numeric_limits<double>::infinity();

    // last updated entry

    // Connect junctions to circuits
    auto pi_it = circuit_map.find(pi);
    auto pj_it = circuit_map.find(pj);
    if (pi_it != circuit_map.end() && pj_it == circuit_map.end()) {
      last_idx = pi_it->second;
      circuits[pi_it->second].junctions.push_back(pj);
      circuit_count[pi_it->second]++;
      circuit_map[pj] = pi_it->second;
    } else if (pj_it != circuit_map.end() && pi_it == circuit_map.end()) {
      last_idx = pj_it->second;
      circuits[pj_it->second].junctions.push_back(pi);
      circuit_count[pj_it->second]++;
      circuit_map[pi] = pj_it->second;
    } else if (pj_it == circuit_map.end() && pi_it == circuit_map.end()) {
      last_idx = i_min;
      circuits[i_min].junctions.push_back(pi);
      circuits[i_min].junctions.push_back(pj);
      circuit_map[pi] = i_min;
      circuit_map[pj] = i_min;
      circuit_count[i_min] += 2;
    } else if (pj_it->second != pi_it->second) {
      last_idx = pi_it->second;
      circuit_count[pi_it->second] += circuit_count[pj_it->second];
      circuit_count[pj_it->second] = 0;

      // define move circuits
      Circuit& to_add = circuits[pi_it->second];
      Circuit& to_move = circuits[pj_it->second];

      // Update the map
      for (auto& junction : to_move.junctions) {
        circuit_map[junction] = pi_it->second;
      }

      // Add to new circuit and remove from the old tracked junctions
      to_add.junctions.insert(to_add.junctions.end(), to_move.junctions.begin(),
                              to_move.junctions.end());
      to_move.junctions.clear();
    } else {
      continue;
    }

    if (circuit_count[last_idx] == NUM_JUNCTIONS) {
      // All junctions are in one circuit
      node_last = pi;
      node_next_last = pj;
      break;
    }
  }

  // multiply the largest 3 circuits
  total += node_next_last.x * node_last.x;

  cout << "PART2: " << total << endl;
}

void partOne(const string& file_path, int N) {
  ifstream file(file_path);
  string input;
  uint64_t total = 0;

  regex re(R"((\d+),(\d+),(\d+))");

  // Data tracking
  vector<Point3D> points_3d;
  vector<vector<double>> distance_matrix;

  // Parse the input
  while (getline(file, input)) {
    smatch m;
    if (regex_match(input, m, re)) {
      int64_t x = stoi(m[1].str());
      int64_t y = stoi(m[2].str());
      int64_t z = stoi(m[3].str());
      points_3d.push_back({x, y, z});
    }
  }

  // Compute the distance matrix
  for (size_t i = 0; i < points_3d.size(); i++) {
    Point3D current_point = points_3d[i];
    vector<double> distance_row(points_3d.size(), 0);
    for (size_t j = i + 1; j < points_3d.size(); j++) {
      Point3D next_point = points_3d[j];
      distance_row[j] = dist(current_point, next_point);
    }
    distance_matrix.push_back(distance_row);
  }

  // Compute the circuits
  map<Point3D, int> circuit_map;
  vector<uint64_t> circuit_count(points_3d.size(), 0);
  vector<Circuit> circuits(points_3d.size());
  for (int i = 0; i < N; i++) {
    // Get the min dist
    pair<int, int> coord = getMinDist(distance_matrix);
    int i_min = coord.first;
    int j_min = coord.second;

    Point3D pi = points_3d[i_min];
    Point3D pj = points_3d[j_min];

    // Remove the current distance from the distance matrix
    distance_matrix[i_min][j_min] = numeric_limits<double>::infinity();

    // Connect junctions to circuits
    auto pi_it = circuit_map.find(pi);
    auto pj_it = circuit_map.find(pj);
    if (pi_it != circuit_map.end() && pj_it == circuit_map.end()) {
      circuits[pi_it->second].junctions.push_back(pj);
      circuit_count[pi_it->second]++;
      circuit_map[pj] = pi_it->second;
    } else if (pj_it != circuit_map.end() && pi_it == circuit_map.end()) {
      circuits[pj_it->second].junctions.push_back(pi);
      circuit_count[pj_it->second]++;
      circuit_map[pi] = pj_it->second;
    } else if (pj_it == circuit_map.end() && pi_it == circuit_map.end()) {
      circuits[i_min].junctions.push_back(pi);
      circuits[i_min].junctions.push_back(pj);
      circuit_map[pi] = i_min;
      circuit_map[pj] = i_min;
      circuit_count[i_min] += 2;
    } else if (pj_it->second != pi_it->second) {
      circuit_count[pi_it->second] += circuit_count[pj_it->second];
      circuit_count[pj_it->second] = 0;

      // define move circuits
      Circuit& to_add = circuits[pi_it->second];
      Circuit& to_move = circuits[pj_it->second];

      // Update the map
      for (auto& junction : to_move.junctions) {
        circuit_map[junction] = pi_it->second;
      }

      // Add to new circuit and remove from the old tracked junctions
      to_add.junctions.insert(to_add.junctions.end(), to_move.junctions.begin(),
                              to_move.junctions.end());
      to_move.junctions.clear();
    }
  }

  // sort the count map
  sort(begin(circuit_count), end(circuit_count), greater<uint64_t>());

  // multiply the largest 3 circuits
  total += accumulate(begin(circuit_count), begin(circuit_count) + 3, 1ULL,
                      multiplies<uint64_t>());

  cout << "PART1: " << total << endl;
}

int main() {
  string test_path = "test.txt";
  string my_path = "input.txt";

  cout << "========TEST OUTPUT========" << endl;
  partOne(test_path, 10);
  partTwo(test_path);

  cout << endl;

  cout << "=========MY OUTPUT=========" << endl;
  // partOne(my_path, 1000);
  partTwo(my_path);

  return 0;
};
