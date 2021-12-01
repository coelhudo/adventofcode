#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>


// O(n)
auto count_increases(const std::vector<int> &inputs) {
    auto counter = 0;
    for(std::size_t i = 1; i < inputs.size(); ++i) {
        if (inputs.at(i) > inputs.at(i-1)) {
            ++counter;
        }
    }
    return counter;
}

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
        std::cout << count_increases(inputs) << '\n';
    }

    // Part 2
    {
        //O(n)
        std::vector<int> grouped_inputs;
        for(std::size_t i = 0; i < inputs.size() - 2; ++i) {
            grouped_inputs.push_back(inputs.at(i) + inputs.at(i+1) + inputs.at(i+2));
        }

        std::cout << count_increases(grouped_inputs) << '\n';
    }

    return 0;
}
