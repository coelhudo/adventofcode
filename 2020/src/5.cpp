#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
#include <bitset>
#include <set>


//the entry is nothing more than binary encoded: FBBBFFFLRL would be
//0111000010 where the first 7 digits is the row number in base 10,
//and the last 3 digits is the column in base 10.

//using set makes the insert log n. unordered_set would have been O(1)

//O(number of entries encoded * size of the seat encoding) + O(number of entries decoded)
int main(int argc, char *argv[])
{
    assert(("expected input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::string current_seat;
    unsigned long max_id = 0;
    std::set<unsigned long> seats;

    while(ifs >> current_seat)
    {
        std::string rows{current_seat.substr(0, 7)};
        std::string columns{current_seat.substr(7, 3)};
        //O(1) - 7 characteres
        std::for_each(std::begin(rows), std::end(rows), [](char &c)
        {
            if(c == 'F')
                c = '0';
            if(c == 'B')
                c = '1';
        });

        //O(1) - 3 characteres
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
        seats.insert(seat_id);
    }

    std::cout << "max id " << max_id << '\n';
    auto last = std::prev(std::end(seats));

    //O(number of seats)
    for(auto i = std::begin(seats); i != last; ++i)
    {
        auto n = std::next(i);
        if(*n - *i != 1) {
            std::cout << "my seat id is " << *i + 1<< '\n';
            break;
        }
    }


    return 0;
}
