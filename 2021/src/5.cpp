#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <sstream>
#include <regex>
#include <tuple>
#include <set>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::ifstream ifs{argv[1]};
    std::string line;
    std::set<std::tuple<int, int>> coordinates;
    std::map<std::tuple<int, int>, int> overlapping_coordinates;
    auto to_int = [](auto regex_it) {
        return std::atoi(regex_it->str().c_str());
    };

    int max_x = 0;
    int max_y = 0;
    while(std::getline(ifs, line)) {
        std::regex number_regex("(\\d+)");
        auto first = std::sregex_iterator(line.begin(), line.end(), number_regex);
        auto second = std::next(first), third = std::next(second), fourth = std::next(third);
        auto x1 = to_int(first), y1 = to_int(second), x2 = to_int(third), y2 = to_int(fourth);

        max_x = std::max(std::max(x1, x2), max_x);
        max_y = std::max(std::max(y1, y2), max_y);

        if (x1 == x2) {
            auto start = y1, end = y2;
            if (y1 > y2)
                std::swap(start, end);

            for (auto i = start; i <= end; ++i) {
                auto it = coordinates.emplace(x1, i);
                if(!it.second) {
                    overlapping_coordinates[{x1, i}] += 1;
                }
            }
        } else if (y1 == y2) {
            auto start = x1, end = x2;
            if (x1 > x2)
                std::swap(start, end);

            for (auto i = start; i <= end; ++i) {
                auto it = coordinates.emplace(i, y1);
                if(!it.second) {
                    overlapping_coordinates[{i, y1}] += 1;
                }
            }
        } else {
            // TODO
            // coordinates.emplace(std::make_pair(x1, std::make_tuple(y1, x2)));
        }
    }

    // for(int i = 0; i <= max_y; ++i) {
    //     for(int j = 0; j <= max_x; ++j) {
    //         int n_overlaps = (overlapping_coordinates.contains({j, i})) ? overlapping_coordinates[{j, i}] : 0;
    //         std::cout << (coordinates.contains({j, i}) ? 1 + n_overlaps : 0) << ' ';
    //     }
    //     std::cout << '\n';
    // }
    std::cout << "Part 1: " << overlapping_coordinates.size() << '\n';

    return 0;
}
