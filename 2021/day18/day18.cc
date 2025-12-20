#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <math.h>
#include <sstream>
#include <stack>
using namespace std;

//Spaghetticode for now. Will be cleaned at a later date

class FishNumber
{
    public:
        FishNumber(string const& s): number{s}
        {}

        void insertNumber(string const& str)
        {
            number = str;
        }

        void addNumber(string const& n)
        {
            number.insert(0, 1, '[');
            number.append("," + n + ']');

            reduce();
        }

        void reduce()
        {
            bool isDone = false;
            int iter = 0;
            while (true)
            {
                int par_num = 0;
                for (int i=0; i < number.size(); ++i)
                {
                    par_num += '[' == number[i];
                    par_num -= ']' == number[i];
                    if (par_num >= 5)
                    {
                        do_explode(i);
                        //cout << "Explosion:" << number << endl;
                        break;
                    }
                }
                if (par_num == 0)
                {
                    isDone = do_split();
                    //cout << "Split: " << number << endl;
                }
                if (isDone)
                    return;
            }
        }

        string getNumber()
        {
            return number;
        }

        bool do_split()
        {
            int par_num{};
            for (int i=0; i < number.size()-1; ++i)
            {
                if (isNumber(number.substr(i,2)))
                {
                    int n = stoi(number.substr(i,2));
                    if (n > 9)
                    {
                        int pair1 = floor(float(n) / 2);
                        int pair2 = ceil(float(n) / 2);

                        stringstream ss;
                        ss << "[" << pair1 << "," << pair2 << "]";
                        number.replace(i, 2, ss.str());
                        return false;
                    }
                }
            }
            return true;
        }

        void do_explode(int const from_idx)
        {
            auto [pair1, pair2] = pairs(
                number.substr(from_idx, 
                number.find(']', from_idx) - from_idx + 1));
            number.replace(from_idx, 
            number.find(']', from_idx) - from_idx + 1, "0");
            findRightNumInc(from_idx, pair2);
            findLeftNumInc(from_idx, pair1);
        }

        int magnitide()
        {
            string numbers = this -> number;
            while(true)
            {
                stack<int> s = {};
                s.push(0);
                for (int i=1; i < numbers.size(); ++i)
                {
                    if (numbers[i] == '[')
                        s.push(i);
                    if (numbers[i] == ']')
                    {
                        auto [pair1, pair2] = 
                        pairs(numbers.substr(s.top(), i - s.top() + 1));
                        int m = 3*pair1 + 2*pair2;
                        numbers.replace(s.top(), i - s.top() + 1, to_string(m));
                        s.pop();
                        break;
                    }
                }
                if (s.empty())
                    break;
            }
            return stoi(numbers);
        }

    private:
        pair<size_t, size_t> pairs(string const& number)
        {
            int pair1 = stoi(
                number.substr(1, number.find(',') - 1));
            int pair2 = stoi(
                number.substr(
                    number.find(',') + 1,
                     number.size() - number.find(',') - 2));
            return {pair1,pair2};
        }

    

        void findLeftNumInc(int from_idx, int inc)
        {
            for (int i = from_idx - 1; i >= 2; --i)
            {
                if (isNumber(number.substr(i-2,2)))
                {
                    int d = stoi(number.substr(i-2,2));
                    d += inc;
                    number.replace(i-2, 2, to_string(d));
                    break;
                }
                else if (isdigit(number[i]))
                {
                    int d = number[i] - '0';
                    d += inc;
                    number.replace(i, 1, to_string(d));
                    break;
                }
            }
        }

        void findRightNumInc(int from_idx, int inc)
        {
            for (int i = from_idx + 1; i < number.size()-1; ++i)
            {
                if (isNumber(number.substr(i,2)))
                {
                    int d = stoi(number.substr(i,2));
                    d += inc;
                    number.replace(i, 2, to_string(d));
                    break;
                }
                else if (isdigit(number[i]))
                {
                    int d = number[i] - '0';
                    d += inc;
                    number.replace(i, 1, to_string(d));
                    break;
                }
            }
        }


        string number;

        //Checks if string is a number (at least 2 digits)
        bool isNumber(string const& s)
        {
            for (auto c:s)
            {
                if (!isdigit(c))
                    return false;
            }
            return true;
        }
};

void partTwo()
{
    fstream f{"day18.txt"};
    string line;
    vector<FishNumber*> fish_numbers;
    vector<int> magnitudes;
    while(getline(f,line))
    {
        fish_numbers.push_back(new FishNumber(line));
    }

    for (auto& p1:fish_numbers)
    {
        for (auto& p2:fish_numbers)
        {
            string temp = p1->getNumber();
            p1->addNumber(p2->getNumber());
            magnitudes.push_back(p1->magnitide());
            p1->insertNumber(temp);
        }
    }
    cout << "The maximum sum of two fishnumbers is " << *max_element(magnitudes.begin(), magnitudes.end()) << endl;
}

void partOne()
{
    fstream f{"day18.txt"};
    string line;
    getline(f,line);
    FishNumber nr{line};
    while(getline(f,line))
    {
        nr.addNumber(line);
    }
    
    cout << "Maginitude is " << nr.magnitide() << endl;
}

int main()
{
    partOne();
    partTwo();
    return 0;
}