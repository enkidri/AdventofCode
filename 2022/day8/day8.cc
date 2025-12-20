#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <numeric>
#include <math.h>
#include <sstream>
#include <algorithm>
using namespace std;

void print(vector<vector<string>> const& vect)
{
    for (auto const& output:vect)
    {
        for (auto const& ele:output)
        {
            cout << ele << " ";
        }
        cout << endl;
    }
}

void print(vector<string> const& vect)
{
    int count = 0;
    for (auto const& ele:vect)
    {
        cout << ele << "; " << count << endl;
        ++count;
    }
    cout << endl;

}

bool isInString(string const& key, string const& to_check)
{
    bool is_in_string = false;
    int len = to_check.length();
    for (auto const& c:key)
    {
        is_in_string = false;
        int count = 0;
        for (auto const& l:to_check)
        {
            if (c == l)
            {
                is_in_string = true;
                break;
            }
            ++count;
            if (count == len)
            {
                return false;
            }
        }
    }
    return is_in_string;
}

string inStringRemove(string const& key, string r)
{
    for (auto const& c:key)
    {
        for (auto const& l:r)
        {
            if (c == l)
            {
                r.erase(remove(r.begin(), r.end(), c), r.end());
            }
        }
    }
    return r;
}

vector<string> decode(vector<string>& input)
{
    vector<string> decoded(10);
    int decoded_count{};

    while (decoded_count < 3)
    {
        for (auto & code:input)
        {
            if (code == "")
            {
                continue;
            }
            int size = code.length();
            if ((size == 2) || (size == 4) || (size == 3) || (size == 7))
            {
                switch(size){
                    case 2: decoded[1] = code; break;
                    case 4: decoded[4] = code; break;
                    case 3: decoded[7] = code; break;
                    case 7: decoded[8] = code; break;
                }
                code = "";
                decoded_count += 1;
            }
        }
    }

    decoded_count = 0;
    while (decoded_count < 5)
    {
        for (auto & code:input)
        {
            if (code == "")
            {
                continue;
            }
            int size = code.length();
            if (size == 5)
            {
                string test = inStringRemove(decoded[1], decoded[4]);
                if (isInString(decoded[1], code))
                {
                    decoded[3] = code;
                    code = "";
                    decoded_count +=1;
                }
                else if (isInString(test, code))
                {
                    decoded[5] = code;
                    code = "";
                    decoded_count +=1;
                }
                else if(!isInString(test, code))
                {
                    decoded[2] = code;
                    code = "";
                    decoded_count +=1;
                }
            }
            if (size == 6)
            {
                if (!isInString(decoded[1], code))
                {
                    decoded[6] = code;
                    code = "";
                    decoded_count += 1;
                }
                else if(isInString(decoded[4], code))
                {
                    decoded[9] = code;
                    code = "";
                    decoded_count += 1;
                }
                else
                {
                    decoded[0] = code;
                    code = "";
                    decoded_count += 1;
                }
            }
        }
    }

    return decoded;
}

bool isIdenticalWiring(string const& key, string const& to_check)
{
    if (key.length() != to_check.length())
    {
        return false;
    }
    bool is_in_string = false;
    int len = to_check.length();
    for (auto const& c:key)
    {
        is_in_string = false;
        int count = 0;
        for (auto const& l:to_check)
        {
            if (c == l)
            {
                is_in_string = true;
                break;
            }
            ++count;
            if (count == len)
            {
                return false;
            }
        }
    }
    return is_in_string;
}

string decode_digit(vector<string> const& decode, vector<string> const& output)
{
    string digit{};
    for (auto const& o:output)
    {
        int iter = 0;
        for (auto const& d:decode)
        {
            if (isIdenticalWiring(d, o))
            {
                digit += to_string(iter);
            }
            ++iter;
        }
    }
    return digit;
}

void partOne(string const& filename)
{
    ifstream f{filename};
    string line;
    vector<string> output;
    vector<vector<string>> outputs;
    while(getline(f,line))
    {
        bool delimiterReached = false;
        stringstream ss(line);
        for (string i; ss >> i;)
        {
            if (i == "|")
            {
                delimiterReached = true;
                continue;
            }
            if (delimiterReached)
            {
                output.push_back(i);
            }
        }
        outputs.push_back(output);
        output.clear();
    }
    //1: size = 2
    //4: size = 4
    //7: size = 3
    //8: size = 7
    int sum = 0;
    for (auto const& output:outputs)
    {
        for (auto const& c:output)
        {
            int size = c.length();
            if ((size == 2) || (size == 4) || (size == 3) || (size == 7))
            {
                ++sum;
            }
        }
    }

    cout << "The digits on question appears " << sum << " number of times." << endl;
}

void partTwo(string const& filename)
{
    ifstream f{filename};
    string line;

    vector<string> input;
    vector<string> output;
    vector<int> decoded_digits{};


    while(getline(f,line))
    {
        bool delimiterReached = false;
        stringstream ss(line);
        for (string i; ss >> i;)
        {
            if (i == "|")
            {
                delimiterReached = true;
                continue;
            }
            if (!delimiterReached)
            {
                input.push_back(i);
            }
            else
            {
                output.push_back(i);
            }
        }
        vector<string> decoded = decode(input);
        //print(output);
        string digit = decode_digit(decoded, output);
        decoded_digits.push_back(stoi(digit));
        input.clear();
        output.clear();
    }
    int sum = accumulate(decoded_digits.begin(), decoded_digits.end(), 0);

    cout << "The total sum is " << sum << endl;
}

int main(int argv, char** argc)
{
    //partOne(argc[1]);
    partTwo(argc[1]);
    
    //cout << isInString("ab", "cdfgeb");
    return 0;
}