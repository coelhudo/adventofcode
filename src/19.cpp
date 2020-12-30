#include <cstddef>
#include <ios>
#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <cmath>

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


    //All the inputs have the cycles specified within the rule
    //Ex.: 8 -> 42 8
    //So, we only need to search for this in each rule
    std::map<int, std::vector<int>> cyclic_rules;
    for(auto it = rules.begin(); it != rules.end();)
    {
        auto [rule_number, subrules] = *it;
        const bool cyclic = std::find(std::begin(subrules), std::end(subrules), rule_number) != std::end(subrules);
        if(cyclic)
        {
            auto cyclic_rule_it = std::remove(std::begin(subrules), std::end(subrules), rule_number);
            subrules.erase(cyclic_rule_it);
            cyclic_rules.insert({rule_number, subrules});
            it = rules.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // for(auto [rule, subrules]: rules)
    // {
    //     std::cout << rule << ": ";
    //     print_container(subrules);
    // }

    // std::cout << "cyclic rules" << "\n";

    // for(auto [rule, subrules]: cyclic_rules)
    // {
    //     std::cout << rule << ": ";
    //     print_container(subrules);
    // }

    auto translate_to_terminal_rules = [&terminal_rules](std::string const& entry)
    {
        std::list<int> result(entry.size());
        std::transform(entry.begin(), entry.end(), result.begin(), [&terminal_rules](char c){
            return terminal_rules[c];
        });

        return result;
    };

    bool debug = false;
    auto print_expanded_rules = [debug](auto const& expanded_rules, std::string const& delimiter)
    {
        if(!debug)
            return;
        std::cout << "======== " << delimiter << " start ========" << '\n';
        for(auto subrules: expanded_rules)
            print_container(subrules);
        std::cout << "======== " << delimiter << " end ========" << '\n';
    };

    int count{0};
    for(auto entry : entries)
    {
        auto entry_rules = translate_to_terminal_rules(entry);
        // print_container(entry_rules);

        std::list<std::list<int>> expanded_rules{{0}};
        while(!entry_rules.empty() && !expanded_rules.empty())
        {
            print_expanded_rules(expanded_rules, "begin");
            std::list<std::list<int>> current_expanded_rules;
            //expand leftmost rules
            for(auto &subrules : expanded_rules)
            {
                auto leftmost_rule = subrules.front();

                //If leftmost is a terminal rule, just add the rule
                //and move on
                if (rules.find(leftmost_rule) == std::end(rules)) {
                    current_expanded_rules.push_back(subrules);
                }
                else
                {
                    //remove the first and expand it
                    subrules.pop_front();

                    auto [first, last] = rules.equal_range(leftmost_rule);
                    //we don't to search when the current subrules
                    //size is greater than the current entry size
                    for(; first != last && subrules.size() <= entry_rules.size(); ++first)
                    {
                        auto new_subrules = subrules;
                        auto s = first->second;
                        for(auto r = s.rbegin(); r != s.rend(); ++r)
                        {
                            new_subrules.push_front(*r);
                        }
                        current_expanded_rules.push_back(new_subrules);
                    }

                    //If we have a cycle we will expand as many times
                    //as necessary. It depends on the entry's size
                    auto cyclic_subrule = cyclic_rules.find(leftmost_rule);
                    if(cyclic_subrule != cyclic_rules.end())
                    {
                        auto c = cyclic_subrule->second;
                        int n_expansion = std::sqrt(entry.size());
                        for(int i = 2; i < n_expansion; ++i)
                        {
                            std::list<int> v;
                            for(auto subrule: c)
                            {
                                std::fill_n(std::back_inserter(v), i, subrule);
                            }

                            std::copy(subrules.begin(), subrules.end(), std::back_inserter(v));
                            current_expanded_rules.push_back(v);
                        }
                    }
                }
            }

            expanded_rules = current_expanded_rules;

            print_expanded_rules(expanded_rules, "middle");

            const bool all_leftmost_rules_are_terminal = std::all_of(std::begin(expanded_rules),
                                                                     std::end(expanded_rules),
                                                                     [&rules](auto const& subrules)
                                                                     {
                                                                         return rules.find(subrules.front()) == std::end(rules);
                                                                     });

            // print_container(entry_rules); If all leftmost rules in

            if(!all_leftmost_rules_are_terminal)
                continue;

            //all expanded_rules are a terminal rule we can compare
            //against the current entry
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

            //no more rules to expand, so no need to continue to search;
            if(expanded_rules.empty())
                break;

            entry_rules.pop_front();
            auto result = std::count_if(std::begin(expanded_rules), std::end(expanded_rules), [&entry_rules](auto &subrules)
            {
                return subrules.empty();
            });

            if(entry_rules.empty() && result == 1)
                ++count;

            auto empty_subrule_it = std::remove_if(std::begin(expanded_rules), std::end(expanded_rules), [&entry_rules](auto &subrules)
            {
                return subrules.empty();
            });
            expanded_rules.erase(empty_subrule_it, expanded_rules.end());
        }
    }

    std::cout << "Part 1: " << count << "\n";


    return 0;
}
