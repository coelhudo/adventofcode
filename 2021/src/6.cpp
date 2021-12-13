// Most of the files will follow this pattern
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <numeric>

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

    std::vector<long long int> fishes(9);
    while(std::getline(ss, line, ',')) {
        int index = std::atoi(line.c_str());
        fishes.at(index)++;
    }

    auto play = [](auto fishes, int days) {
        for(int day = 0; day < days; ++day) {
            long long int zeros = fishes.at(0);
            for(std::size_t i = 1; i < fishes.size(); ++i) {
                fishes.at(i-1) = fishes.at(i);
            }
            fishes.at(8) = zeros;
            fishes.at(6) += zeros;
        }

        long long int result = 0;
        return std::accumulate(fishes.cbegin(), fishes.cend(), result);
    };

    std::cout << "Part 1: " << play(fishes, 80) << '\n';
    std::cout << "Part 2: " << play(fishes, 256) << '\n';
    return 0;
}
