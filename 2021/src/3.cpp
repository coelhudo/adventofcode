// Most of the files will follow this pattern
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <bitset>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::ifstream ifs{argv[1]};
    std::vector<std::string> inputs{std::istream_iterator<std::string>{ifs}, std::istream_iterator<std::string>{}};

    std::vector<int> counter(static_cast<int>(inputs.at(0).size()));
    for(auto input : inputs) {
        for (std::size_t i = 0; i < counter.size(); ++i) {
            if (input[i] == '1')
                ++counter[i];
        }
    }

    const int half = inputs.size() >> 1;

    int gamma{0};
    int epsilon{0};
    for (auto c: counter) {
        gamma <<= 1;);
    }

    std::cout << "Part 1 " << gamma * epsilon << '\n';

    return 0;
}
