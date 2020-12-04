#include <iostream>
#include <cassert>
#include <bitset>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <algorithm>

//assumptions:
// - there are no spaces between the field name and the field value
// - all fields per passport are unique

struct SimplePassport
{
    SimplePassport(std::string const& passport_fields) : _passport_fields{passport_fields}
    {

    }

    bool is_valid() const {
        //std::cend(passport_fields) - 1: to ignore the last space that were included during the parsing phase
        auto n_field_separators = std::count(std::cbegin(_passport_fields), std::cend(_passport_fields) - 1, ' ');
        if(n_field_separators == 7)
        {
            std::cout << "Passport Valid -> " << _passport_fields << '\n';
            return true;
        } else if(n_field_separators == 6 && _passport_fields.find("cid:") == std::string::npos) //since cid is optional
        {
            std::cout << "Passport Valid -> " << _passport_fields << '\n';
            return true;
        }

        std::cout << "Passport Invalid -> " << _passport_fields << '\n';
        return false;
    }

    const std::string _passport_fields;
};



int main(int argc, char *argv[])
{
    assert(("expected input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::string line;
    std::ostringstream oss;
    int count_valid_passports{0};
    while(std::getline(ifs, line))
    {
        if(line.empty()){
            auto passport = SimplePassport(oss.str());
            if(passport.is_valid())
            {
                ++count_valid_passports;
            }
            oss.str("");
        }
        else {
            std::string current_field;
            std::istringstream iss{line};
            while(iss >> current_field)
            {
                oss << current_field << ' ';
            }
        }
    }

    //test last passport
    auto passport = SimplePassport(oss.str());
    if(passport.is_valid())
    {
        ++count_valid_passports;
    }

    std::cout << "Number of valid passports " << count_valid_passports << '\n';

    return 0;
}
