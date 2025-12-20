#include <iostream>
#include <fstream>
#include <vector>
#include <map>

void partTwo()
{
    std::ifstream file("day1.txt");
    std::vector<int> left;
    std::map<int, int> right_count;
    int a, b;
    while (file >> a >> b)
    {
        left.push_back(a);
        right_count[b]++;
    }

    int score{};
    for (int &num : left)
    {
        score += num * right_count[num];
    }

    std::cout << "Answer: " << score << std::endl;
}

int main()
{
    partTwo();
    return 0;
}