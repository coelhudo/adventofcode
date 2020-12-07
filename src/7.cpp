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

std::ostream& operator<<(std::ostream &os, Bag const& bag)
{
    os << "Bag{" << bag.color << ", " << bag.quantity << '}';
    return os;
}


//fill recursively a set containing all bags.
//I'm assuming that there is no cycle. Some bags cannot be inside other bags.
void find_bags(const std::string& bag_name, std::map<std::string, std::set<std::string>> const &bags, std::set<std::string> &all_bags)
{
    all_bags.insert(bag_name);
    if(bags.find(bag_name) == std::cend(bags))
        return;

    for(auto outer_bag_name : bags.at(bag_name))
    {
        find_bags(outer_bag_name, bags, all_bags);
    }
};

int find_total_bags(Bag const& bag, std::map<std::string, std::set<Bag>> const &bags)
{
    if(bags.find(bag.color) == std::cend(bags))
        return 0;

    int count = 0;
    for(auto outer_bag : bags.at(bag.color))
    {
        count += outer_bag.quantity;
        count += outer_bag.quantity * find_total_bags(outer_bag, bags);
    }

    return count;
};


int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::regex pattern{R"(([\w ]+) bags contain no other bags\.|([\w ]+) bags contain ([\w ]+) bag| ([\w ]+) bag[s]?)"};

    std::map<std::string, std::set<std::string>> inverted_index_bags;
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
        auto insert = [&](std::string const& inner_bag)
        {
            auto space_pos = inner_bag.find(' ');
            auto quantity = std::stoi(inner_bag.substr(0, space_pos));
            inverted_index_bags[inner_bag.substr(space_pos+1)].insert(outer_bag);
            bags[outer_bag].insert({inner_bag.substr(space_pos+1), quantity});
        };

        insert(match[3].str());

        ++begin; // search for other bags within

        //third group remaining bags
        while(begin != end)
        {
            match = *begin++;
            insert(match[4].str());
        }
    }

    std::set<std::string> all_bags;
    find_bags("shiny gold", inverted_index_bags, all_bags);
    std::cout << "Total bags that can carry shiny gold: " << all_bags.size() - 1 << '\n';
    std::cout << "Total bags that shiny gold bags carry: " << find_total_bags({"shiny gold", 0}, bags) << '\n';

    return 0;
}
