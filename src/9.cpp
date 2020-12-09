#include <bits/c++config.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <cassert>
#include <list>
#include <unordered_set>
#include <algorithm>
#include <tuple>

const int PREAMBLE = 25;

//O(n): similar to day 1 problem. Use a set to find if a number is the sum of two numbers in a container
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


//O(n)
std::tuple<std::size_t, std::size_t> bounds_of_sum_of_sequence(std::list<std::size_t> numbers, std::size_t number) {
    //find contiguous sequeuence in numbers that when added up result into "number"
    //returns the smallest and largest of this sequence
    auto top_it = std::begin(numbers);
    auto current_number_it = std::begin(numbers);
    std::size_t accumulator{};

    while(accumulator != number && current_number_it != std::cend(numbers) && *current_number_it < number) {
        accumulator += *current_number_it++;
        while(accumulator > number) {
            accumulator -= *top_it++;
        }
    }

    auto smallest = std::min_element(top_it, current_number_it);
    auto largest = std::max_element(top_it, current_number_it);
    return std::make_tuple(*smallest, *largest);
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

    auto numbers_copy = numbers;
    auto current_it = std::cbegin(numbers_copy);
    std::advance(current_it, PREAMBLE + 1);

    while(its_sum_is_in_previous_numbers(*current_it, numbers_copy) && current_it != std::cend(numbers_copy))
    {
        numbers_copy.pop_front();
        current_it = std::next(current_it);
    }

    std::cout << "Not sum of previous numbers " << *current_it << '\n';

    auto [smallest, largest] = bounds_of_sum_of_sequence(numbers, *current_it);

    std::cout << "smallest " << smallest << ", largest " << largest << '\n';
    std::cout << "result " << smallest + largest << '\n';


    return 0;
}
