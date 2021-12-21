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
    const std::map<unsigned long, int> mapped_integers = {
        {(a | b | c | e | f | g).to_ullong(), 0},
        {(c | f).to_ulong(), 1},
        {(a | c | d | e | g).to_ulong(), 2},
        {(a | c | d | f | g).to_ulong(), 3},
        {(b | c | d | f).to_ulong(), 4},
        {(a | b | d | f | g).to_ulong(), 5},
        {(a | b | d | e | f | g).to_ulong(), 6},
        {(a | c | f).to_ulong(), 7},
        {(a | b | c | d | e | f | g).to_ulong(), 8},
        {(a | b | c | d | f | g).to_ulong(), 9}
    };

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
    counter = 0;
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

        auto all_zero_six_nine = all | std::views::filter([](const auto &v) {
            return v.count() == 6;
        });

        auto zero_six_nine = std::reduce(all_zero_six_nine.begin(), all_zero_six_nine.end(), digit_t(0xff), [](auto a, auto b) { return a & b;});

        auto new_a = seven ^ one;
        auto new_d = four & two_three_five;
        auto new_g = (new_a | new_d) ^ two_three_five;
        auto new_b = (one | new_d) ^ four;
        auto new_e = (one | new_b | two_three_five) ^ eight;
        auto new_f = (new_a | new_b | new_g) ^ zero_six_nine;
        auto new_c = one ^ new_f;

        const std::map<unsigned long, char> new_digits{
            {new_a.to_ulong(), 'a'}, {new_b.to_ulong(), 'b'}, {new_c.to_ulong(), 'c'},
            {new_d.to_ulong(), 'd'}, {new_e.to_ulong(), 'e'}, {new_f.to_ulong(), 'f'},
            {new_g.to_ulong(), 'g'}
        };

        std::stringstream ss;
        for (auto output : outputs) {
            digit_t result{0b0000000};
            for(digit_t mask = 0b0000001; mask != 0b0; mask <<= 1) {
                if((mask & output).to_ulong()) {
                    char current_char = new_digits.at((mask & output).to_ulong());
                    digit_t current = mapped_digits.at(current_char);
                    result |= current;
                }
            }

            ss << mapped_integers.at(result.to_ullong());
        }
        counter += std::atoi(ss.str().c_str());
    }

    std::cout << "Part 2: " << counter << '\n';

    return 0;
}
