#include <bits/c++config.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <cassert>
#include <list>
#include <unordered_set>

const int PREAMBLE = 25;

bool its_sum_is_in_previous_numbers(std::size_t next_number, std::list<std::size_t> const& numbers)
{
    auto first = std::begin(numbers);
    auto last = std::begin(numbers);
    std::advance(last, PREAMBLE + 1);
    const std::unordered_set<std::size_t> s{first, last};

    for(auto current: s)
    {
        auto candidate = next_number - current;
        if(candidate != current && s.find(candidate) != std::cend(s)){
            return true;
        }
    }

    return false;
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::list<std::size_t> numbers;

    std::size_t number;
    while(ifs >> number) {
        numbers.push_back(number);
    }

    auto current_it = std::cbegin(numbers);
    std::advance(current_it, PREAMBLE + 1);

    while(its_sum_is_in_previous_numbers(*current_it, numbers) && current_it != std::cend(numbers))
    {
        numbers.pop_front();
        current_it = std::next(current_it);
    }

    std::cout << *current_it << '\n';

    return 0;
}
