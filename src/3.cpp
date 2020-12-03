#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <vector>
#include <algorithm>


int main(int argc, char *argv[])
{
    assert(("expected input", argc == 2));

    constexpr auto hstride = 3;
    constexpr auto vstride = 1;

    std::vector<std::string> toboggan_grid;

    std::ifstream ifs{argv[1]};

    std::copy(std::istream_iterator<std::string>{ifs}, std::istream_iterator<std::string>{},
              std::back_inserter(toboggan_grid));

    std::size_t current_hpos = hstride;
    std::size_t current_vpos = vstride;

    const auto max_unique_length = toboggan_grid.at(0).size();

    int tree_counter = 0;

    while(current_vpos != toboggan_grid.size()) {

        char tile = toboggan_grid.at(current_vpos).at(current_hpos);

        current_hpos = (current_hpos + hstride) % max_unique_length;
        current_vpos += vstride;

        if(tile == '#')
            ++tree_counter;
    }

    std::cout << "Trees: " << tree_counter << '\n';


    return 0;
}
