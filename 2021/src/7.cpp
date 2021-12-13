// Most of the files will follow this pattern
#include <fstream>
#include <iostream>
#include <limits>
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
    // std::stringstream oss{"16,1,2,0,4,2,7,1,2,14"};
    std::string input;
    // std::getline(oss, input);
    std::getline(ifs, input);
    std::stringstream ss(input);
    std::string line;

    std::vector<int> fuels;
    int max = 0;
    while(std::getline(ss, line, ',')) {
        max = std::max(std::atoi(line.c_str()), max);
        fuels.emplace_back(std::atoi(line.c_str()));
    }


    // naive search
    int min_count = std::numeric_limits<int>::max();
    for(int pos = 1; pos <= max; ++pos) {
        int count = 0;
        for(auto fuel : fuels) {
            count += std::abs(fuel - pos);
        }
        min_count = std::min(count, min_count);
    }
    std::cout << "Part 1: " << min_count << '\n';

    return 0;
}
