#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>


int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::vector<int> inputs;

    std::ifstream ifs{argv[1]};
    std::copy(std::istream_iterator<int>{ifs}, std::istream_iterator<int>{},
              std::inserter(inputs, std::begin(inputs)));

    // Part 1
    {
        auto counter = 0;
        for(std::size_t i = 1; i < inputs.size(); ++i) {
            if (inputs.at(i) > inputs.at(i-1)) {
                ++counter;
            }
        }

        std::cout << counter << '\n';
    }

    return 0;
}
