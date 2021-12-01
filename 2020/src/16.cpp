#include <bits/c++config.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <regex>
#include <iterator>
#include <set>
#include <list>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <array>
#include <string>
#include <bitset>

enum class State{RULES, MY, OTHERS};

//This code doesn't work with more than two possible intervals per field type
//Since it isn't part ofthe requirement, I will leave like this

struct IntervalBucket {
    explicit IntervalBucket(std::size_t _min, std::size_t _max) : min{_min}, max{_max}
    {
        subintervals.insert(min);
        subintervals.insert(max);
    }

    std::size_t min;
    std::size_t max;
    std::set<std::size_t> subintervals;
};

//The idea is to get all interval and create all possible valid intervals
//For example:
//1-3 or 5-11
//2-3 or 6-8
//This would result in interval bucket
//1-3: 1-2-3
//5-11: 5-8-11.
//Which in turn, will result in the all possible subintervals
//1-2 2-3 5-6 6-8 8-11
//Now I can search each field and check if it is between any of the subintervals

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    State current_state = State::RULES;

    std::vector<IntervalBucket> intervals_buckets;
    auto insert_in_interval_bucket = [&intervals_buckets](std::size_t a, std::size_t b) {
        auto it = std::find_if(std::begin(intervals_buckets), std::end(intervals_buckets), [=](IntervalBucket &bucket)
        {
            return !(b < bucket.min || a > bucket.max);
        });
        if(it != std::end(intervals_buckets)) {
            it->subintervals.insert(a);
            it->subintervals.insert(b);
        } else {
            intervals_buckets.emplace_back(a, b);
        }
    };

    std::list<std::vector<std::size_t>> other_tickets;

    std::vector<std::size_t> my_ticket;
    std::map<std::string, std::array<std::size_t, 4>> field_intervals;

    std::string line;
    while(std::getline(ifs, line))
    {
        if(line.empty()) {
            std::getline(ifs, line); //discard your ticket / nearby tickets
            if(current_state == State::RULES)
                current_state = State::MY;
            else if(current_state == State::MY)
                current_state = State::OTHERS;
        } else {
            switch(current_state)
            {
            case State::RULES:
                {
                    //std::cout << "Rules " << line << '\n';
                    std::regex pattern{R"(([\w ]+): (\d+)-(\d+) or (\d+)-(\d+))"}; //assuming there is only one or
                    std::smatch match;
                    std::regex_match(line, match, pattern);
                    //Part A
                    insert_in_interval_bucket(std::stoul(match[2]), std::stoul(match[3]));
                    insert_in_interval_bucket(std::stoul(match[4]), std::stoul(match[5]));
                    //Part B
                    field_intervals[match[1].str()] = std::array<std::size_t,4>{std::stoul(match[2]), std::stoul(match[3]),
                                                                                std::stoul(match[4]), std::stoul(match[5])};
                }
                break;
            case State::MY:
                {
                    std::istringstream iss{line};
                    std::string field;
                    while(std::getline(iss, field, ',')){
                        my_ticket.push_back(std::stoul(field));
                    }
                }
                break;
            case State::OTHERS:
                {
                    std::istringstream iss{line};
                    std::vector<std::size_t> fields;
                    std::string field;
                    while(std::getline(iss, field, ',')){
                        fields.push_back(std::stoul(field));
                    }
                    other_tickets.push_back(fields);
                }
                break;
            }
        }
    }

    //creates a vector where every two elements represents an interval
    //for example: {1,3,5,6,6,8} contains 3 intervals
    std::vector<std::size_t> intervals;
    for(auto b: intervals_buckets) {
        auto begin = b.subintervals.cbegin();
        auto last = --(b.subintervals.cend());
        intervals.push_back(*begin++);
        while(begin != last) {
            intervals.push_back(*begin);
            intervals.push_back(*begin++);
        }
        intervals.push_back(*last);
    }

    //Part A
    std::vector<std::size_t> fault_digits;

    for(auto ticket = other_tickets.cbegin(); ticket != other_tickets.cend();)
    {
        bool valid = true;
        for(auto field: *ticket) {
            auto first = std::cbegin(intervals);
            auto second = std::next(first);
            bool found = false;
            while(first != std::cend(intervals)) {
                if((found = *first <= field && field <= *second)) {
                    break;
                }
                first = std::next(second);
                second = std::next(first);
            }

            valid &= found;
            if(!found) {
                fault_digits.push_back(field);
            }
        }

        //remove invalid tickets. This code is part of Part B
        if(!valid) {
            ticket = other_tickets.erase(ticket);
        } else {
            ++ticket;
        }
    }

    std::cout << "Part A\nFault digits\n";
    for(auto fault_digit: fault_digits)
        std::cout << fault_digit << ' ';
    std::cout << '\n';
    std::cout << "result " << std::accumulate(std::cbegin(fault_digits), std::cend(fault_digits), 0) << '\n';

    std::cout << "Part B\nMy tickets\n";
    for(auto field: my_ticket)
        std::cout << field << ' ';
    std::cout << '\n';

    std::vector<std::bitset<1000>> bitmap;
    for(auto ticket: other_tickets) {
        std::ostringstream bits;
        for(auto field: ticket) {
            for(auto [field_name, intervals]: field_intervals) {
                bool first = intervals[0] <= field && field <= intervals[1];
                bool second = intervals[2] <= field && field <= intervals[3];
                if(first || second) {
                    bits << "1";
                } else {
                    bits << "0";
                }
            }
        }
        bitmap.emplace_back(bits.str());
    }

    //1. create a single bitset containing all results every "number of fields bits"
    //         Value 1 | Value 2 | Value 3
    //Ticket1: 011       111       111
    //Ticket2: 110       111       111
    //Ticket3: 111       110       111
    //
    //So, Ticket 1, value 1 can be field 2 and 3. Ticket 2 Value 3 can
    //be all fields, and so on.
    //This result in the following bitset
    //010 110 111
    std::bitset<1000> result{};
    result.set();
    for(auto bits: bitmap) {
        result &= bits;
    }

    //2. create a mask to extract all values associated with all fields
    //This will separate the values
    //So, the vector will contain the values {111, 110, 010}
    std::vector<std::bitset<1000>> possible_fields;
    std::bitset<1000> mask{};
    for(int i = 0; i < field_intervals.size(); ++i)
    {
        mask.set(i);
    }

    for(int i = 0; i < field_intervals.size(); ++i)
    {
        auto r = result & mask;
        r >>= (field_intervals.size() * i);

        possible_fields.push_back(r);
        mask <<= field_intervals.size();
    }

    //Need to reverse due to the way the data was manipulated above: from left to right
    //So, the vector now will contain the values {010, 110, 111}
    std::reverse(std::begin(possible_fields), std::end(possible_fields));

    auto only_relevant_bits = [&](auto const &bits) {
        auto str = bits.to_string();
        return str.substr(str.size() - field_intervals.size());
    };

    //The value with just one bit as 1 shows what is the correct
    //field. So, we can 0 all the other candidates in this
    //possible. We do this by flipping the bitset and masking with all
    //the values
    std::vector<std::string> fields_correct_order(possible_fields.size());

    //The field
    auto times = possible_fields.size();
    while (times--) {
        auto it = std::find_if(std::begin(possible_fields), std::end(possible_fields), [](auto const& possible_field){
            return possible_field.count() == 1;
        });

        //find where the single bit is 1 and put in the correct possition
        auto index = std::distance(std::begin(possible_fields), it);
        fields_correct_order[index] = only_relevant_bits(*it);

        auto current_mask = *it;
        current_mask.flip(); //value inversion for subsequent opration
        it->reset();//make it all zeros since we already know which field was represented by this bit

        //Since we know the field name for the bit at position index, we can make it zero in all other candidates
        std::transform(std::begin(possible_fields), std::end(possible_fields),
                       std::begin(possible_fields), [&current_mask](auto const& possible_field) {
                           return possible_field & current_mask;
                       });
    }

    std::size_t all_departures_multiplied{1};
    std::vector<std::string> fields_correct;
    for(int i = 0; i < field_intervals.size(); ++i){
        auto current = fields_correct_order.at(i);
        auto pos = current.find("1");
        auto field_it = std::begin(field_intervals);
        std::advance(field_it, pos);
        auto field_name = field_it->first;
        fields_correct.push_back(field_name);

        //all fields contains the departure word as the first word, so
        //don't need to worry about fields that doesn't start with
        //departure since the requirement is only about fields that
        //starts with it
        if(field_name.find("departure") != std::string::npos) {
            all_departures_multiplied *= my_ticket.at(i);
        }
    }

    std::cout << all_departures_multiplied << "\n";

    return 0;
}
