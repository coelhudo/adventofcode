#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <unordered_map>

int main(int argc, char *argv[])
{
    //I have to keep just the last position of each number I only need something associative
    //Since the order doesn't matter, an unordered_map yields a better performance
    std::unordered_map<int,int> input{{15,0}, {12,1}, {0,2}, {14, 3}, {3,4}, {1,5}};
    auto nth_number{30000000 - input.size()};
    auto current = 0;
    auto current_pos = 6;

    int last_n{};

    while(nth_number--) {
        last_n = current;

        auto pos = input.find(current);
        bool has_been_spoken = pos != std::end(input);

        if(has_been_spoken) {
            auto last_time_spoken = pos->second;
            input[current] = current_pos;
            current = current_pos - last_time_spoken;
            ++current_pos;
        } else {
            input[current] = current_pos;
            ++current_pos;
            current = 0;
        }
    }

    std::cout << last_n << '\n';

    return 0;
}
