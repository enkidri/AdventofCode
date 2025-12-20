#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <regex>
#include <fstream>
#include <iomanip>
#include <utility>
#include <sstream>
#include <istream>
using namespace std;

class Polymer
{
    public:
        Polymer()
        {}
        
        void addInput()
        {
            ifstream f{"day14.txt"};
            string line;
            getline(f,line);
            polyString = line;
            polyCountAdd();     //Adds the first line


            for (int i=0; i < polyString.length()-1; ++i)
            {
                if (polyStringMap.find(polyString.substr(i,2)) == polyStringMap.end())
                    polyStringMap[polyString.substr(i,2)];
                ++polyStringMap[polyString.substr(i,2)];
            }
            
            f.ignore();     //Ignore empty line
            while (getline(f,line))
            {
                stringstream ss(line);
                string key;
                char value;
                string temp;
                ss >> key >> temp >> value;
                polyMap[key] = value;

                if (polyStringMap.find(key) == polyStringMap.end())
                    polyStringMap[key];
                if (polyCount.find(value) == polyCount.end())
                    polyCount[value];
            }
            //addCountStart(polyString);
        }

        void step()
        {
            stringstream ss;
            string new_polymer;
            string curr_subStr = polyString.substr(0,2);
            ss << curr_subStr[0] << polyMap[curr_subStr] << curr_subStr[1];
            new_polymer += ss.str();
            ss.str("");

            for (unsigned long long int i=1; i < polyString.length() - 1; ++i)
            {
                string curr_subStr = polyString.substr(i,2);
                ss << polyMap[curr_subStr] << curr_subStr[1];
                new_polymer += ss.str();
                ss.str("");

                if (polyCount.find(polyMap[curr_subStr]) == polyCount.end())
                {
                    polyCount[polyMap[curr_subStr]];
                }
                ++polyCount[polyMap[curr_subStr]];
            }
            polyString = new_polymer;
        }

        void stepNew()          //Faster/smartar than step()
        {
            stringstream ss;
            map<string, unsigned long long int> temp_polyStringMap = polyStringMap;
            for (auto& p:polyStringMap)
            {
                if (p.second == 0)
                    continue;
                ss << polyMap[p.first] << (p.first)[1];
                string a = ss.str(); 
                ss.str("");

                ss << (p.first)[0] << polyMap[p.first];
                string b = ss.str(); 
                ss.str("");
                temp_polyStringMap[a] += p.second;
                temp_polyStringMap[b] += p.second;

                temp_polyStringMap[p.first] -= p.second;
                
                polyCount[polyMap[p.first]] += polyStringMap[p.first];
            }
            polyStringMap = temp_polyStringMap;
        }

        string getPolymer()
        {
            return polyString;
        }

        int partOneRes()
        {
            for (auto& c:polyString)
            {
                if (polyCount.find(c) == polyCount.end())
                {
                    polyCount[c];
                }
                ++polyCount[c];
            }

            unsigned long long int min_val = (*min_element(polyCount.begin(), polyCount.end(), [](auto& a, auto& b)
            {
                return a.second < b.second;
            })).second;
            unsigned long long int max_val = (*max_element(polyCount.begin(), polyCount.end(), [](auto& a, auto& b)
            {
                return a.second < b.second;
            })).second;
            return max_val - min_val;
        }

        unsigned long long int partTwoRes()
        {
            unsigned long long int min_val = (*min_element(polyCount.begin(), polyCount.end(), [](auto& a, auto& b)
            {
                return a.second < b.second;
            })).second;
            unsigned long long int max_val = (*max_element(polyCount.begin(), polyCount.end(), [](auto& a, auto& b)
            {
                return a.second < b.second;
            })).second;
            return max_val - min_val;
        }

        void printCount()
        {
            for (auto const& p:polyCount)
            {
                cout << p.first << " " << p.second << endl;
            }
        }

    private:
        void polyCountAdd()
        {
            for (auto& c:polyString)
            {
                if (polyCount.find(c) == polyCount.end())
                {
                    polyCount[c];
                }
                ++polyCount[c];
            }
        }

        string polyString;
        map<string, unsigned long long int> polyStringMap; //Counts of groups of AA, BB, CC...
        map<string, char> polyMap;                          
        map<char, unsigned long long int> polyCount;        //Counts of singels A,B,C...

};

void partTwo()
{
    Polymer polymer;
    polymer.addInput();
    int stepCount = 40;
    for (int i=1; i <= stepCount; ++i)
    {
        polymer.stepNew();
    }
    polymer.printCount();

    cout << "The result of the subtraction of max element and min element is " 
         << polymer.partTwoRes() << endl;
}


void partOne()
{
    Polymer polymer;
    polymer.addInput();
    int stepCount = 10;
    for (int i=1; i <= stepCount; ++i)
    {
        polymer.step();
        //cout << "After step " << i << ": " << polymer.getPolymer() << endl;
    }
    cout << "The result of the subtraction of max element and min element is " 
         << polymer.partOneRes() << endl;
}

int main()
{
    //partOne();
    partTwo();

    return 0;
}
