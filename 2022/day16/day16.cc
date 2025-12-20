#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <map>
#include <regex>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <queue>
#include <set>
#include <numeric>
#include <functional>
using namespace std;

class Decoder
{
    public:
        Decoder() = default;

        void inputToMessage()
        {
            ifstream f{"day16.txt"};
            for_each(istream_iterator<char>{f}, istream_iterator<char>{},
                        [this](auto& c){message += m[c];});
        }

        int getVersionTotal()
        {
            return version_total;
        }

        uint64_t parser()
        {
            int version = binToDec(message.substr(0,3)); 
            version_total += version;
            int ID = binToDec(message.substr(3,3)); 
            message.erase(0,6);
            
            if (ID == 4)
            {
                return parseLiteral();
            }
            else
            {
                return parseOperator(ID);
            }
        }

        uint64_t parseOperator(int ID)
        {
            vector<uint64_t> val{};

            if (message[0] == '0')
            {
                int bitLen = binToDec(message.substr(1,15));
                message.erase(0, 15 + 1);
                int processed_bits = 0;
                while (processed_bits < bitLen)
                {
                    int bitSize_bef = message.size();
                    
                    val.push_back(parser());

                    int bitSize_aft = message.size();
                    processed_bits += bitSize_bef - bitSize_aft;
                }
            }
            else
            {
                int bitLen = binToDec(message.substr(1,11));
                message.erase(0, 11 + 1);
                for (int i=0; i < bitLen; ++i)
                {
                    val.push_back(parser());
                }
            }
            if (ID == 0)
                return accumulate(val.begin(), val.end(), uint64_t(0));
            else if (ID == 1)
                return accumulate(val.begin(), val.end(), uint64_t(1),  multiplies<>());
            else if (ID == 2)
                return *min_element(val.begin(), val.end());
            else if (ID == 3)
                return *max_element(val.begin(), val.end());
            else if (ID == 5)
                return val[0] > val[1];
            else if (ID == 6)
                return val[0] < val[1]; 
            else if (ID == 7)
                return val[0] == val[1];
            else
                return 0;
        }

        uint64_t parseLiteral()
        {
            string res{};
            while (true)
            {
                string sub = message.substr(0,5);
                res += sub.substr(1,4);
                message.erase(0,5);
                if (sub[0] == '0')
                {
                    return binToDec(res);
                }
            }
        }

        uint64_t binToDec(string const& s)
        {
            uint64_t dec_val = 0;
            uint64_t base = 1;
            for (auto it = s.crbegin(); it != s.crend(); ++it)
            {
                dec_val += (*it - '0') * base;
                base *= 2;
            }
            return dec_val;
        }

    private:
        int version_total{};
        string message{};
        map<char,string> m{{'0', "0000"},
                           {'1', "0001"},
                           {'2', "0010"},
                           {'3', "0011"},
                           {'4', "0100"},
                           {'5', "0101"},
                           {'6', "0110"},
                           {'7', "0111"},
                           {'8', "1000"},
                           {'9', "1001"},
                           {'A', "1010"},
                           {'B', "1011"},
                           {'C', "1100"},
                           {'D', "1101"},
                           {'E', "1110"},
                           {'F', "1111"}};
};

void partTwo()
{
    Decoder d;
    d.inputToMessage();

    cout << "Part 2: The packet value is " << d.parser();
}

void partOne()
{
    Decoder d;
    d.inputToMessage();
    d.parser();

    cout << "Part 1: The version total is " << d.getVersionTotal();
}

int main()
{   
    partOne();
    partTwo();

    return 0;
}