#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <regex>
#include <iterator>
#include <set>
#include <map>

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

    bool operator<(const Bag& other) const
    {
        return color < other.color;
    }
};


//fill recursively a set containing all bags.
//I'm assuming that there is no cycle. Some bags cannot be inside other bags.
void find_bags(const std::string& bag_name, std::map<std::string, std::set<Bag>> const &bags, std::set<std::string> &all_bags)
{
    all_bags.insert(bag_name);
    if(bags.find(bag_name) == std::cend(bags))
        return;

    for(auto b: bags.at(bag_name))
    {
        find_bags(b.color, bags, all_bags);
    }
};


int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::regex pattern{R"(([\w ]+) bags contain no other bags\.|([\w ]+) bags contain ([\w ]+) bag| ([\w ]+) bag[s]?)"};

    std::map<std::string, std::set<Bag>> bags;

    std::string line;
    while (std::getline(ifs, line)) {

        auto begin = std::sregex_iterator(std::begin(line),  std::end(line), pattern);
        auto end = std::sregex_iterator();
        std::smatch match = *begin;

        //first group
        if(!match[1].str().empty())
            continue; //since there are no other bags here

        //second group can have at least one bag

        // first bag that contain other bags
        auto outer_bag = match[2].str();
        // first bag within
        auto inner_bag = match[3].str();
        auto space_pos = inner_bag.find(' ');
        auto quantity = std::stoi(inner_bag.substr(0, space_pos));
        bags[inner_bag.substr(space_pos+1)].insert({outer_bag, quantity});

        ++begin; // search for other bags within

        //third group remaining bags
        while(begin != end)
        {
            match = *begin++;
            auto inner_bag = match[4].str();
            auto space_pos = inner_bag.find(' ');
            auto quantity = std::stoi(inner_bag.substr(0, space_pos));
            bags[inner_bag.substr(space_pos+1)].insert({outer_bag, quantity});
        }
    }

    std::set<std::string> all_bags;
    find_bags("shiny gold", bags, all_bags);
    std::cout << "Total bags that can carry shiny gold: " << all_bags.size() - 1 << '\n';

    return 0;
}
