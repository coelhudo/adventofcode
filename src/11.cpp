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

    int number_adjacent_seats_occupied(std::size_t r, std::size_t c) const {
        int row = static_cast<int>(r);
        int column = static_cast<int>(c);
        int occupied_seats{};
        for(int i = row - 1; i <= row + 1; ++i) {
            for(int j = column - 1; j <= column + 1; ++j) {
                auto seat = this->operator()(i, j);
                if(seat != Seats::invalid_char && seat == '#' && !(i == row && j == column) ) {
                    ++occupied_seats;
                }
            }
        }
        return occupied_seats;
    };


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
                    int occupied_seats = seats.number_adjacent_seats_occupied(i, j);
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

            seats = Seats(assigned_seats, rows, columns);
        }

        std::cout << seats.occupied() << '\n';
    }

    //part b

    // auto first_seat = [&](std::size_t r, std::size_t c){
    //     int row = static_cast<int>(r);
    //     int column = static_cast<int>(c);
    //     const std::array<std::pair<int,int>, 8> initial_positions = {
    //         std::make_pair(-1,-1), {0, -1}, {1,-1},
    //         {-1,0}, {1,0}, {-1,1}, {0,1}, {1,1}
    //     };

    //     int min_pos_index = 9;
    //     std::pair<int, int> min_pos{};

    //     for(auto initial_pos: initial_positions) {
    //         for(int i = 1; i <= 8; ++i) {
    //             auto cr = row + initial_pos.first * i;
    //             auto cc = column + initial_pos.second * i;
    //             auto [valid, seat] = seat_at(cr, cc);
    //             if(valid && seat == 'L' && i < min_pos_index) {
    //                 min_pos_index = i;
    //                 min_pos = initial_pos;
    //             }
    //         }
    //     }

    //     return min_pos;
    // };



    //part b
    // {
    //     std::vector<char> assigned_seats;
    //     bool changed = true;
    //     //O(number of times until equilibria * n) where n is the input value;
    //     while(changed) {

    //         changed = false;
    //         for(int i = 0; i < rows; ++i) {
    //             for(int j = 0; j < columns; ++j) {
    //                 auto [seat, valid] = seat_at(i, j);
    //                 if(seat == 'L')
    //                 {
    //                     auto [r, c] = first_seat(i, j);
    //                     if(!(r == 0 && c == 0)) {
    //                         changed = true;
    //                         assigned_seats.push_back('#');
    //                         continue;
    //                     }
    //                 }
    //                 else if(seat == '#')
    //                 {
    //                     int occupied_seats = number_adjacent_seats_occupied(i, j);
    //                     if(occupied_seats >= 5) {
    //                         changed = true;
    //                         assigned_seats.push_back('L');
    //                         continue;
    //                     }
    //                 }

    //                 assigned_seats.push_back(seat);
    //             }
    //         }

    //         seats = assigned_seats;
    //         assigned_seats.clear();
    //     }

    //     print_vector(seats);

    //     std::cout << std::count(std::cbegin(seats), std::cend(seats), '#') << '\n';
    // }

    return 0;
}
