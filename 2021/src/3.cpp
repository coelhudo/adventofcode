// Most of the files will follow this pattern
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>
#include <bitset>
#include <functional>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::ifstream ifs{argv[1]};
    std::vector<std::string> inputs{std::istream_iterator<std::string>{ifs}, std::istream_iterator<std::string>{}};

    const int input_size = static_cast<int>((*inputs.cbegin()).size());

    // O(n)
    std::vector<int> counter(input_size);
    for(auto input : inputs) {
        for (std::size_t i = 0; i < counter.size(); ++i) {
            if (input[i] == '1')
                ++counter[i];
        }
    }

    const int half = inputs.size() >> 1;

    //O(1)
    int gamma{0};
    int epsilon{0};
    for (auto c: counter) {
        gamma <<= 1;
        epsilon <<= 1;

        (c > half) ? (gamma |= 1) : (epsilon |= 1);
    }

    std::cout << "Part 1 " << gamma * epsilon << '\n';

    //O(N log N)
    std::set<std::string> sorted_inputs{inputs.cbegin(), inputs.cend()};

    //O(log N)
    auto find_entry = [&sorted_inputs, &input_size](auto cmp) {
        auto start = sorted_inputs.cbegin();
        auto end = sorted_inputs.cend();

        for (int i = 0; i < input_size; ++i) {
            auto it = std::find_if(start, end, [i](std::string_view entry) {
                return entry[i] == '1';
            });
            const int zeros = std::distance(start, it);
            const int ones = std::distance(it, end);
            if (zeros == 0 || ones == 0)
                break;

            if (cmp(zeros, ones)) {
                end = it;
            } else {
                start = it;
            }
        }
        return std::bitset<32>(*start).to_ullong();
    };

    auto oxi_gen = find_entry(std::greater<int>());
    auto co2_scrubber = find_entry(std::less_equal<int>());
    std::cout << "Part 2: " << oxi_gen * co2_scrubber << '\n';

    return 0;
}
