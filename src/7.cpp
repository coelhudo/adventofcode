#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <regex>
#include <iterator>
#include <set>

struct Bag
{
    std::string color;
    int quantity;

    bool operator==(const Bag& other) const
    {
        return color == other.color;
    }

    bool operator==(const std::string& other) const
    {
        return color == other;
    }

    bool operator<(const Bag& other)
    {
        return color < other.color;
    }
};

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::regex pattern{R"(([\w ]+) bags contain no other bags\.|([\w ]+) bags contain ([\w ]+) bag| ([\w ]+) bag[s]?)"};

    std::string line;
    while (std::getline(ifs, line)) {
        std::cout << "begin\n";
        std::cout << line << '\n';

        auto begin = std::sregex_iterator(std::begin(line),  std::end(line), pattern);
        auto end = std::sregex_iterator();
        std::smatch match = *begin;

        //first group
        if(!match[1].str().empty()) {
            std::cout << match[1].str() << '\n';
            continue; //since there are no other bags here
        }

        //second group can have at least one bag
        if(!match[2].str().empty()) // first bag that contain other bags
            std::cout << match[2].str() << '\n';
        if(!match[3].str().empty()) // first bag within
            std::cout << match[3].str() << '\n';

        ++begin; // search for other bags within

        //third group remaining bags
        while(begin != end)
        {
            match = *begin++;
            if(!match[4].str().empty())
                std::cout << match[4].str() << '\n'; //
        }
    }


    return 0;
}
