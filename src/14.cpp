#include <bits/c++config.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <numeric>
#include <string>
#include <unordered_map>
#include <bitset>
#include <algorithm>
#include <regex>
#include <vector>
#include <iterator>
#include <cmath>

constexpr std::size_t BITS = 36;

auto parse_mem(std::string const& entry)
{
    std::smatch match;
    std::regex pattern{R"(mem\[(\d+)\] = (\d+))"};
    std::regex_match(entry, match, pattern);
    std::size_t address = std::stoull(match[1]);
    std::size_t value = std::stoull(match[2]);
    return std::make_pair(address, value);
}

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
            auto [address, value] = parse_mem(entry);

            std::bitset<BITS> valuebits(value);
            auto result = valuebits & bitmask_and | bitmask_or;
            memory[address] = result.to_ullong();
        }
    }

    return std::accumulate(memory.cbegin(), memory.cend(), static_cast<std::size_t>(0),
                           [](std::size_t i, std::pair<std::size_t, std::size_t> const &v){ return i + v.second;});
}

std::size_t part_b(std::vector<std::string> const& entries)
{
    std::map<std::size_t, std::size_t> memory;
    std::bitset<BITS> bitmask_or;
    std::bitset<BITS> base_bitmask;
    std::vector<int> x_pos;

    for(auto entry : entries)
    {

        if(entry.substr(0, 4) == "mask") {
            x_pos.clear();
            const std::string bitmask = entry.substr(7);
            std::string bitmask_value = bitmask;
            std::string base_bitmask_value = bitmask;
            auto it = std::find(bitmask.cbegin(), bitmask.cend(), 'X');
            while(it != bitmask.cend()) {
                x_pos.push_back(35 - (it - bitmask.cbegin()));
                it = std::find(++it, bitmask.cend(), 'X');
            }

            std::replace(std::begin(bitmask_value), std::end(bitmask_value), 'X', '1');
            std::replace(std::begin(base_bitmask_value), std::end(base_bitmask_value), '0', '1');
            std::replace(std::begin(base_bitmask_value), std::end(base_bitmask_value), 'X', '0');
            bitmask_or = std::bitset<BITS>(bitmask_value);
            base_bitmask = std::bitset<BITS>(base_bitmask_value);
        } else {
            auto [address, value] = parse_mem(entry);

            std::bitset<BITS> valuebits(address);

            auto masked_value = ((valuebits | bitmask_or) & base_bitmask);
            memory[masked_value.to_ullong()] = value;

            //Generate each combination possible and shift the 1 to the correct position
            auto n_combinations = std::pow(2, x_pos.size());

            const std::vector<int> x_pos2{x_pos.rbegin(), x_pos.rend()};
            //O(2^36 * 36) since we can have 36 Xs and we will perform the logical operators 36 times at maximum
            for(int i = 0; i < n_combinations; ++i)
            {
                std::bitset<BITS> x{0};
                for(int index = 0; index < x_pos2.size(); ++index) {
                    std::bitset<BITS> current_bit(((1 << index) & i) >> index);
                    auto mask = current_bit << x_pos2.at(index);
                    x |= mask;
                }

                //mask each new combination with the isolated value
                auto new_address = (masked_value | std::bitset<BITS>(x));
                memory[new_address.to_ullong()] = value;
            }
        }
    }

    return std::accumulate(memory.cbegin(), memory.cend(), static_cast<std::size_t>(0),
                           [](std::size_t i, std::pair<std::size_t, std::size_t> const &v){ return i + v.second;});
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
    std::cout << "Part A total: " << total << '\n';

    //Part B
    total = part_b(entries);
    std::cout << "Part B total: " << total << '\n';

    return 0;
}
