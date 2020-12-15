#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

int main(int argc, char *argv[])
{
    std::vector<int> input{15,12,0,14,3,1};
    const auto initial_size = input.size();

    auto current = input.rbegin();
    auto nth_number{2020 - input.size()};
    while(nth_number--) {
        auto pos = std::find(std::next(current), std::rend(input), *current);
        if(pos != std::rend(input)) {
            input.push_back(pos - current);
        } else {
            input.push_back(0);
        }
        current = input.rbegin();
    }

    for(auto n: input)
        std::cout << n << ' ';
    std::cout << '\n';

    std::cout << *input.rbegin() << '\n';

    return 0;
}
