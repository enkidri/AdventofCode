#include <iostream>
#include <iomanip>
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

#include <chrono>
using namespace std;

#define INF 999
typedef pair<int, vector<string>> Value;

// Part 1: Dynamic programming over nonreduced graph
// Part 2: Dynamic programming over reduced graph + combinatorics over possible
//         significant valve configuration. I do not like the implementation since
//         I'm making hard assumptions about the number of valves I and the elephant
//         are visiting (me visits # significant valves/2, elephant visits the rest)
//         but oh well it works.

map<string,Value> parse(ifstream& file)
{
    string input{};
    map<string,Value> m;
    Value item;
    regex r1(R"([A-Z]{2})");
    regex r2(R"(\d+)");
    smatch match;
    while (getline(file, input))
    {
        item = {};

        regex_search(input,match,r1);
        string key = match.str();
        input = match.suffix();

        regex_search(input, match, r2);
        item.first = stoi(match.str());
        input = match.suffix();

        while (regex_search(input,match,r1))
        {
            item.second.push_back(match.str());
            input = match.suffix();
        }

        m[key] = item;
    }

    return m;
}

int find_max(map<vector<string>, int>& cache, map<string,Value>& map, string start, 
             set<string> open_valves, int cost_to_go, int rate_i, int min_left)
{
    vector<string> state = {to_string(cost_to_go), to_string(min_left), start};
    if (min_left <= 0)
        return cost_to_go;
    else if(cache.find(state) != cache.end())
        return cache[state];

    int max{};
    vector<string>& next = map[start].second;
    for (auto& valve:next)
    {
        vector<int> maxes;
        int max_i = cost_to_go;
        int min_i = min_left;           
        int rate = map[valve].first;
        set<string> open_valve_i = open_valves;
        min_i--; //Decrement for moving
        
        maxes.push_back(find_max(cache, map, valve, open_valve_i, max_i, rate_i, min_i));   

        if ((open_valves.find(valve) == open_valves.end()) && (rate != 0))              
        {
            min_i--;
            max_i += rate*(min_i);
            open_valve_i.insert(valve);
            rate_i += rate;
            maxes.push_back(find_max(cache, map, valve, open_valve_i, max_i, rate_i, min_i)); 
        }

        int max_fin = *max_element(maxes.begin(), maxes.end());    
        if (max_fin > max)
            max = max_fin;
    }

    vector<string> memo = {to_string(cost_to_go), to_string(min_left), start};
    cache[memo] = max;   

    return max;
}

vector<vector<int>> floydWarshall(vector<vector<int>>& m)
{
    vector<vector<int>> v = m;
    for (int k=0; k < v.size(); k++)
    {
        for (int i=0; i < v.size(); i++)
        {
            for (int j=0; j < v.size(); j++)
            {
                if (v[i][k] + v[k][j] < v[i][j])
                    v[i][j] = v[i][k] + v[k][j];
            }
        }
    }
    
    return v;
}

void printMatrix(vector<vector<int>>& m);

vector<vector<int>> calcDistances(map<string,Value>& m)
{
    vector<vector<int>> distance_matrix(m.size(), vector<int>(m.size(), INF));
    vector<string> valves;
    for_each(m.begin(), m.end(), [&valves](auto& ele){
        valves.push_back(ele.first);
    });

    int idx = 0;
    for (auto& ele:m)
    {
        vector<string> next = ele.second.second;
        distance_matrix[idx][idx] = 0;
        for (auto& n:next)
        {
            int n_idx = distance(valves.begin(), find(valves.begin(), valves.end(), n));
            distance_matrix[idx][n_idx] = 1;
        }
        idx++;
    }
    
    vector<vector<int>> m_floyd = floydWarshall(distance_matrix);
    return m_floyd;
}

vector<int> getWorkingValves(map<string,Value>& m)
{
    vector<int> v;
    int idx{};
    for (auto& ele:m)
    {
        if (ele.second.first > 0)
            v.push_back(idx);
        idx++;
    }

    return v;
}

void printMatrix(vector<vector<int>>& m)
{
    for (auto& ele1:m)
    {
        for (auto& ele2:ele1)
        {
            cout << setw(3) << setfill(' ') << ele2 << " ";
        }
        cout << endl;
    }
}

void printParse(map<string,Value>& p)
{
    for (auto& ele:p)
    {
        cout << ele.first << " ";
        for (auto& e:ele.second.second)
        {
            cout << e << " ";
        }
        cout << endl;
    }
}

