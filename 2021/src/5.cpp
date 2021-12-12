#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <regex>
#include <tuple>

auto print_coordinates = [](const auto &span) {
    for(auto s : span) {
        auto first = std::get<0>(s);
        auto second = std::get<1>(s);
        std::cout << "(" << std::get<0>(first) << ", " << std::get<1>(first) << ") : (";
        std::cout << std::get<0>(second) << ", " << std::get<1>(second) << ")\n";
    }
};

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::ifstream ifs{argv[1]};
    std::string line;
    using start = std::tuple<int, int>;
    using end = std::tuple<int, int>;
    std::vector<std::tuple<start, end>> vents;

    auto to_int = [](auto regex_it) {
        return std::atoi(regex_it->str().c_str());
    };

    while(std::getline(ifs, line)) {
        std::regex number_regex("(\\d+)");
        auto first = std::sregex_iterator(line.begin(), line.end(), number_regex);
        auto second = std::next(first);
        auto third = std::next(second);
        auto fourth = std::next(third);
        vents.push_back({{to_int(first), to_int(second)}, {to_int(third), to_int(fourth)}});
    }

    print_coordinates(vents);

    return 0;
}
