#include <bits/c++config.h>
#include <functional>
#include <ios>
#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <vector>
#include <algorithm>
#include <numeric>
#include <stack>
#include <tuple>

//stucked and had to search for help since brute force wasn't working. applying chinese remainder theorem
std::size_t crt(std::vector<int> const& buses_id, std::vector<int> const& buses_id_offset) {
    auto find_x = [](std::size_t candidate, std::size_t N_busid, std::size_t bus_id) {
        return (N_busid * candidate) % bus_id != 1;
    };

    std::size_t N = std::accumulate(std::cbegin(buses_id), std::cend(buses_id), static_cast<std::size_t>(1), std::multiplies<std::size_t>());

    std::vector<std::tuple<int, std::size_t, std::size_t>> result;
    for(auto index = 1; index < buses_id.size(); ++index) {
        auto bus_id = buses_id.at(index);
        auto N_i = N / bus_id;
        std::size_t i = 0;
        while(find_x(i, N_i, bus_id)) {
            ++i;
        }

        result.emplace_back(std::make_tuple(buses_id.at(index) - buses_id_offset.at(index), N_i, i));
    }

    std::size_t acc = 0;
    for(auto [a, b, c]: result) {
        acc += a * b * c;
    }

    return acc % N;
}


int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    int arrival;
    ifs >> arrival;

    std::vector<int> buses_id;
    std::vector<int> buses_id_offset;

    std::string value;
    int max_value = 0;
    int offset = 0;
    while(std::getline(ifs, value, ','))
    {
        if(value != "x") {
            buses_id.push_back(std::stoi(value));
            max_value = std::max(std::stoi(value), max_value);

            buses_id_offset.push_back(offset);
        }
        ++offset;
    }

    //Part A
    int earliest_bus = max_value;
    for(auto bus_id : buses_id) {
        earliest_bus = std::min(arrival - (arrival % bus_id) + bus_id, earliest_bus);
    }

    std::cout << "Part A\n";
    std::cout << earliest_bus * (earliest_bus - arrival % earliest_bus) << '\n';


    //Part B
    auto departure = crt(buses_id, buses_id_offset);
    std::cout << "Part B\n";
    std::cout << departure << '\n';

    return 0;
}