void partOne()
{   
    ifstream file("day16.txt");
    map<string,Value> parsed = parse(file);
    map<vector<string>, int> cache;

    string start = "AA";
    int goal_iter = 30;
    int cost_to_go = 0;
    int start_rate = 0;
    set<string> visited_valves;

    int top_pressure = find_max(cache, parsed, start, visited_valves, cost_to_go, start_rate, goal_iter);
    
    cout << "Part 1: " << top_pressure << " pressure can be released at most."<< endl;

}

int maxSearchReduced(map<tuple<int,int,int>,pair<int,set<int>>>& cache, map<string,Value>& parsed,
                     vector<vector<int>>& distances, vector<int> working_valves, 
                     vector<int> sequence, int current, int time_left, int cost_to_go)
{
    tuple<int,int,int> state = {cost_to_go, time_left,
                                current};
    if ((time_left <= 0) || (working_valves.empty())) 
        return cost_to_go;
    else if(cache.find(state) != cache.end())
        return cache[state].first;

    int max_pressure{};
    for (int i=0; i < working_valves.size(); i++)
    {
        vector<int> seq_temp = sequence;
        seq_temp.push_back(working_valves[i]);
        vector<int> valves_left_temp = working_valves;
        valves_left_temp.erase(valves_left_temp.begin() + i);
        int time_left_temp = time_left;
        int cost_to_go_temp = cost_to_go;
        auto it_rate = parsed.begin(); 
        advance(it_rate, working_valves[i]);
        int rate = (*it_rate).second.first;

        time_left_temp -= distances[current][working_valves[i]];   //time moving to significant valve
        time_left_temp--;  //time opening the valve
        cost_to_go_temp += rate * time_left_temp;

        int max_temp{};
        if (time_left_temp < 0)
        {
            max_temp = cost_to_go;
        }
        else
        {
            max_temp = maxSearchReduced(cache, parsed, distances, valves_left_temp, 
                                        seq_temp, working_valves[i], time_left_temp, 
                                        cost_to_go_temp);
        }

        if (max_temp > max_pressure)
            max_pressure = max_temp;
    }

    set<int> s(sequence.begin(), sequence.end());
    tuple<int, int, int> key = {cost_to_go, time_left, current};
    cache[key] = make_pair(max_pressure, s);

    return max_pressure;
}

vector<int> symmetric_difference(vector<int> s1, vector<int> s2)
{
    sort(s1.begin(), s1.end());
    sort(s2.begin(), s2.end());

    vector<int> res;
    set_symmetric_difference(s1.begin(), s1.end(),
                             s2.begin(), s2.end(),
                             back_inserter(res));

    return res;
}

//Gets combination of significant valves without repititions.
void combinatorics(vector<int>& comb, vector<int>& valves,
                   int offset, int k, vector<vector<int>>& output)
{
    if (k == 0)
    {
        output.push_back(comb);
        return;
    }

    for (int i = offset; i <= valves.size() - k; i++)
    {
        comb.push_back(valves[i]);
        combinatorics(comb, valves, i+1, k-1, output);
        comb.pop_back();
    }
}

void partTwo()
{
    //Same as part one (aka with DP) but with reduced graph over significant valves.
    //Split all significant valve comibations into two sets, remove duplicates 
    //and run the algorithm over it.
    ifstream file("day16.txt");
    map<string,Value> parsed = parse(file);
    vector<vector<int>> v = calcDistances(parsed);
    vector<int> working_valves = getWorkingValves(parsed);
    map<tuple<int,int,int>,pair<int,set<int>>> cache;                        //key: valves visited + current valve item: cost-to-go
    vector<int> sequence;
    int current = 0;                                          //First valve has index 0
    int time_left{26};

    vector<int> comb;
    vector<int> valves(working_valves);
    int k = working_valves.size() / 2;
    vector<vector<int>> output;
    combinatorics(comb, valves, 0, k, output);

    int max{};
    for (vector<int>& vec:output)
    {
        vector<int> s1 = vec;
        vector<int> s2 = symmetric_difference(working_valves, vec);
        cache = {};
        int m1 = maxSearchReduced(cache, parsed, v, s1, sequence, current, time_left, 0);
        cache = {};
        int m2 = maxSearchReduced(cache, parsed, v, s2, sequence, current, time_left, 0);

        if (m1 + m2 > max)
            max = m1 + m2;
    }

    cout << "Part 2: The maximum pressure with two agents is " << max << endl;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();
    partOne();
    auto stop1 = chrono::high_resolution_clock::now();
    partTwo();
    auto stop2 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::microseconds>(stop1-start);
    auto duration2 = chrono::duration_cast<chrono::microseconds>(stop2-stop1);
    cout << "Part 1: It took " << duration1.count()/(float)1000000 << " seconds to run." << endl;
    cout << "Part 2: It took " << duration2.count()/(float)1000000 << " seconds to run." << endl;

    return 0;
}
