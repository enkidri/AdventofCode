#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>

void partOne() {
    std::ifstream file("day1.txt");
    std::vector<int> left;
    std::vector<int> right;

    int a,b;
    while (file >> a >> b) {
        left.push_back(a);
        right.push_back(b);
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    
    int sum{};
    for (size_t i=0; i < left.size(); i++) {
        int diff = std::abs(left[i] - right[i]);
        sum += diff;
    }
    
    std::cout << "Answer: " << sum << std::endl;

}

int main() {
    partOne();

    return 0;
}