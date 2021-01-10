#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <ratio>
#include <vector>
#include <algorithm>
#include <list>
#include <chrono>
#include <map>
#include <execution>

template<typename Container>
void print_container(Container const& container, int current_cup)
{
    for(auto i : container)
        if(current_cup == i)
            std::cout << "(" << i << ") ";
        else
            std::cout << i << " ";
    std::cout << "\n";
}

void play_combat(int current_cup, int times, std::map<int, int> &adjacent_cups)
{
    for (int i = 0; i < times; ++i)
    {
        int cup_one = adjacent_cups[current_cup];
        int cup_two = adjacent_cups[cup_one];
        int cup_three = adjacent_cups[cup_two];
        int destination_cup = current_cup - 1;
        while(destination_cup == cup_one || destination_cup == cup_two || destination_cup == cup_three || destination_cup == 0)
        {
            --destination_cup;
            if(destination_cup < 1)
                destination_cup = 9;
        }
        // std::cout << "destination cup " << destination_cup << '\n';

        int adjacent_destination_cup = adjacent_cups[destination_cup];
        int adjacent_cup_three = adjacent_cups[cup_three];
        adjacent_cups[current_cup] = adjacent_cup_three;
        adjacent_cups[cup_three] = adjacent_destination_cup;
        adjacent_cups[destination_cup] = cup_one;
        current_cup = adjacent_cup_three;
    }
}

int main(int argc, char *argv[])
{
    //Part 1
    std::vector<int> puzzle_input_raw{8,7,2,4,9,5,1,3,6};
    // std::vector<int> puzzle_input_raw{3, 8, 9, 1, 2, 5, 4, 6, 7};
    std::map<int, int> adjacent_cups;
    for(int i = 0; i < puzzle_input_raw.size() - 1; ++i)
    {
        adjacent_cups[puzzle_input_raw.at(i)] = puzzle_input_raw.at(i + 1);
    }

    adjacent_cups[puzzle_input_raw.at(puzzle_input_raw.size() - 1)] = puzzle_input_raw.at(0);
    auto current_cup = puzzle_input_raw.at(0);

    play_combat(current_cup, 100, adjacent_cups);

    int next = adjacent_cups[1];
    while(next != 1)
    {
        std::cout << next;
        next = adjacent_cups[next];
    }
    std::cout << '\n';

    //Part 2
    int count = puzzle_input_raw.size() + 1;
    std::generate_n(std::inserter(puzzle_input_raw, puzzle_input_raw.begin()), 1000000 - puzzle_input_raw.size(), [&count]{ return count++;});
    adjacent_cups.clear();
    for(int i = 0; i < puzzle_input_raw.size() - 1; ++i)
    {
        adjacent_cups[puzzle_input_raw.at(i)] = puzzle_input_raw.at(i + 1);
    }

    adjacent_cups[puzzle_input_raw.at(puzzle_input_raw.size() - 1)] = puzzle_input_raw.at(0);

    // play_combat(current_cup, 10000000, adjacent_cups);

    return 0;
}
