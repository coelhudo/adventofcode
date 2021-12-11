#include <fstream>
#include <iostream>
#include <stdexcept>
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

class BingoBoard {
public:
    explicit(true) BingoBoard(std::vector<std::string> items) : items(items), size(items.at(0).size()) {

    }

    std::string_view operator()(int row, int column) const {
        int index = row * size + column;
        if (index > size * size || index < 0) {
            throw std::range_error("Invalid row or column");
        }

        return items.at(index);
    }

    const std::vector<std::string> items;
    const int size;
};

std::ostream& operator<<(std::ostream& os, const BingoBoard& board) {
    for(auto item : board.items) {
        os << item << ' ';
    }
    os << '\n';
    return os;
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

    std::vector<BingoBoard> bingo_boards;
    std::stringstream ss;
    while(std::getline(ifs, line)) {
        if (line.empty()) {
            bingo_boards.emplace_back(split(ss.str(), ' '));
            ss.str("");
            continue;
        }
        ss << line << ' ';
    }

    bingo_boards.emplace_back(split(ss.str(), ' '));

    print(bingo_boards);

    return 0;
}
