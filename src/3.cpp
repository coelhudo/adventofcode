#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <vector>
#include <algorithm>
#include <array>


//This takes O(toboggan_grid size) since all the other operations occur in constant time
int main(int argc, char *argv[])
{
    assert(("expected input", argc == 2));

    //strategies with first as horizontal stride and second as vertical stride
    std::array<std::pair<std::size_t, std::size_t>, 5> strategies = {
        std::make_pair(1, 1), {3, 1}, {5, 1}, {7, 1}, {1, 2}
    };


    std::vector<std::string> toboggan_grid;

    //read input
    std::ifstream ifs{argv[1]};

    std::copy(std::istream_iterator<std::string>{ifs}, std::istream_iterator<std::string>{},
              std::back_inserter(toboggan_grid));

    int trees_counter = 1;

    //O(1) since number of strategies is limited
    for(auto strategy : strategies )
    {
        auto [hstride, vstride] = strategy;
        auto current_hpos = hstride;
        auto current_vpos = vstride;

        const auto max_unique_length = toboggan_grid.at(0).size();

        int tree_counter = 0;

        //O(toboggan_grid size)
        while(current_vpos < toboggan_grid.size()) {

            //O(1)
            char tile = toboggan_grid.at(current_vpos).at(current_hpos);

            current_hpos = (current_hpos + hstride) % max_unique_length;
            current_vpos += vstride;

            if(tile == '#')
                ++tree_counter;
        }
        std::cout << "Right " << hstride << ", down " << vstride << ". Tree encounters: " << tree_counter << '\n';
        trees_counter *= tree_counter;
    }

    std::cout << "Trees multiplied: " << trees_counter << '\n';

    return 0;
}
