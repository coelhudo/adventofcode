#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <vector>
#include <algorithm>

void print_container(std::vector<int> const& container, int current_cup)
{
    for(auto i : container)
        if(current_cup == i)
            std::cout << "(" << i << ") ";
        else
            std::cout << i << " ";
    std::cout << "\n";
}

int main(int argc, char *argv[])
{
    std::vector<int> puzzle_input{8,7,2,4,9,5,1,3,6};
    {
        //Part 1
        auto current_cup = puzzle_input.begin();
        for (int i = 0; i < 100; ++i)
        {
            int destination_cup = *current_cup - 1;

            auto next_cup_it = (current_cup + 1 == puzzle_input.end()) ? puzzle_input.begin() : current_cup + 1;
            //erase the three elements and save them into variables
            int cup_one = *next_cup_it;
            next_cup_it = puzzle_input.erase(next_cup_it);
            if(next_cup_it == puzzle_input.end())
                next_cup_it = puzzle_input.begin();

            int cup_two = *next_cup_it;
            next_cup_it = puzzle_input.erase(next_cup_it);
            if(next_cup_it == puzzle_input.end())
                next_cup_it = puzzle_input.begin();

            int cup_three = *next_cup_it;
            next_cup_it = puzzle_input.erase(next_cup_it);

            //search for the destination cup
            while(destination_cup == cup_one || destination_cup == cup_two || destination_cup == cup_three || destination_cup == 0)
            {
                --destination_cup;
                if(destination_cup < 1)
                    destination_cup = 9;
            }

            //if the current_cup is at the end of the list, we make
            //the next one the first of the list
            if(*current_cup == *puzzle_input.rbegin())
            {
                current_cup = puzzle_input.begin();
            }
            else
            {
                ++current_cup;
            }

            //saving since the iterator will be invalidated
            int next_cup = *current_cup;

            auto destination_cup_it = std::find(puzzle_input.begin(), puzzle_input.end(), destination_cup);
            if(*destination_cup_it == *puzzle_input.rbegin())
            {
                puzzle_input.push_back(cup_one);
                puzzle_input.push_back(cup_two);
                puzzle_input.push_back(cup_three);
            }
            else
            {
                puzzle_input.insert(destination_cup_it + 1, cup_one);
                puzzle_input.insert(destination_cup_it + 2, cup_two);
                puzzle_input.insert(destination_cup_it + 3, cup_three);
            }

            current_cup = std::find(puzzle_input.begin(), puzzle_input.end(), next_cup);
        }

        //Output using the number 1 as the reference number
        auto one_pos = std::find(puzzle_input.begin(), puzzle_input.end(), 1);
        auto current = std::next(one_pos);
        //everything that is after the number 1
        for(;current != puzzle_input.end(); ++current)
            std::cout << *current;

        //everything that is before the number 1
        for(auto it = puzzle_input.begin(); it != one_pos; ++it)
        {
            std::cout << *it;
        }
        std::cout << '\n';
    }
    return 0;
}
