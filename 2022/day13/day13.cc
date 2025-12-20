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

class Origami
{
    public:
        Origami()
        {
            origami.resize(1);
        }
        void addInput(string const& filename)
        {
            const regex r("(\\d+),(\\d+)");
            const regex r1("fold along (\\w)=(\\d+)");
            smatch sm;

            ifstream f{filename};
            string line;
            int xLim{};
            int yLim{};
            while(getline(f,line))
            {
                if (line[0] == 'f')
                {
                    if (regex_search(line, sm, r1))
                    {
                        string axis = sm[1];
                        int coord = stoi(sm[2]);
                        foldLine.push_back(make_pair(axis, coord));
                    }
                    //break; //Solution for partOne();
                }
                else if(regex_search(line, sm, r))
                {
                    int x = stoi(sm[1]);
                    int y = stoi(sm[2]);
                    if (y > yLim)
                    {
                        int rowsToAdd = y - yLim;
                        vector<int> row(xLim + 1, 0);
                        for (int i=0; i < rowsToAdd; ++i)
                        {
                            origami.push_back(row);
                        }
                        yLim = y;
                    }
                    if (x > xLim)
                    {
                        for (auto& row:origami)
                        {
                            row.resize(x + 1);
                        }
                        xLim = x;
                    }
                    ++origami[y][x];
                }
            }
        }

        void foldOrigami()
        {
            for (auto const& p:foldLine)
            {
                if (p.first == "x")
                {
                    foldX(p.second);
                }
                else if(p.first == "y")
                {
                    foldY(p.second);
                }
            }
        }

        void printOrigami()
        {
            auto minCompX = [](pair<string,int>& a, pair<string,int>& b)
            {
                if (a.first == "x")
                    return a.second < b.second;
                else
                    return false;
            };
            int min_x = (*min_element(foldLine.begin(), foldLine.end(), minCompX)).second;

            auto minCompY = [](pair<string,int>& a, pair<string,int>& b)
            {
                if (a.first == "y")
                    return a.second < b.second;
                else
                    return false;
            };
            int min_y = (*min_element(foldLine.begin(), foldLine.end(), minCompY)).second;


            for (int y=0; y < min_y; ++y)
            {
                vector<int> row = origami[y];
                for (int x=0; x < min_x; ++x)
                {
                    int ele = row[x];
                    if (ele > 0)
                        cout << '#' << " ";
                    else
                        cout << '.' << " ";
                }
                cout << endl;
            }
        }

        void printFold()
        {
            for (auto& line:foldLine)
            {
                cout << line.first << " " << line.second << endl;
            }
        }

        int countDots()
        {
            int dots{};
            for (auto const& row:origami)
            {
                for (auto const& dot:row)
                {
                    if (dot>0)
                    {
                        ++dots;
                    }
                }
            }

            return dots;
        }

    private:
        void foldX(int foldAxis)
        {
            int rowSize = origami[1].size();
            for (int y=0; y < origami.size(); ++y)
            {
                for (int x=foldAxis + 1; x < min(foldAxis*2 + 1, rowSize); ++x)
                {
                    int dist = x - foldAxis;
                    origami[y][foldAxis-dist] += origami[y][x];
                    origami[y][x] = 0;
                }
            }
        }
        void foldY(int foldAxis)
        {
            int colSize = origami.size();
            for (int y=foldAxis + 1; y < min(foldAxis*2 + 1, colSize); ++y)
            {
                for (int x=0; x < origami[1].size(); ++x)
                {
                    int dist = y - foldAxis;
                    origami[foldAxis-dist][x] += origami[y][x];
                    origami[y][x] = 0;
                }
            }
        }

        vector<vector<int>> origami;
        vector<pair<string,int>> foldLine;

};

void partTwo()
{
    Origami o{};
    o.addInput("day13.txt");
    o.foldOrigami();
    o.printOrigami();

    cout << "Finished" << endl;
}

void partOne()
{
    Origami o{};
    o.addInput("day13.txt");
    o.foldOrigami();
    o.printOrigami();

    cout << "There are " << o.countDots() << " dots on the paper." << endl;
}

int main()
{
    //partOne();
    partTwo();
    return 0;
}