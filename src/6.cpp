#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <vector>
#include <set>
#include <numeric>

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::string line;
    std::ostringstream group;
    std::vector<std::string> groups;

    while(!ifs.eof())
    {
        std::getline(ifs, line);
        if(line.empty())
        {
            auto group_str = group.str();
            std::set<char> questions{std::cbegin(group_str), std::cend(group_str)};
            groups.emplace_back(std::cbegin(questions), std::cend(questions));
            group.str("");
        }
        else
        {
            group << line;
        }

    }

    std::cout << std::accumulate(std::begin(groups), std::end(groups), 0, [](int i, std::string const & questions)
    {
        return i + questions.size();
    }) << '\n';



    return 0;
}
