//This problem is about finding elements from a sequence that when added result into 2020.
//Works with two or three elements that their sum is 2020

#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <optional>
#include <tuple>

using namespace std;

//there is probably a bug here if no elements that sum to the expected sentinel.
//this is a naive approach
std::optional<std::tuple<int, int>> find_two_numbers_equals_to_sentinel(const std::set<int> &input, int sentinel, int skip)
{
    //input: set containing all values
    //sentinel: the value that we test if it is the sum of two elements from the input
    //skip: a number from the input to be skippde

    auto begin = std::begin(input);
    auto rbegin = std::rbegin(input);
    auto result = *begin + *rbegin;
    // The worst case is when the two elements are in the middle
    // of the sequence. Is this n^2? I believe so.
    while(result != sentinel && begin != std::end(input) && rbegin != std::rend(input)) {
        if(result > sentinel) {
            ++rbegin;
        } else {
            ++begin;
            if(*begin == skip)
                ++begin;
            rbegin = std::rbegin(input);
        }
        result = *begin + *rbegin;
    }

    if(result == sentinel) {
        return std::optional<std::tuple<int, int>>{std::make_tuple(*begin, *rbegin)};
    }

    return std::optional<std::tuple<int, int>>{};
}

int main(int argc, char *argv[])
{
    assert(("expect input file", argc == 2));

    std::set<int> input;
    std::ifstream ifs{argv[1]};
    auto begin = std::istream_iterator<int>{ifs};
    auto end = std::istream_iterator<int>{};

    //load content in a sort way
    std::copy(begin, end, std::inserter(input, std::begin(input))); // O(n)

    // testing with only two numbers
    // auto result = find_two_numbers_equals_to_sentinel(input, 2020, 0);
    // if(result.has_value()){
    //     auto values = *result;
    //     int first = std::get<0>(values);
    //     int second = std::get<1>(values);
    //     std::cout << "first " << first << ", second " << second << '\n';
    //     std::cout << "result " << (first * second) << '\n';
    // }

    //testing with three numbers. This will be no better than O(n^2)
    {
        auto begin = std::begin(input);
        while(begin != std::end(input)) {
            auto result = find_two_numbers_equals_to_sentinel(input, 2020 - *begin, *begin);
            if(result.has_value()){
                auto values = *result;
                int first = std::get<0>(values);
                int second = std::get<1>(values);
                std::cout << "first " << first << ", second " << second << ", third " << *begin << '\n';
                std::cout << "result " << (first * second * (*begin)) << '\n';
                break;
            }

            ++begin;
        }
    }



    return 0;
}
