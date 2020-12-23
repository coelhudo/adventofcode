#include <cstddef>
#include <ios>
#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <unordered_map>

void f(std::unordered_multimap<std::string, std::string> const& rules,
       std::map<std::string, std::string> const& terminal_rules,
       std::string candidate, bool& found,
       std::size_t maximum_nonterminal_length)
{
    auto [start_it, end_it] = rules.equal_range(candidate);
    if(start_it != end_it && start_it->second == "0")
    {
        found = true;
        return;
    }

    for(int start = 0; start != (candidate.size() - maximum_nonterminal_length); ++start)
    {
        auto prefix = candidate.substr(0, start);
        for(int n_characters = 1; n_characters <= maximum_nonterminal_length; ++n_characters)
        {
            auto sub = candidate.substr(start, n_characters);
            auto [start_it, end_it] = rules.equal_range(sub);
            while(start_it != end_it)
            {
                std::ostringstream oss;
                oss << prefix;
                oss << start_it->second;
                oss << candidate.substr(start+n_characters);
                f(rules, terminal_rules, oss.str(), found, maximum_nonterminal_length);
                if(found) //don't need to keep searching
                    return;
                ++start_it;
            }
        }
    }
};

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::unordered_multimap<std::string, std::string> rules;
    std::map<std::string, std::string> terminal_rules;
    std::vector<std::string> entries;

    std::ifstream ifs{argv[1]};
    std::string line;

    bool is_a_rule = true;

    auto transform_into_string = [](std::string nonterminal_symbol){
        auto space_pos = nonterminal_symbol.find(' ');
        while(space_pos != std::string::npos)
        {
            nonterminal_symbol.erase(space_pos,1);
            space_pos = nonterminal_symbol.find(' ');
        }
        return nonterminal_symbol;
    };

    std::size_t maximum_nonterminal_length{0};

    while(std::getline(ifs, line))
    {
        if(line.empty())
        {
            is_a_rule = false;
            continue;
        }

        if(is_a_rule) {
            auto pos = line.find(':');
            const auto rule_number = line.substr(0, pos);
            auto or_pos = line.find('|');
            if(or_pos == std::string::npos) //without the | we can have a terminal or nonterminal symbol
            {
                const auto start_quote = line.find('"');
                if(start_quote != std::string::npos)
                {
                    const auto end_quote = line.rfind('"');
                    auto terminal_symbol = line.substr(start_quote+1, end_quote-start_quote-1);
                    terminal_rules.insert({terminal_symbol, rule_number});
                }
                else
                {
                    auto nonterminal_symbol = transform_into_string(line.substr(pos+1));
                    rules.insert({nonterminal_symbol, rule_number});
                }
            }
            else
            {
                auto first_nonterminal_symbol = transform_into_string(line.substr(pos+1, or_pos-pos-1));
                auto second_nonterminal_symbol = transform_into_string(line.substr(or_pos+1));
                maximum_nonterminal_length = std::max(maximum_nonterminal_length, first_nonterminal_symbol.size());
                maximum_nonterminal_length = std::max(maximum_nonterminal_length, second_nonterminal_symbol.size());
                rules.insert({first_nonterminal_symbol, rule_number});
                rules.insert({second_nonterminal_symbol, rule_number});
            }
        } else {
            entries.push_back(line);
        }
    }

    for(auto entry: entries)
    {
        std::ostringstream oss;
        for(auto c: entry)
        {
            oss << terminal_rules.at(std::string(1,c));
        }

        auto candidate = oss.str();
        bool found = false;
        f(rules, terminal_rules, candidate, found, maximum_nonterminal_length);
        std::cout << entry << " found: " << std::boolalpha << found << '\n';
    }



    return 0;
}
