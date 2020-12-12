#include <bits/c++config.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::string line;
    ifs >> line;
    std::vector<char> seats{std::cbegin(line), std::cend(line)};
    const std::size_t columns = line.size();

    seats.insert(std::end(seats), std::istream_iterator<char>{ifs}, std::istream_iterator<char>{});

    const std::size_t rows = seats.size() / columns;

    auto seat_at = [&](std::size_t row, std::size_t column) {
        if(row >= rows || column >= columns)
            return std::make_pair(static_cast<char>(0), false);

        return std::make_pair(seats.at(columns * row + column), true);
    };

    auto set_seat_at = [&](std::size_t row, std::size_t column, char value) {
        if(row > rows || column > columns)
            return;

        seats.at(columns * row + column) = value;
    };

    auto number_adjacent_seats_occupied = [&](std::size_t r, std::size_t c) {
        int row = static_cast<int>(r);
        int column = static_cast<int>(c);
        int occupied_seats{};
        for(int i = row - 1; i <= row + 1; ++i) {
            for(int j = column - 1; j <= column + 1; ++j) {
                auto [seat, valid_pos] = seat_at(i, j);
                if(valid_pos && seat == '#' && !(i == row && j == column) ) {
                    ++occupied_seats;
                }
            }
        }
        return occupied_seats;
    };


    auto print_vector = [rows, columns](std::vector<char> const& seats)
    {
        for(int i = 0; i < rows; ++i) {
            for(int j = 0; j < columns; ++j) {
                std::cout << seats.at(columns * i + j) << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    };

    //part a
    std::vector<char> assigned_seats;
    bool changed = true;
    //O(number of times until equilibria * n) where n is the input value;
    while(changed) {

        changed = false;
        for(int i = 0; i < rows; ++i) {
            for(int j = 0; j < columns; ++j) {
                auto [seat, valid] = seat_at(i, j);
                int occupied_seats = number_adjacent_seats_occupied(i, j);
                if(seat == 'L' && occupied_seats == 0)
                {
                    changed = true;
                    assigned_seats.push_back('#');
                }
                else if(seat == '#' && occupied_seats >= 4)
                {
                    changed = true;
                    assigned_seats.push_back('L');
                }
                else
                    assigned_seats.push_back(seat);
            }
        }

        seats = assigned_seats;
        assigned_seats.clear();
    }

    print_vector(seats);

    std::cout << std::count(std::cbegin(seats), std::cend(seats), '#') << '\n';

    return 0;
}
