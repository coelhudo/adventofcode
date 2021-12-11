#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>
#include <cmath>

template<typename Items>
void print(const Items &items) {
    for(auto item : items) {
        std::cout << item << ' ';
    }
    std::cout << '\n';
}

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

class BingoBoard {
public:
    explicit(true) BingoBoard(std::vector<std::string> items) : items(items), size(std::sqrt(items.size())) {
        for(int i = 0; i < size; ++i) {
            for(int j = 0; j < size; ++j) {
                auto item = operator()(i, j);
                inverted_index[std::string(item)] = std::make_pair(i, j);
            }
        }
    }

    const std::string& operator()(int row, int column) const {
        int index = row * size + column;
        if (index > size * size || index < 0) {
            throw std::range_error("Invalid row or column");
        }

        return items.at(index);
    }

    bool mark(const std::string &item) {
        if(!has_item(item)) {
            return false;
        }

        marked.insert(item);

        auto item_position = inverted_index[item];
        bool found_row = true;
        for (int i = 0; i < size; ++i) {
            auto item = operator()(std::get<0>(item_position), i);
            found_row &= marked.contains(item);
        }

        if(found_row) {
            return true;
        }

        bool found_column = true;
        for (int i = 0; i < size; ++i) {
            auto item = operator()(i, std::get<1>(item_position));
            found_column &= marked.contains(item);
        }

        return found_column;
    }

    bool has_item(const std::string &item) const {
        return inverted_index.contains(item);
    }

    const std::vector<std::string> items;
    const int size;

    auto unmarked() const {
        const std::set<std::string> all{items.cbegin(), items.cend()};
        std::vector<std::string> result;
        std::set_difference(all.cbegin(), all.cend(),
                            marked.cbegin(), marked.cend(),
                            std::back_inserter(result));
        return result;
    }
private:
    std::map<std::string, std::tuple<int,int>> inverted_index;
    std::set<std::string> marked;
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

    // std::cout << line << '\n';
    // print(draws);

    std::vector<BingoBoard> bingo_boards;
    std::stringstream ss;
    std::getline(ifs, line); //skip next line since it is empty
    while(std::getline(ifs, line)) {
        if (line.empty()) {
            bingo_boards.emplace_back(split(ss.str(), ' '));
            ss.str("");
            continue;
        }
        ss << line << ' ';
    }

    bingo_boards.emplace_back(split(ss.str(), ' '));

    // print(bingo_boards);

    auto play = [&draws, &bingo_boards]() {
        for (auto draw : draws) {
            for(std::size_t i = 0; i < bingo_boards.size(); ++i) {
                if(bingo_boards.at(i).mark(draw)) {
                    std::cout << draw << '\n';
                    auto unmarked = bingo_boards.at(i).unmarked();
                    int sum = 0;
                    for(auto i : unmarked) {
                        std::cout << i << ' ';
                        sum += std::atoi(i.c_str());
                    }
                    std::cout << '\n' << sum << '\n';
                    std::cout << std::atoi(draw.c_str()) * sum << '\n';

                    return;
                }
            }
        }
    };

    play();

    return 0;
}
