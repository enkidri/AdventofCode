#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <regex>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <sstream>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include <cmath>
using namespace std;

struct Monkey
{
    queue<unsigned long long int> items;
    string operation;
    unsigned long long int op_factor{};
    unsigned long long int div{};
    int if_true{};
    int if_false{};

    int nr_inspect{};
};

vector<string> stringSplit(string& s)
{
    stringstream ss(s);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> vstrings(begin, end);
    
    return vstrings;
}

bool isNumeric(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

void monkeyPrinter(Monkey& m)
{
    cout << "Items: ";
    while (!m.items.empty())
    {
        cout << m.items.front() << " ";
        m.items.pop();
    }
    cout << endl;

    cout << "Operation: " << m.operation << endl;
    cout << "Operation factor: " << m.op_factor << endl;
    cout << "Division: " << m.div << endl;
    cout << "If true: " << m.if_true << endl;
    cout << "If false: " << m.if_false << endl;
    cout << endl;
}

void doOneRound(vector<Monkey>& monkeys)
{
    for (Monkey& monkey:monkeys)
    {
        while (!monkey.items.empty())
        {
            int worry_level{};
            if (monkey.operation == "add")
            {
                worry_level = (monkey.items.front() + monkey.op_factor) / 3;
            }
            else if (monkey.operation == "pow")
            {
                worry_level = pow(monkey.items.front(), 2) / 3;
            }
            else
            {
                worry_level = (monkey.items.front() * monkey.op_factor) / 3;
            }

            if (worry_level % monkey.div == 0)
            {
                monkeys[monkey.if_true].items.push(worry_level);
            }
            else
            {
                monkeys[monkey.if_false].items.push(worry_level);
            }
            monkey.items.pop();

            monkey.nr_inspect++;
        }
    }
}

//Do power of 2 for monkey 4 is hardcoded but whatever lol
void partOne()
{   
    string input{};
    ifstream file("day11.txt");

    //parsing
    vector<Monkey> monkeys;
    while (file.peek() != EOF)
    {
        getline(file, input);
        monkeys.push_back(Monkey());

        getline(file, input);
        vector<string> start_items = stringSplit(input);
        for (auto& ele:start_items)
        {
            //cout << ele << endl;
            if ((isNumeric(ele)) || (isNumeric(ele.substr(0, ele.size()-1))))
            {
                monkeys.back().items.push(stoi(ele));
            }
        }

        getline(file, input);
        vector<string> operation = stringSplit(input);
        if ((operation[4] == "+") && isNumeric(operation.back()))
        {
            monkeys.back().operation = "add";
            monkeys.back().op_factor = stoi(operation.back());
        }
        else if ((operation[4] == "*") && isNumeric(operation.back()))
        {
            monkeys.back().operation = "mult";
            monkeys.back().op_factor = stoi(operation.back());
        }
        else
        {
            monkeys.back().operation = "pow";
        }

        getline(file, input);
        vector<string> divide = stringSplit(input);
        monkeys.back().div = stoi(divide.back());

        getline(file, input);
        vector<string> if_true = stringSplit(input);
        monkeys.back().if_true = stoi(if_true.back());

        getline(file, input);
        vector<string> if_false = stringSplit(input);
        monkeys.back().if_false = stoi(if_false.back());

        getline(file, input); //Skips to next monkey
    }

    //The actual action
    int total_rounds{20};
    for (int i=0; i < total_rounds; i++)
    {
        doOneRound(monkeys);
    }

    vector<int> inspections;
    for (auto& monkey:monkeys)
    {
        inspections.push_back(monkey.nr_inspect);
    }
    sort(inspections.begin(), inspections.end());

    int monkey_business = inspections.end()[-1] * inspections.end()[-2];

    cout << "Part 1: The monkey business after 20 rounds is " << monkey_business << endl;

}

// Thanks to redditor MattieShoes for an intuitive explaination to
// reduce worry_level
// https://www.reddit.com/r/adventofcode/comments/zizi43/2022_day_11_part_2_learning_that_it_was_that/iztt8mx/
void doOneRoundPartTwo(vector<Monkey>& monkeys)
{
    unsigned long long int cycle_length{1};
    for (Monkey& monkey:monkeys)
    {
        cycle_length *= monkey.div;
    }

    for (Monkey& monkey:monkeys)
    {
        while (!monkey.items.empty())
        {
            unsigned long long int worry_level{};
            if (monkey.operation == "add")
            {
                worry_level = (monkey.items.front() + monkey.op_factor);
            }
            else if (monkey.operation == "pow")
            {
                worry_level = pow(monkey.items.front(), 2);
            }
            else
            {
                worry_level = (monkey.items.front() * monkey.op_factor);
            }

            worry_level = worry_level % cycle_length;

            if (worry_level % monkey.div == 0)
            {
                monkeys[monkey.if_true].items.push(worry_level);
            }
            else
            {
                monkeys[monkey.if_false].items.push(worry_level);
            }
            monkey.items.pop();

            monkey.nr_inspect++;
        }
    }
}

//Essentially the same as part one. doRound is different.
void partTwo()
{
    string input{};
    ifstream file("day11.txt");

    //parsing
    vector<Monkey> monkeys;
    while (file.peek() != EOF)
    {
        getline(file, input);
        monkeys.push_back(Monkey());

        getline(file, input);
        vector<string> start_items = stringSplit(input);
        for (auto& ele:start_items)
        {
            if ((isNumeric(ele)) || (isNumeric(ele.substr(0, ele.size()-1))))
            {
                monkeys.back().items.push(stoi(ele));
            }
        }

        getline(file, input);
        vector<string> operation = stringSplit(input);
        if ((operation[4] == "+") && isNumeric(operation.back()))
        {
            monkeys.back().operation = "add";
            monkeys.back().op_factor = stoi(operation.back());
        }
        else if ((operation[4] == "*") && isNumeric(operation.back()))
        {
            monkeys.back().operation = "mult";
            monkeys.back().op_factor = stoi(operation.back());
        }
        else
        {
            monkeys.back().operation = "pow";
        }

        getline(file, input);
        vector<string> divide = stringSplit(input);
        monkeys.back().div = stoi(divide.back());

        getline(file, input);
        vector<string> if_true = stringSplit(input);
        monkeys.back().if_true = stoi(if_true.back());

        getline(file, input);
        vector<string> if_false = stringSplit(input);
        monkeys.back().if_false = stoi(if_false.back());

        getline(file, input); //Skips to next monkey
    }

    //The actual action
    int total_rounds{10000};
    for (int i=0; i < total_rounds; i++)
    {
        doOneRoundPartTwo(monkeys);
    }

    vector<unsigned long long int> inspections;
    for (auto& monkey:monkeys)
    {
        inspections.push_back(monkey.nr_inspect);
    }
    sort(inspections.begin(), inspections.end());

    unsigned long long int monkey_business = inspections.end()[-1] * inspections.end()[-2];

    cout << "Part 2: The monkey business after 10 000 rounds is " << monkey_business << endl;

}

int main()
{
    partOne();
    partTwo();

    return 0;
}
