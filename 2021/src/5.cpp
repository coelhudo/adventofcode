#include <fstream>
#include <functional>
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

    auto insert_coordinates = [&](int i, int j) {
        auto it = coordinates.emplace(i, j);
        if(!it.second) {
            overlapping_coordinates[{i, j}] += 1;
        }
    };

    int max_x = 0;
    int max_y = 0;
    // O(N log N) where N is the number of possible points
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

            for (auto y = start; y <= end; ++y) {
                insert_coordinates(x1, y);
            }
        } else if (y1 == y2) {
            auto start = x1, end = x2;
            if (x1 > x2)
                std::swap(start, end);

            for (auto x = start; x <= end; ++x) {
                insert_coordinates(x, y1);
            }
        } else {
            // Comment this for Part 1
            auto next = [](int &i) { return ++i;};
            auto prev = [](int &i) { return --i;};
            auto le = [](int i, int j) { return i <= j;};
            auto ge = [](int i, int j) { return i >= j;};

            auto advance_x = (x1 <= x2) ? next : prev;
            auto cmp_x = (x1 <= x2) ? le : ge;

            auto advance_y = (y1 <= y2) ? next : prev;
            for(auto x = x1, y = y1; cmp_x(x, x2); advance_x(x), advance_y(y)) {
                insert_coordinates(x, y);
            }
        }
    }

    // for(int i = 0; i <= max_y; ++i) {
    //     for(int j = 0; j <= max_x; ++j) {
    //         int n_overlaps = (overlapping_coordinates.contains({j, i})) ? overlapping_coordinates[{j, i}] : 0;
    //         std::cout << (coordinates.contains({j, i}) ? 1 + n_overlaps : 0) << ' ';
    //     }
    //     std::cout << '\n';
    // }
    std::cout << "Part 2: " << overlapping_coordinates.size() << '\n';

    return 0;
}
