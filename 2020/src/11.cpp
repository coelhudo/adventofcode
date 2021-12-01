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
#include <array>
#include <functional>
#include <cmath>

class Seats
{
public:
    explicit Seats(std::vector<char> const& seats, std::size_t rows, std::size_t columns): _seats{seats}, _rows{rows}, _columns{columns}
    {}

    explicit Seats(Seats const& other): _seats{other._seats}, _rows{other._rows}, _columns{other._columns}
    {}

    Seats& operator=(Seats const& other) {
        if(this == &other)
            return *this;

        _seats = other._seats;
        _rows = other._rows;
        _columns = other._columns;
        return *this;
    }

    const char& operator()(std::size_t row, std::size_t column) const {
        //return an invalid char instead of throwing exception since
        //it will not be uncommon to receive invalid coordinates
        if(row >= _rows || column >= _columns)
            return Seats::invalid_char;

        return _seats.at(_columns * row + column);
    };

    int number_of_occupied_at_sight(std::size_t r, std::size_t c, bool adjacent) const {
        int row = static_cast<int>(r);
        int column = static_cast<int>(c);

        int occupied_at_sight{};

        for(auto base_pos : _base_positions) {
            int count = 1;
            auto seat = this->operator()(row + base_pos.first * count, column + base_pos.second * count);
            while(seat == '.' && !adjacent) {
                ++count;
                seat = this->operator()(row + base_pos.first * count, column + base_pos.second * count);
            }
            if(seat == '#')
                ++occupied_at_sight;
        }

        return occupied_at_sight;
    }

    void print() const
    {
        for(int i = 0; i < _rows; ++i) {
            for(int j = 0; j < _columns; ++j) {
                std::cout << this->operator()(i, j) << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    };

    auto occupied() const {
        return std::count(std::cbegin(_seats), std::cend(_seats), '#');
    }

    const static char invalid_char;

private:
    std::vector<char> _seats;
    std::size_t _rows;
    std::size_t _columns;
    const std::array<std::pair<int, int>, 8> _base_positions {
            std::make_pair(-1,-1), {-1, 0}, {-1, 1},
            {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };
};

const char Seats::invalid_char = 0;


int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::string line;
    ifs >> line;
    std::vector<char> seats_container{std::cbegin(line), std::cend(line)};
    const std::size_t columns = line.size();

    seats_container.insert(std::end(seats_container), std::istream_iterator<char>{ifs}, std::istream_iterator<char>{});

    const std::size_t rows = seats_container.size() / columns;

    //part a
    {
        bool changed = true;
        //O(number of times until equilibria * n) where n is the input value;
        Seats seats(seats_container, rows, columns);

        while(changed) {
            std::vector<char> assigned_seats;
            changed = false;
            for(int i = 0; i < rows; ++i) {
                for(int j = 0; j < columns; ++j) {
                    auto seat = seats(i, j);
                    int occupied_seats = seats.number_of_occupied_at_sight(i, j, true);
                    bool met_condition = false;
                    if((met_condition = (seat == 'L' && occupied_seats == 0)))
                        assigned_seats.push_back('#');
                    else if((met_condition = seat == '#' && occupied_seats >= 4))
                        assigned_seats.push_back('L');
                    else
                        assigned_seats.push_back(seat);
                    changed |= met_condition;
                }
            }

            // seats.print();

            seats = Seats(assigned_seats, rows, columns);
        }

        // seats.print();

        std::cout << "Part a " << seats.occupied() << '\n';
    }

    //part b
    {
        bool changed = true;
        //O(number of times until equilibria * n) where n is the input value;
        Seats seats(seats_container, rows, columns);

        while(changed) {
            std::vector<char> assigned_seats;
            changed = false;
            for(int i = 0; i < rows; ++i) {
                for(int j = 0; j < columns; ++j) {
                    auto seat = seats(i, j);
                    int occupied_seats_at_sight = seats.number_of_occupied_at_sight(i, j, false);
                    bool met_condition = false;
                    if((met_condition = (seat == 'L' && occupied_seats_at_sight == 0)))
                        assigned_seats.push_back('#');
                    else if((met_condition = seat == '#' && occupied_seats_at_sight >= 5))
                        assigned_seats.push_back('L');
                    else
                        assigned_seats.push_back(seat);
                    changed |= met_condition;
                }
            }

            // seats.print();

            seats = Seats(assigned_seats, rows, columns);
        }

        // seats.print();

        std::cout << "Part b " << seats.occupied() << '\n';
    }

    return 0;
}
