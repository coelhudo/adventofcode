#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>
#include <vector>
#include <set>
#include <numeric>
#include <array>
#include <algorithm>

std::vector<int> check_answers_a(std::ifstream &ifs)
{
    std::string line;
    std::ostringstream group;
    std::vector<int> groups;

    //O(n) where n is the number of lines
    while(!ifs.eof())
    {
        std::getline(ifs, line);
        if(line.empty())
        {
            auto group_str = group.str();
            //O(m) where m is the number of answers per group
            std::set<char> questions{std::cbegin(group_str), std::cend(group_str)};
            groups.push_back(questions.size());
            group.str("");
        }
        else
        {
            group << line;
        }

    }

    return groups;
}

std::vector<int> check_answers_b(std::ifstream &ifs)
{
    std::string line;
    std::ostringstream group;
    std::vector<int> groups;
    int ingroup_counter{};

    //O(n) where n is the number of lines
    while(!ifs.eof())
    {
        std::getline(ifs, line);
        if(line.empty())
        {
            auto group_str = group.str();
            std::array<int, 26> questions_tracker{0};


            //O(m) where m is the number of answers per group
            for(auto c : group_str)
            {
                //I'm assuming that we are using an ASCII table where all
                //ASCII codes are sequential.
                questions_tracker[c - 'a'] += 1;
            }

            //O(1) since questions_tracker size is 26
            auto total = std::count(std::begin(questions_tracker), std::end(questions_tracker), ingroup_counter);
            groups.push_back(total);

            //reset group
            group.str("");
            ingroup_counter = 0;
        }
        else
        {
            group << line;
            ++ingroup_counter;
        }

    }

    return groups;
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    //part a
    //auto groups = check_answers_a(ifs);

    //part b
    auto groups = check_answers_b(ifs);

    std::cout << std::accumulate(std::begin(groups), std::end(groups), 0) << '\n';



    return 0;
}
