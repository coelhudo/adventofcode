#include <cstddef>
#include <ios>
#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

template<typename Container>
void print_container(Container const& subrules, char end = '\n')
{
    for(auto i: subrules)
        std::cout << i << " ";
    std::cout << end;
};

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::multimap<int, std::vector<int>> rules;
    std::multimap<std::vector<int>, int> inverted_rules;
    std::map<char, int> terminal_rules;
    std::vector<std::string> entries;

    std::ifstream ifs{argv[1]};
    std::string line;

    bool is_a_rule = true;


    while(std::getline(ifs, line))
    {
        if(line.empty())
        {
            is_a_rule = false;
            continue;
        }

        if(is_a_rule) {
            auto pos = line.find(':');
            const auto rule_number = std::stoi(line.substr(0, pos));
            const auto start_quote = line.find('"');
            if(start_quote != std::string::npos)
            {
                const auto end_quote = line.rfind('"');
                auto terminal_symbol = line.substr(start_quote+1, end_quote-start_quote-1);
                terminal_rules.insert({terminal_symbol[0], rule_number});
            }
            else
            {
                std::istringstream iss{line.substr(pos+2)};
                std::vector<int> subrules;

                std::string value;
                while(iss >> value)
                {
                    if(value != "|")
                        subrules.push_back(std::stoi(value));

                    if(value == "|")
                    {
                        rules.insert({rule_number, subrules});
                        subrules.clear();
                    }
                }
                rules.insert({rule_number, subrules});
            }

        } else {
            entries.push_back(line);
        }
    }

    for(auto [rule_number, subrules]: rules)
    {
        if(subrules.size() != 1)
            continue;

        for(auto &entry: rules)
        {
            auto &rules = entry.second;
            if(rules.at(0) == rule_number)
                rules[0] = subrules.at(0);
            if(rules.size() != 1 && rules.at(1) == rule_number)
                rules[1] = subrules.at(0);
        }
    }

    for(auto [rule_number, subrules]: rules)
    {
        if(subrules.size() == 1)
            continue;

        inverted_rules.insert({subrules, rule_number});
    }

    // std::cout << "=============" << "\n";

    // for(auto [rule_number, subrules]: rules)
    // {
    //     std::cout << rule_number << ": ";
    //     for(auto subrule: subrules)
    //         std::cout << subrule << ' ';
    //     std::cout << "\n";

    // }

    // for(auto [subrules, rule_number]: inverted_rules)
    // {
    //     for(auto subrule: subrules)
    //         std::cout << subrule << ' ';
    //     std::cout << ": " << rule_number << "\n";
    // }

    auto translate_to_terminal_rules = [&terminal_rules](std::string const& entry)
    {
        std::list<int> result(entry.size());
        std::transform(entry.begin(), entry.end(), result.begin(), [&terminal_rules](char c){
            return terminal_rules[c];
        });

        return result;
    };

    int count{0};
    for(auto entry : entries)
    {
        auto entry_rules = translate_to_terminal_rules(entry);

        std::list<std::list<int>> expanded_rules{{0}};
        while(!entry_rules.empty() && expanded_rules.size())
        {

            std::list<std::list<int>> expanded_rules2;
            //expand leftmost rules
            for(auto &subrules : expanded_rules)
            {
                auto leftmost_rule = subrules.front();

                if (rules.find(leftmost_rule) == std::end(rules)) {
                    expanded_rules2.push_back(subrules);
                }
                else
                {
                    subrules.pop_front();
                    auto [first, last] = rules.equal_range(leftmost_rule);
                    for(; first != last; ++first)
                    {
                        auto new_subrules = subrules;
                        auto s = first->second;
                        for(auto r = s.rbegin(); r != s.rend(); ++r)
                            new_subrules.push_front(*r);
                        expanded_rules2.push_back(new_subrules);
                    }
                }
            }

            expanded_rules = expanded_rules2;

            const bool all_leftmost_rules_are_terminal = std::all_of(std::begin(expanded_rules),
                                                                     std::end(expanded_rules),
                                                                     [&rules](auto const& subrules)
                                                                     {
                                                                         return rules.find(subrules.front()) == std::end(rules);
                                                                     });

            if(all_leftmost_rules_are_terminal)
            {
                auto it = std::remove_if(std::begin(expanded_rules), std::end(expanded_rules), [&entry_rules](auto &subrules)
                {
                    auto leftmost_entry = entry_rules.front();
                    auto leftmost_rule = subrules.front();
                    bool match = leftmost_entry == leftmost_rule;
                    if(match)
                        subrules.pop_front();
                    return !match;
                });
                expanded_rules.erase(it, expanded_rules.end());
                if(!expanded_rules.empty())
                {
                    entry_rules.pop_front();
                }
            }

            if(expanded_rules.size() == 1 && (*std::begin(expanded_rules)).empty())
                break;
        }

        if(entry_rules.empty() && expanded_rules.size() == 1)
            ++count;
    }

    std::cout << count << "\n";


    return 0;
}
