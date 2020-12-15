#include <bits/c++config.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <regex>
#include <vector>
#include <iterator>

constexpr std::size_t BITS = 36;

std::size_t part_a(std::vector<std::string> const& entries)
{
    std::bitset<BITS> bitmask_and;
    std::bitset<BITS> bitmask_or;

    std::map<std::size_t, std::size_t> memory;

    for(auto entry : entries)
    {

        if(entry.substr(0, 4) == "mask") {
            const std::string bitmask = entry.substr(7);
            std::string bitmask_and_value = bitmask;
            std::string bitmask_or_value = bitmask;
            std::replace(std::begin(bitmask_and_value), std::end(bitmask_and_value), 'X', '1');
            std::replace(std::begin(bitmask_or_value), std::end(bitmask_or_value), 'X', '0');

            bitmask_and = std::bitset<BITS>(bitmask_and_value);
            bitmask_or = std::bitset<BITS>(bitmask_or_value);
        } else {
            std::smatch match;
            std::regex pattern{R"(mem\[(\d+)\] = (\d+))"};
            std::regex_match(entry, match, pattern);
            std::size_t address = std::stoull(match[1]);
            std::size_t value = std::stoull(match[2]);

            std::bitset<BITS> valuebits(value);
            auto result = valuebits & bitmask_and | bitmask_or;
            memory[address] = result.to_ullong();
        }
    }

    std::size_t total{};
    for(auto [address, value]: memory)
    {
        total += value;
        //std::cout << address << ' ' << value << '\n';
    }

    return total;
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::vector<std::string> entries;

    std::string line;
    while(std::getline(ifs, line))
        entries.push_back(line);


    //Part A
    auto total = part_a(entries);
    std::cout << "total: " << total << '\n';

    return 0;
}
