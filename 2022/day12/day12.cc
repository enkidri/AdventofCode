#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <fstream>
#include <regex>
using namespace std;

//Not foolproof but it works. Does not work if "start" is on the right side of "-" sign

class CaveSystem
{
    public:
        CaveSystem()
        {}

        void getInput(string const& filename)
        {
            ifstream f{filename};
            string line;

            const regex r("(\\w+)-(\\w+)");
            smatch sm;

            while (getline(f,line))
            {
                if (regex_search(line, sm, r))
                {
                    if (input.find(sm[1]) == input.end())
                    {
                        input[sm[1]];
                    }
                    if (input.find(sm[2]) == input.end())
                    {
                        input[sm[2]];
                    }

                    input[sm[1]].push_back(sm[2]);
                    if (sm[1] != "start")               
                    {
                        input[sm[2]].push_back(sm[1]);
                    }
                }
                else
                {
                    cout << "Error in regex";
                    break;
                }
            }
        }

        void findRoutes()
        {
            vector<string> passedSmallCave;
            vector<string> route;

            findRoutes_helper("start", passedSmallCave, route);
            //routes.erase(unique(routes.begin(), routes.end()), routes.end());
        }

        void findRoutes_helper(string const& cave, vector<string> passedSmallCave, 
                               vector<string> route)
        {
            if (cave == "end")
            {
                route.push_back(cave);
                routes.push_back(route);
                return;
            }
            if ((input.find(cave) == input.end()) || 
                (find(passedSmallCave.begin(), passedSmallCave.end(), cave) 
                != passedSmallCave.end()))
                return;
            
            route.push_back(cave);
            if (!all_of(cave.begin(), cave.end(), [](unsigned char c){ return std::isupper(c);})) //!is_capital letters
            {
                passedSmallCave.push_back(cave);
            }
            for (int i=0; i < input[cave].size(); ++i)
            {
                string c = input[cave][i];
                vector<string> passedSmallCaveDiv = passedSmallCave;
                
                findRoutes_helper(c, passedSmallCaveDiv, route);
            }
        }

        void findRoutes2()
        {
            map<string,int> passedSmallCave;
            vector<string> route;
            bool passSmallTwice = false;

            findRoutes2_helper("start", passedSmallCave, route, passSmallTwice);
            //routes.erase(unique(routes.begin(), routes.end()), routes.end());
        }

        void findRoutes2_helper(string const& cave, map<string,int> passedSmallCave, 
                               vector<string> route, bool passSmallTwice)
        {
            if (cave == "end")
            {
                route.push_back(cave);
                routes.push_back(route);
                return;
            }
            if ((input.find(cave) == input.end()) || 
                (((passedSmallCave[cave] == 1) || (passedSmallCave[cave] == 2)) && (passSmallTwice)))
                return;
            
            route.push_back(cave);
            if (!all_of(cave.begin(), cave.end(), [](unsigned char c){ return std::isupper(c);})) //!is_capital letters
            {
                if (passedSmallCave.find(cave) == passedSmallCave.end())
                {
                    passedSmallCave[cave];
                }
                ++passedSmallCave[cave];
                if (passedSmallCave[cave] == 2)
                    passSmallTwice = true;
            }
            for (int i=0; i < input[cave].size(); ++i)
            {
                string c = input[cave][i];
                map<string,int> passedSmallCaveDiv = passedSmallCave;
                bool passSmallTwiceDiv = passSmallTwice;
                
                findRoutes2_helper(c, passedSmallCaveDiv, route, passSmallTwiceDiv);
            }
        }

        void printInput()
        {
            for (auto const& v:input)
            {
                cout << v.first << ": ";
                for (auto& ele:v.second)
                {
                    cout << ele << " ";
                }
                cout << endl;
            }
        }

        void printRoutes()
        {
            for (auto const& route:routes)
            {
                for(auto const& cave:route)
                {
                    cout << cave << " ";
                }
                cout << "\n";
            }
            cout << endl;
        }

        int getRoutesSize()
        {
            return routes.size();
        }

    private:
        map<string,vector<string>> input;
        vector<vector<string>> routes;
};

//To check if string is in uppercase
//all_of(s.begin(), s.end(), [](unsigned char c){ return std::isupper(c);}); 
//TODO: Lös rekursivt genom att gå igenom input

void partTwo()
{
    CaveSystem c;
    c.getInput("day12.txt");
    c.printInput();
    //c.findRoutes2();
    //c.printRoutes();
    cout << "There are " << c.getRoutesSize() << " possible routes." << endl;
}

void partOne()
{
    CaveSystem c;
    c.getInput("day12.txt");
    //c.printInput();
    c.findRoutes();
    c.printRoutes();
    cout << "There are " << c.getRoutesSize() << " possible routes." << endl;
}

int main()
{
    //partOne();
    partTwo();
    return 0;
}