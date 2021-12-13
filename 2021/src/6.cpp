// Most of the files will follow this pattern
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::ifstream ifs{argv[1]};
    std::string input;
    std::getline(ifs, input);
    std::stringstream ss(input);
    std::string line;

    std::vector<int> fishes;
    while(std::getline(ss, line, ',')) {
        fishes.emplace_back(std::atoi(line.c_str()));
    }

    for(int day = 0; day < 80; ++day) {
        // for (auto &fish : fishes) {
        //     std::cout << fish << " ";
        // }
        // std::cout << "\n";
        int zeros = 0;
        for (auto &fish : fishes) {
            if (fish-- == 0) {
                ++zeros;
                fish = 6;
            }
        }

        for(int i = 0; i < zeros; ++i) {
            fishes.emplace_back(8);
        }
    }

    std::cout << "Part 1: " << fishes.size() << '\n';

    return 0;
}
