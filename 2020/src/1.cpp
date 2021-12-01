#include <iostream>
#include <fstream>
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <utility>
#include <iterator>
#include <optional>

std::optional<std::pair<int, int>> find_sum(std::unordered_set<int> const& inputs, int value)
{
    for(auto i : inputs)
    {
        auto candidate = value - i;
        auto candidate_it = inputs.find(candidate);
        if(candidate_it != std::end(inputs))
        {
            return std::pair<int, int>{i, *candidate_it};
        }
    }

    return std::optional<std::pair<int, int>>{};
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));
    std::unordered_set<int> inputs;

    std::ifstream ifs{argv[1]};
    std::copy(std::istream_iterator<int>{ifs}, std::istream_iterator<int>{},
              std::inserter(inputs, std::begin(inputs)));

    std::cout << "find sum of two numbers that result in 2020:\n";
    auto result = find_sum(inputs, 2020);
    if(result)
    {
        auto [first, second] = *result;
        std::cout << first << ' ' << second << '\n';
        std::cout << (first * second) << '\n';
    }

    std::cout << "find sum of three numbers that result in 2020:\n";
    for(auto i : inputs)
    {
        auto candidate = 2020 - i;
        auto result = find_sum(inputs, candidate);
        if(result)
        {
            auto [first, second] = *result;
            std::cout << i << ' ' << first << ' ' << second << '\n';
            std::cout << (first * second * i) << '\n';
            break;
        }
    }

    return 0;
}
