#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::ifstream ifs{argv[1]};
    std::string line;
    std::getline(ifs, line);
    const int columns = line.length();
    std::cout << columns << '\n';
    ifs.clear();
    ifs.seekg(0, std::ios::beg);

    const std::vector<char> inputs{std::istream_iterator<char>{ifs}, std::istream_iterator<char>{}};
    std::cout << inputs.size() << '\n';

    const int rows = inputs.size() / columns;

    auto access = [&](int row, int column) -> int {
        if (row >= rows || column >= columns || row < 0 || column < 0) {
            return 10;
        }

        // Assuming that they are in sequence in the ASCII table
        return static_cast<int>(inputs.at(row * columns + column) - static_cast<int>('0'));
    };

    std::cout << access(1, 1) << '\n';

    int count = 0;
    for(int row = 0; row < rows; ++row) {
        for(int column = 0; column < columns; ++column) {
            auto current = access(row, column);
            auto above = access(row-1, column);
            auto below = access(row+1, column);
            auto left = access(row, column-1);
            auto right = access(row, column+1);
            if(current < above && current < below && current < left && current < right) {
                count += (current + 1);
            }
        }
    }

    std::cout << "Part 1: " << count << '\n';

    return 0;
}
