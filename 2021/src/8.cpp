// Most of the files will follow this pattern
#include <bitset>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <iterator>
#include <set>
#include <map>
#include <ranges>

template<typename T>
void print(const T &container) {
    for(auto c : container) {
        std::cout << c;
    }
    std::cout << '\n';
}

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }
    std::ifstream ifs{argv[1]};
    std::string line;

    using digit_t = std::bitset<7>;
    using digits_t = std::vector<digit_t>;

    const digit_t g{"0000001"};
    const digit_t f = g << 1;
    const digit_t e = f << 1;
    const digit_t d = e << 1;
    const digit_t c = d << 1;
    const digit_t b = c << 1;
    const digit_t a = b << 1;
    const std::map<char, digit_t> mapped_digits{{'a', a}, {'b', b}, {'c', c}, {'d', d}, {'e', e}, {'f', f}, {'g', g}};

    std::vector<std::tuple<digits_t, digits_t, digits_t>> parsed_entries;
    while(std::getline(ifs, line)) {
        auto it = line.find( '|');
        if (it == std::string::npos)
            continue;

        std::stringstream ss(line.substr(0, it-1));

        digits_t entries;
        digits_t inputs;
        auto parse_line = [&entries, &mapped_digits](auto &digits, std::istream &stream) {
            std::string word;
            while(!stream.eof()) {
                stream >> word;
                digit_t digit;
                for(auto character : word) {
                    digit |= mapped_digits.at(character);
                }
                digits.emplace_back(digit);
                entries.emplace_back(digit);
            }
        };

        parse_line(inputs, ss);

        ss.str(line.substr(it+1));
        ss.clear();
        digits_t outputs;
        parse_line(outputs, ss);

        parsed_entries.emplace_back(std::tuple{entries, inputs, outputs});
    }

    // Part 1
    int counter = 0;
    for(auto [all, inputs, outputs] : parsed_entries) {
        for (auto output : outputs) {
            if(output.count() == 2 || output.count() == 3 || output.count() == 4 || output.count() == 7)
                ++counter;
        }
    }

    std::cout << "Part 1: " << counter << '\n';

    // // Part 2
    for(auto [all, inputs, outputs] : parsed_entries) {

        digits_t all_copy = all;
        auto one = *std::find_if(all.cbegin(), all.cend(), [](auto digit) { return digit.count() == 2; });
        auto seven = *std::find_if(all.cbegin(), all.cend(), [](auto digit) { return digit.count() == 3; });
        auto four = *std::find_if(all.cbegin(), all.cend(), [](auto digit) { return digit.count() == 4; });
        auto eight = *std::find_if(all.cbegin(), all.cend(), [](auto digit) { return digit.count() == 7; });

        auto all_two_three_five = all | std::views::filter([](const auto &v) {
            return v.count() == 5;
        });

        auto two_three_five = std::reduce(all_two_three_five.begin(), all_two_three_five.end(), digit_t(0xff), [](auto a, auto b) { return a & b;});

        std::cout << two_three_five << '\n';

        auto all_zero_six_nine = all | std::views::filter([](const auto &v) {
            return v.count() == 6;
        });

        auto zero_six_nine = std::reduce(all_zero_six_nine.begin(), all_zero_six_nine.end(), digit_t(0xff), [](auto a, auto b) { return a & b;});
        std::cout << zero_six_nine << '\n';

        auto new_a = seven ^ one;
        std::cout << "a: " << new_a << '\n';

        auto new_d = four & one & eight & zero_six_nine;
        std::cout << "d: " << new_d << '\n';

        auto new_b = four & one ^ new_d;
        std::cout << "b: " << new_b << '\n';

    }


    return 0;
}
