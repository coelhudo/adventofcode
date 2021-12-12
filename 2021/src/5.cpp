#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <sstream>
#include <regex>
#include <tuple>

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

    auto to_int = [](auto regex_it) {
        return std::atoi(regex_it->str().c_str());
    };

    while(std::getline(ifs, line)) {
        std::regex number_regex("(\\d+)");
        auto first = std::sregex_iterator(line.begin(), line.end(), number_regex);
        auto second = std::next(first), third = std::next(second), fourth = std::next(third);
        auto x1 = to_int(first), y1 = to_int(second), x2 = to_int(third), y2 = to_int(fourth);
        if (x1 == x2) {
            if(y1 <= y2)
                vertical.emplace(std::make_pair(x1, std::make_tuple(y1, y2)));
            else
                vertical.emplace(std::make_pair(x1, std::make_tuple(y2, y1)));
        } else if (y1 == y2) {
            if(x1 <= x2)
                horizontal.emplace(std::make_pair(y1, std::make_tuple(x1, x2)));
            else
                horizontal.emplace(std::make_pair(y1, std::make_tuple(x2, x1)));
        } else {
            // TODO
            diagonal.emplace(std::make_pair(x1, std::make_tuple(y1, x2)));
        }
    }

    std::cout << "horizontal\n";
    print_coordinates(horizontal);
    std::cout << "vertical\n";
    print_coordinates(vertical);
    std::cout << "diagonal\n";
    print_coordinates(diagonal);
    // for(auto vent : vents) {
    //     auto first = std::get<0>(vent);
    //     auto second = std::get<1>(vent);

    //     std::cout << "(" << std::get<0>(first) << ", " << std::get<1>(first) << ") : (";
    //     std::cout << std::get<0>(second) << ", " << std::get<1>(second) << ")\n";
    // }

    return 0;
}
