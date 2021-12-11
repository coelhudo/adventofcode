#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

auto split(std::string line, char sep) {
    if (sep != ' ') {
        auto it = std::find(line.begin(), line.end(), sep);
        auto current = line.begin();
        while(it != line.end()) {
            *it = ' ';
            current = std::next(it);
            it = std::find(current, line.end(), sep);
        }
    }

    std::vector<std::string> draws;
    std::stringstream ss(line);
    while(!ss.eof()) {
        std::string draw;
        ss >> draw;
        draws.emplace_back(draw);
    }

    return draws;
}

template<typename Items>
void print(const Items &items) {
    for(auto item : items) {
        std::cout << item << ' ';
    }
    std::cout << '\n';
}

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::ifstream ifs{argv[1]};

    std::string line;
    std::getline(ifs, line);
    std::vector<std::string> draws = split(line, ',');

    std::cout << line << '\n';
    print(draws);

    std::stringstream ss;
    while(std::getline(ifs, line)) {
        if (line.empty()) {
            auto bingo_board = split(ss.str(), ' ');
            print(bingo_board);
            std::cout << "next!\n";
            ss.str("");
            continue;
        }
        ss << line << ' ';
    }

    auto bingo_board = split(ss.str(), ' ');
    print(bingo_board);

    return 0;
}
