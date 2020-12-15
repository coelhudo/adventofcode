#include <bits/c++config.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <algorithm>
#include <regex>

struct MaskEntry {
    std::string bitmask_and;
    std::string bitmask_or;
};

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::string line;

    constexpr std::size_t bits = 36;
    std::bitset<bits> bitmask_and;
    std::bitset<bits> bitmask_or;

    std::map<std::size_t, std::size_t> memory;



    while(std::getline(ifs, line))
    {

        if(line.substr(0, 4) == "mask") {
            const std::string bitmask = line.substr(7);
            std::string bitmask_and_value = bitmask;
            std::string bitmask_or_value = bitmask;
            std::replace(std::begin(bitmask_and_value), std::end(bitmask_and_value), 'X', '1');
            std::replace(std::begin(bitmask_or_value), std::end(bitmask_or_value), 'X', '0');

            bitmask_and = std::bitset<bits>(bitmask_and_value);
            bitmask_or = std::bitset<bits>(bitmask_or_value);
        } else {
            std::smatch match;
            std::regex pattern{R"(mem\[(\d+)\] = (\d+))"};
            std::regex_match(line, match, pattern);
            std::size_t address = std::stoull(match[1]);
            std::size_t value = std::stoull(match[2]);

            std::bitset<bits> valuebits(value);
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

    std::cout << "total: " << total << '\n';

    return 0;
}
