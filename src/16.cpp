#include <iostream>
#include <fstream>
#include <cassert>
#include <regex>
#include <iterator>
#include <set>
#include <algorithm>
#include <sstream>
#include <numeric>

enum class State{RULES, MY, OTHERS};

struct Bucket {
    explicit Bucket(std::size_t _min, std::size_t _max) : min{_min}, max{_max}
    {
        subintervals.insert(min);
        subintervals.insert(max);
    }

    std::size_t min;
    std::size_t max;
    std::set<std::size_t> subintervals;
};

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    State current_state = State::RULES;

    std::vector<Bucket> intervals_buckets;
    auto insert_in_interval_bucket = [&intervals_buckets](std::size_t a, std::size_t b) {
        auto it = std::find_if(std::begin(intervals_buckets), std::end(intervals_buckets), [=](Bucket &bucket)
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

    std::vector<std::vector<std::size_t>> other_tickets;

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
                    std::regex pattern{R"([\w ]+: (\d+)-(\d+) or (\d+)-(\d+))"}; //assuming there is only one or
                    std::smatch match;
                    std::regex_match(line, match, pattern);
                    insert_in_interval_bucket(std::stoll(match[1]), std::stoll(match[2]));
                    insert_in_interval_bucket(std::stoll(match[3]), std::stoll(match[4]));
                }
                break;
            case State::MY:
                {
                    //I think my ticket doesn't matter now
                    //std::cout << "My " << line << '\n';
                }
                break;
            case State::OTHERS:
                {
                    //std::cout << "Others " << line << '\n';
                    std::istringstream iss{line};
                    std::vector<std::size_t> fields;
                    std::string field;
                    while(std::getline(iss, field, ',')){
                        fields.push_back(std::stoll(field));
                    }
                    other_tickets.push_back(fields);
                }
                break;
            }
        }
    }

    //creates a vector that can navigate every two elements to check the interval
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

    for(auto ticket : other_tickets)
    {
        //std::cout << "ticket\n";
        for(auto field: ticket) {
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

            if(!found) {
                fault_digits.push_back(field);
            }
        }
    }

    for(auto fault_digit: fault_digits)
        std::cout << fault_digit << ' ';
    std::cout << '\n';
    std::cout << "result " << std::accumulate(std::cbegin(fault_digits), std::cend(fault_digits), 0) << '\n';

    return 0;
}
