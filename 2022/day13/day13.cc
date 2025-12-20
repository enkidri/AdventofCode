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
#include <iomanip>
#include <variant>
using namespace std;

//Dont look. The code is cursed. This day required some inspiration for the
//data structure. Not my produest work.

struct packet : vector<variant<int,packet>>
{
    using vector<variant<int,packet>>::vector;
};

typedef pair<packet,packet> Packets;

packet parse(string line)
{
    packet packets;
    stack<packet*> s;
    s.push(&packets);

    for (int i=0; i < line.size(); i++)
    {
        const char c = line[i];
        if (c=='[')
            s.push(&get<packet>(s.top()->emplace_back(packet{})));
        else if(c == ']')
        {
            s.pop();
        }
        else if(c==',')
        {
        }
        else
        {
            if (line[i+1] == ',' || line[i+1] == ']')
            {
                s.top()->push_back(stoi(line.substr(i,1)));
            }
            else if(line[i+2] == ',' || line[i+2] == ']')
            {
                s.top()->push_back(stoi(line.substr(i,2)));
                i++;
            }
        }
    }

    return packets;
}

vector<Packets> parser(ifstream& file)
{
    string input{};
    vector<Packets> v;
    Packets ppair;
    int i{};
    while (getline(file, input))
    {
        if (input.empty())
            continue;

        packet p = parse(input);
        if (i == 0)
            ppair.first = p;
        else if (i == 1)
            ppair.second = p;

        i++;
        if (i == 2)
        {
            i = 0;
            v.push_back(ppair);
        }
    }

    return v;
}

void printer(packet& p)
{
    for (int i=0; i < p.size(); i++)
    {
        if (p[i].index() == 1)
        {
            cout << "[";
            printer(get<packet>(p[i]));
            cout << "]";
        }
        if (p[i].index() == 0)
        {
            cout << get<int>(p[i]);
        }
        cout << ",";
    }
}

void packetPrinter(vector<Packets>& p)
{
    for (auto& pa:p)
    {
        printer(pa.first);
        cout << endl;
        printer(pa.second);
        cout << endl << endl;
    }
}

enum order_status{wrong_order=0, right_order, equal_order};
order_status isCorrectOrder(Packets p)
{
    order_status status = equal_order;
    for (int i=0; i <= max(p.first.size(), p.second.size()) && (status == equal_order); i++)
    {
        if ((i == p.first.size()) && (i == p.second.size()))
            return equal_order;
        else if (i == p.first.size())
            return right_order;
        else if (i == p.second.size())
            return wrong_order;
        else
        {
            variant<int,packet>& l = p.first[i];
            variant<int,packet>& r = p.second[i];
            if ((l.index() == 0) && (r.index() == 0))
            {
                if (get<int>(l) < get<int>(r))
                    return right_order;
                else if (get<int>(l) > get<int>(r))
                    return wrong_order;
            }
            else if ((l.index() == 0) && (r.index() == 1))
            {
                status = isCorrectOrder(Packets{{l},get<packet>(r)});
            }
            else if((l.index() == 1) && (r.index() == 0))
            {
                status = isCorrectOrder(Packets{get<packet>(l), {r}});
            }
            else if ((l.index() == 1) && (r.index() == 1))
            {
                status = isCorrectOrder(Packets{get<packet>(l), get<packet>(r)});
            }
        }
    }

    return status;   //Only reaches this part is packets are equal
}

void partOne()
{   
    ifstream file("day13.txt");
    vector<Packets> packets = parser(file);

    int sum_of_indices{};
    for (int i=0; i < packets.size(); i++)
    {
        if (isCorrectOrder(packets[i]) == 1)
            sum_of_indices += i+1;
    }

    cout << "Part 1: The sum of indices in the correct order is " << sum_of_indices << endl;

}

vector<packet> parser2(ifstream& file)
{
    string input{};
    vector<packet> v;
    while (getline(file,input))
    {
        if (input.empty())
            continue;
        v.push_back(parse(input));
    }

    return v;
}


void packetPrinter2(vector<packet>& p)
{
    for (auto& pa:p)
    {
        printer(pa);
        cout << endl;
    }
}

void partTwo()
{
    ifstream file("day13.txt");
    vector<packet> packets = parser2(file);
    packets.push_back(parse("[[2]]"));
    packets.push_back(parse("[[6]]"));

    sort(packets.begin(), packets.end(), 
        [](auto& a, auto& b)
        {
            return isCorrectOrder({a,b});
        });
    //packetPrinter2(packets);
    int decoder_key{1};
    for (int i=0; i < packets.size(); i++)
    {
        packet& current = get<packet>(packets[i][0]);
        if ((current.size() == 1))
        {
            if (current[0].index() == 1)
            {
                packet& next = get<packet>(current[0]);
                if ((next.size() == 1))
                {
                    if (next[0].index() == 0)
                    {
                        int val = get<int>(next[0]);
                        if ((val==2) || (val==6))
                            decoder_key *= i+1;
                    }
                }
            }
        }
    }

    cout << "Part 2: The decoder key is " << decoder_key << endl;
}

int main()
{
    partOne();
    partTwo();

    return 0;
}
