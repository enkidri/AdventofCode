#include<iostream>
#include<fstream>
#include<string>
#include<queue>
#include<numeric>
using namespace std;

void day1()
{
    string input;
    ifstream file("day1.txt");
    int calories{};
    int calories_per_Elf{};
    int most_calories{};
    while(getline(file, input))
    {
        if (!input.empty())
        {
            calories = stoi(input);
            calories_per_Elf += calories;
        }
        else
        {
            if (most_calories < calories_per_Elf)
            {
                most_calories = calories_per_Elf;
            }
            calories_per_Elf = 0;
        }
    }
    cout << "Part 1: The Elf carrying the most calories has " 
         << most_calories << "calories." << endl;
}

void day1N()
{
    string input;
    ifstream file("day1.txt");
    queue<int> q;
    int calories{};
    int calories_per_Elf{};
    int most_calories{};
    while (getline(file, input))
    {
        if (!input.empty())
        {
            calories = stoi(input);
            calories_per_Elf += calories;
        }
        else
        {
            if (!q.empty())
            {
                most_calories = q.back();
            }
            if (most_calories < calories_per_Elf)
            {
                q.push(calories_per_Elf);
                if (q.size() > 3)
                {
                    q.pop();
                }
            }
            calories_per_Elf = 0;
        }
    }
    
    int tot_Elf_calories{};
    while(!q.empty())
    {
        tot_Elf_calories += q.front();
        q.pop();
    }

    cout << "Part 2: The top 3 Elves are carrying " << tot_Elf_calories 
         << " calories." << endl;
}

int main()
{
    day1();
    day1N();

    return 0;
}