#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<iterator>
#include<numeric>
#include<regex>
#include<stack>
#include<queue>
#include<set>
#include<map>
#include<sstream>
#include<functional>
#include<algorithm>
using namespace std;

class Dict
{
    private:
        struct Node;
    public:
        Dict(void)
        {
            current = new Node{};
            first = current;
            (*current).previous = nullptr;
            max_size = 70000000;
            space_needed = 30000000;
        }

        void insertFile(unsigned long int filesize)
        {
            (*current).files.push_back(filesize);
            (*current).file_sizes += filesize;
        }

        void insertMap(string& map)
        {
            (*current).folders.insert({map, new Node{}});
        }

        void doCD(string& map)
        {
            Node* tmp = current;
            current = (*current).folders.at(map);
            (*current).previous = tmp;
            (*current).foldername = map;
        }

        void doReturn()
        {
            current = (*current).previous;
        }

        void recursion(Node* p)
        {
            if ((*p).folders.empty())
            {
                //cout << "if " << (*p).foldername << endl;
                if ((*p).file_sizes < 100000)
                {
                    total_size += (*p).file_sizes;
                }
                return;
            }
            for (auto& i : (*p).folders)
            {
                recursion(i.second);
                (*p).child_size += (*i.second).file_sizes;
                //cout << "out " << (*p).foldername << endl;
            }
            (*p).file_sizes += (*p).child_size;
            //cout << (*p).foldername << " " << (*p).file_sizes << endl;
            if ((*p).file_sizes < 100000)
            {
                total_size += (*p).file_sizes;
            }
        }

        void recursiveExplore(Node* p)
        {
            if ((*p).folders.empty())
            {
                //cout << "if " << (*p).foldername << endl;
                if ((*p).file_sizes > space_needed - available_space)
                {
                    v_sizes.push_back((*p).file_sizes);
                }
                return;
            }
            for (auto& i : (*p).folders)
            {
                recursiveExplore(i.second);
            }

            if ((*p).file_sizes > space_needed - available_space)
            {
                v_sizes.push_back((*p).file_sizes);
            }
        }

        //debugging
        void printCurrentFolders()
        {
            for (auto& i:(*current).folders)
            {
                cout << i.first << endl;
            }
        }
        
        //debugging
        void printFiles()
        {
            for (auto& i:(*current).files)
            {
                cout << i << endl;
            }
        }

        Node* getFirst()
        {
            return first;
        }

        int getUsedSpace()
        {
            return first->folders["/"]->file_sizes;
        }

        void setAvailableSpace()
        {
            available_space = max_size - first->folders["/"]->file_sizes;
        }

        unsigned long int getAnswer()
        {
            return total_size;
        }

        int getAnswerPartTwo()
        {
            return *min_element(v_sizes.begin(), v_sizes.end());
        }

    private:
        struct Node
        {
            string foldername;
            vector<int> files;
            unsigned long int file_sizes{};
            map<string,Node*> folders;
            Node* previous;
            unsigned long int child_size{};
        };

        Node* current;
        Node* first;
        unsigned long int total_size{};
        int max_size;
        int available_space;
        int space_needed;
        vector<int> v_sizes;
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

//add end of getline func
void partOne()
{
    string input;
    ifstream file("day7.txt");
    Dict dict;
    string start_map = "/";
    dict.insertMap(start_map);    //adds the larger single map

    //Reads all the line to the dict data structure
    while (getline(file,input))
    {
        vector<string> commands = stringSplit(input);
        if ((commands[0] == "$") && (commands[1] == "cd"))
        {
            if (commands[2] == "..")
            {
                dict.doReturn();
            }
            else
            {
                dict.doCD(commands[2]);
            }
        }
        else
        {
            if (commands[0] == "dir")
            {
                dict.insertMap(commands[1]);
            }
            if (isNumeric(commands[0]))
            {
                dict.insertFile(stoi(commands[0]));
            }
        }
    }
    dict.recursion(dict.getFirst());

    cout << "Part 1: The total sum of specified dictionaries is " << dict.getAnswer() << endl;

}

void partTwo()
{
    string input;
    ifstream file("day7.txt");

    Dict dict;
    string start_map = "/";
    dict.insertMap(start_map);    //adds the larger single map

    //Reads all the line to the dict data structure
    while (getline(file,input))
    {
        vector<string> commands = stringSplit(input);
        if ((commands[0] == "$") && (commands[1] == "cd"))
        {
            if (commands[2] == "..")
            {
                dict.doReturn();
            }
            else
            {
                dict.doCD(commands[2]);
            }
        }
        else
        {
            if (commands[0] == "dir")
            {
                dict.insertMap(commands[1]);
            }
            if (isNumeric(commands[0]))
            {
                dict.insertFile(stoi(commands[0]));
            }
        }
    }

    dict.recursion(dict.getFirst());
    dict.setAvailableSpace();
    dict.recursiveExplore(dict.getFirst());

    cout << "Part 2: The smallest dictionary that frees enough space has the size of "
         << dict.getAnswerPartTwo() << endl;

    
}

int main()
{
    partOne();
    partTwo();
    return 0;
}
