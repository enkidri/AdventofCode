#include <math.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

using namespace std;

struct Point2D {
  int64_t x;
  int64_t y;

  bool operator==(const Point2D& other) const {
    return (x == other.x) && (y == other.y);
  }

  bool operator<(const Point2D& other) const {
    if (x != other.x) return x < other.x;
    return y < other.y;
  }
};

ostream& operator<<(ostream& os, const Point2D& point) {
  os << "(" << point.x << "," << point.y << ")";
  return os;
}

bool pointOnBorder(const Point2D& p, const Point2D& a, const Point2D& b) {
  // Check collinearity via cross product = 0
  int64_t cross = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
  if (abs(cross) != 0) return false;

  // Check the boxed line segment
  int64_t x_min = min(a.x, b.x);
  int64_t x_max = max(a.x, b.x);
  int64_t y_min = min(a.y, b.y);
  int64_t y_max = max(a.y, b.y);

  return (p.x >= x_min && p.x <= x_max && p.y <= y_max && p.y >= y_min);
}

bool isInsidePolygon(const Point2D& p1, const Point2D& p2,
                     const vector<Point2D>& polygon) {
  bool p1_inside_polygon = false;
  bool p1_on_border = false;

  bool p2_inside_polygon = false;
  bool p2_on_border = false;
  for (auto i = std::begin(polygon), j = std::end(polygon) - 1;
       i != std::end(polygon); j = i++) {
    // check the boarders. Flip to true only once.
    p1_on_border |= pointOnBorder(p1, *i, *j);
    p2_on_border |= pointOnBorder(p2, *i, *j);

    // check strictly inside the polygon
    if ((i->y > p1.y) != (j->y > p1.y) && p1.x < i->x) {
      p1_inside_polygon = !p1_inside_polygon;
    }

    if ((i->y > p2.y) != (j->y > p2.y) && p2.x < i->x) {
      p2_inside_polygon = !p2_inside_polygon;
    }
  }

  // Point should exist within polygon and one point should be green
  return (p1_inside_polygon || p1_on_border) &&
         (p2_inside_polygon || p2_on_border);
}

bool rectangleInsidePolygon(const Point2D& p1, const Point2D& p2,
                            const vector<Point2D>& poly) {
  // Check the boxed line segment
  int64_t x_min = min(p1.x, p2.x);
  int64_t x_max = max(p1.x, p2.x);
  int64_t y_min = min(p1.y, p2.y);
  int64_t y_max = max(p1.y, p2.y);

  // Check if the edges are valid
  for (int64_t x = x_min; x <= x_max; x++) {
    Point2D p1_to_check{x, y_min};
    Point2D p2_to_check{x, y_max};
    if (!isInsidePolygon(p1_to_check, p2_to_check, poly)) {
      return false;
    }
  }

  for (int64_t y = y_min; y <= y_max; y++) {
    Point2D p1_to_check{x_min, y};
    Point2D p2_to_check{x_max, y};
    if (!isInsidePolygon(p1_to_check, p2_to_check, poly)) {
      return false;
    }
  }

  return true;
}

void partTwo(const string& file_path) {
  ifstream file(file_path);
  string input;
  int64_t max_area = 0;

  regex re(R"((\d+),(\d+))");

  // Parse the problem
  vector<Point2D> points;
  set<int64_t> x_cut;
  set<int64_t> y_cut;
  while (getline(file, input)) {
    smatch m;
    if (regex_match(input, m, re)) {
      int64_t x = stoll(m[1].str());
      int64_t y = stoll(m[2].str());
      Point2D p{x, y};
      points.push_back(p);
      x_cut.insert(x);
      x_cut.insert(x + 1);
      y_cut.insert(y);
      y_cut.insert(y + 1);
    }
  }

  // Represents the points inside a compressed grid
  vector<Point2D> compressed_points;
  for (auto& point : points) {
    auto x_it = x_cut.find(point.x);
    auto y_it = y_cut.find(point.y);

    Point2D compressed_point;

    if (x_it != x_cut.end()) {
      compressed_point.x = distance(x_cut.begin(), x_it);
    } else {
      cout << "Not found" << endl;
      break;
    }

    if (y_it != y_cut.end()) {
      compressed_point.y = distance(y_cut.begin(), y_it);
    } else {
      cout << "Not found" << endl;
      break;
    }

    compressed_points.push_back(compressed_point);
  }

  // brute force check but with additional constraint
  for (size_t i = 0; i < compressed_points.size(); i++) {
    Point2D tl = compressed_points.at(i);
    for (size_t j = i + 1; j < compressed_points.size(); j++) {
      Point2D br = compressed_points.at(j);

      if (rectangleInsidePolygon(tl, br, compressed_points)) {
        int64_t x1_orig = *next(x_cut.begin(), tl.x);
        int64_t y1_orig = *next(y_cut.begin(), tl.y);
        int64_t x2_orig = *next(x_cut.begin(), br.x);
        int64_t y2_orig = *next(y_cut.begin(), br.y);

        int64_t width = abs(x1_orig - x2_orig) + 1;
        int64_t height = abs(y1_orig - y2_orig) + 1;
        int64_t area = width * height;
        // cout << "Area: " << area << " Point: " << tl << " " << br << endl;
        max_area = max(max_area, area);
      }
    }
  }

  // 4629432496 TOO HIGH
  // 4629160206 TOO HIGH
  cout << "PART2: " << max_area << endl;
}

void partOne(const string& file_path) {
  ifstream file(file_path);
  string input;
  int64_t max_area = 0;

  regex re(R"((\d+),(\d+))");

  // parse the problem
  vector<Point2D> points;
  while (getline(file, input)) {
    smatch m;
    if (regex_match(input, m, re)) {
      int64_t x = stoll(m[1].str());
      int64_t y = stol(m[2].str());
      Point2D p{x, y};
      points.push_back(p);
    }
  }

  // brute force get the max area
  for (size_t i = 0; i < points.size(); i++) {
    Point2D a = points.at(i);
    for (size_t j = i + 1; j < points.size() - 1; j++) {
      Point2D b = points.at(j);
      int64_t width = abs(a.x - b.x) + 1;
      int64_t length = abs(a.y - b.y) + 1;
      int64_t area = width * length;
      max_area = max(max_area, area);
    }
  }

  cout << "PART1: " << max_area << endl;
}

int main() {
  string test_path = "test.txt";
  string my_path = "input.txt";

  cout << "========TEST OUTPUT========" << endl;
  partOne(test_path);
  partTwo(test_path);

  cout << endl;

  cout << "=========MY OUTPUT=========" << endl;
  partOne(my_path);
  partTwo(my_path);

  return 0;
};
