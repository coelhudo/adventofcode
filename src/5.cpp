#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>


//the entry is nothing more than binary encoded
int main(int argc, char *argv[])
{
    assert(("expected input", argc == 2));

    std::vector<std::string> seating;

    std::ifstream ifs{argv[1]};

    std::string current_seat;
    unsigned long max_id = 0;
    while(ifs >> current_seat)
    {
        std::string rows{current_seat.substr(0, 7)};
        std::string columns{current_seat.substr(7, 3)};
        std::for_each(std::begin(rows), std::end(rows), [](char &c)
        {
            if(c == 'F')
                c = '0';
            if(c == 'B')
                c = '1';
        });

        std::for_each(std::begin(columns), std::end(columns), [](char &c)
        {
            if(c == 'R')
                c = '1';
            if(c == 'L')
                c = '0';
        });

        std::bitset<7> r{rows};
        std::bitset<3> c{columns};
        auto row_number = r.to_ulong();
        auto column_number = c.to_ulong();
        auto seat_id = row_number * 8 + column_number;
        //std::cout << current_seat << ": row " << row_number << ", column " << column_number << ", seat ID " << seat_id << '\n';
        max_id = std::max(seat_id, max_id);
    }

    std::cout << "max id " << max_id << '\n';

    return 0;
}
