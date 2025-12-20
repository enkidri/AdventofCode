#include <math.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using Grid = std::vector<std::vector<int>>;
using namespace std;

struct Index2D {
    int i;
    int j;

    bool operator==(Index2D other) const { return i == other.i && j == other.j; }

    bool operator!=(Index2D other) const { return !(*this == other); }

    friend ostream& operator<<(ostream& os, const Index2D& other) {
        os << "(" << other.i << " " << other.j << ")";
        return os;
    }
};

std::vector<int> StringToVector(std::string& str) {
    std::stringstream ss(str);
    std::vector<int> vec;
    int val;
    while (ss >> val) {
        vec.push_back(val);
    }

    return vec;
}

void Printer(std::vector<int>& vec) {
    for (auto& val : vec) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

void Printer(std::string::const_iterator it_start,
    std::string::const_iterator it_end) {
    std::cout << "Printer: " << std::string(it_start, it_end) << std::endl;
}

void Printer(Grid& grid) {
    for (auto& row : grid) {
        for (auto& ele : row) {
            std::cout << ele << " ";
        }
        std::cout << std::endl;
    }
}

Index2D FindNextObstacle(Index2D& index, vector<Index2D>& obstacles, int dir) {
    Index2D closest_obstacle = { -1, -1 };
    int diff_min = INT32_MAX;
    if (dir == 0) {
        for (size_t i = 0; i < obstacles.size(); i++) {
            if (obstacles[i].j == index.j) {
                int diff = index.i - obstacles[i].i;
                if (diff > 0 && diff <= diff_min) {
                    diff_min = diff;
                    closest_obstacle = obstacles[i];
                }
            }
        }
        if (closest_obstacle != Index2D{ -1, -1 }) ++closest_obstacle.i;
    }
    else if (dir == 1) {
        for (size_t j = 0; j < obstacles.size(); j++) {
            if (obstacles[j].i == index.i) {
                int diff = obstacles[j].j - index.j;
                if (diff > 0 && diff <= diff_min) {
                    diff_min = diff;
                    closest_obstacle = obstacles[j];
                }
            }
        }
        if (closest_obstacle != Index2D{ -1, -1 }) --closest_obstacle.j;
    }
    else if (dir == 2) {
        for (size_t i = 0; i < obstacles.size(); i++) {
            if (obstacles[i].j == index.j) {
                int diff = obstacles[i].i - index.i;
                if (diff > 0 && diff <= diff_min) {
                    diff_min = diff;
                    closest_obstacle = obstacles[i];
                }
            }
        }
        if (closest_obstacle != Index2D{ -1, -1 }) --closest_obstacle.i;
    }
    else if (dir == 3) {
        for (size_t j = 0; j < obstacles.size(); j++) {
            if (obstacles[j].i == index.i) {
                int diff = index.j - obstacles[j].j;
                if (diff > 0 && diff <= diff_min) {
                    diff_min = diff;
                    closest_obstacle = obstacles[j];
                }
            }
        }
        if (closest_obstacle != Index2D{ -1, -1 }) ++closest_obstacle.j;
    }

    return closest_obstacle;
}

Index2D GetLastPos(Index2D p, int dir, int N, int M) {
    if (dir == 0)
        return { 0, p.j };
    else if (dir == 1)
        return { p.i, M };
    else if (dir == 2)
        return { N, p.j };
    else if (dir == 3)
        return { p.i, 0 };

    return {};
}

void AddDiffToContainer(Index2D g1, Index2D g2, vector<Index2D>& s) {
    // Determine the boundaries
    int x_min = std::min(g1.i, g2.i);
    int x_max = std::max(g1.i, g2.i);
    int y_min = std::min(g1.j, g2.j);
    int y_max = std::max(g1.j, g2.j);

    // Generate the range of 2D points
    for (int x = x_min; x <= x_max; ++x) {
        for (int y = y_min; y <= y_max; ++y) {
            Index2D idx = { x, y };
            if (find(s.begin(), s.end(), idx) == s.end()) {
                // cout << "t: " << idx << std::endl;
                s.push_back(idx);
            }
        }
    }
}

bool CheckIfCycle(Index2D pos, vector<Index2D>& obstacles) {
    int dir{};
    vector<Index2D> visited = { pos };
    while (true) {
        Index2D pos_next = FindNextObstacle(pos, obstacles, dir);
        if (pos_next.i == -1 && pos_next.j == -1) {
            break;
        }
        if (find(visited.begin(), visited.end(), pos_next) != visited.end()) {
            Index2D pos_next_again = FindNextObstacle(pos_next, obstacles, dir + 1);
            if (find(visited.begin(), visited.end(), pos_next_again) !=
                visited.end()) {
                return true;
            }
        }
        visited.push_back(pos_next);
        pos = pos_next;
        dir = (dir + 1) % 4;
    }

    return false;
}

vector<Index2D> partOne();

void partTwo() {
    std::ifstream file("day6.txt");
    string input;

    int N{}, M{};
    vector<Index2D> obstacles;
    Index2D guard_position;

    while (getline(file, input)) {
        for (int j = 0; j < input.size(); j++) {
            if (input[j] == '#') obstacles.push_back(Index2D{ N, j });

            if (input[j] == '^') guard_position = Index2D{ N, j };
        }
        ++N;
    }
    M = input.size() - 1;
    --N;

    vector<Index2D> unique_pos = partOne();
    // remove guard position
    unique_pos.erase(remove(unique_pos.begin(), unique_pos.end(), guard_position),
        unique_pos.end());

    int count{};
    for (auto& pos : unique_pos) {
        obstacles.push_back(pos);
        if (CheckIfCycle(guard_position, obstacles)) {
            ++count;
        }
        obstacles.pop_back();
    }

    cout << "Answer: " << count << endl;
}

vector<Index2D> partOne() {
    std::ifstream file("day6.txt");
    string input;

    int N{}, M{};
    vector<Index2D> obstacles;
    Index2D guard_position;

    while (getline(file, input)) {
        for (int j = 0; j < input.size(); j++) {
            if (input[j] == '#') obstacles.push_back(Index2D{ N, j });

            if (input[j] == '^') guard_position = Index2D{ N, j };
        }
        ++N;
    }
    M = input.size() - 1;
    --N;

    int dir{};
    int count{};
    vector<Index2D> s = {};
    while (guard_position.i >= 0 && guard_position.i <= N &&
        guard_position.j >= 0 && guard_position.j <= M) {
        Index2D guard_position_next =
            FindNextObstacle(guard_position, obstacles, dir);
        if (guard_position_next.i == -1 && guard_position_next.j == -1) {
            guard_position_next = GetLastPos(guard_position, dir, N, M);
            AddDiffToContainer(guard_position, guard_position_next, s);
            break;
        }
        else {
            AddDiffToContainer(guard_position, guard_position_next, s);
            guard_position = guard_position_next;
            dir = (dir + 1) % 4;
        }
    }

    std::cout << "Answer: " << s.size() << std::endl;
    return s;
}

int main() {
    //   partOne();
    partTwo();
    return 0;
}