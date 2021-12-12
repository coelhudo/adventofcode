#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <sstream>
#include <regex>
#include <tuple>
#include <set>

auto print_coordinates = [](const auto &span) {
    for (const auto &p : span) {
        std::cout << p.first << " => " << std::get<0>(p.second) << " " << std::get<1>(p.second) << '\n';
    }
};

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::ifstream ifs{argv[1]};
    std::string line;
    using coordinate = std::tuple<int, int>;
    std::multimap<int, coordinate> horizontal;
    std::multimap<int, coordinate> vertical;
    std::multimap<int, coordinate> diagonal;
    std::set<int> h_entry;
    std::set<int> v_entry;
    std::set<int> d_entry;

    auto to_int = [](auto regex_it) {
        return std::atoi(regex_it->str().c_str());
    };

    while(std::getline(ifs, line)) {
        std::regex number_regex("(\\d+)");
        auto first = std::sregex_iterator(line.begin(), line.end(), number_regex);
        auto second = std::next(first), third = std::next(second), fourth = std::next(third);
        auto x1 = to_int(first), y1 = to_int(second), x2 = to_int(third), y2 = to_int(fourth);
        if (x1 == x2) {
            v_entry.insert(x1);
            if(y1 <= y2)
                vertical.emplace(std::make_pair(x1, std::make_tuple(y1, y2)));
            else
                vertical.emplace(std::make_pair(x1, std::make_tuple(y2, y1)));
        } else if (y1 == y2) {
            h_entry.insert(y1);
            if(x1 <= x2)
                horizontal.emplace(std::make_pair(y1, std::make_tuple(x1, x2)));
            else
                horizontal.emplace(std::make_pair(y1, std::make_tuple(x2, x1)));
        } else {
            // TODO
            d_entry.insert(x1);
            diagonal.emplace(std::make_pair(x1, std::make_tuple(y1, x2)));
        }
    }

    // std::cout << "horizontal\n";
    // print_coordinates(horizontal);
    // std::cout << "vertical\n";
    // print_coordinates(vertical);
    // std::cout << "diagonal\n";
    // print_coordinates(diagonal);

    //TODO: fix this
    for(auto line : h_entry) {
        auto result = horizontal.equal_range(line);
        auto n_elements = std::distance(result.first, result.second);
        if (n_elements > 1) {
            auto first = result.first->second;
            auto second = std::next(result.first)->second;
            auto x1 = std::get<0>(first);
            auto x2 = std::get<1>(first);
            auto x3 = std::get<0>(second);
            auto x4 = std::get<1>(second);

            while(result.first != result.second) {
                std::cout << result.first->first << ": " << std::get<0>(result.first->second) << ' ' << std::get<1>(result.first->second) << '\n';
                ++result.first;
            }
        }
    }

    return 0;
}
