#include <iostream>
#include <cassert>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>
#include <regex>

//assumptions:
// - there are no spaces between the field name and the field value
// - all fields per passport are unique
// - all years values are numbers (so I don't need to check for its
// size or if there are any funny character present)
// - if height contains cm it will have exactly 3 numbers; otherwise,
// if it contains in it will have exactly 2 numbers

struct SimplePassport
{
    explicit SimplePassport(std::string const& passport_fields) : _passport_fields{passport_fields}
    {}

    virtual bool is_valid() const {
        //std::cend(passport_fields) - 1: to ignore the last space that were included during the parsing phase
        auto n_field_separators = std::count(std::cbegin(_passport_fields), std::cend(_passport_fields) - 1, ' ');
        return (n_field_separators == 7) || // if there are 7 separators, that means 8 fields
               (n_field_separators == 6 && _passport_fields.find("cid:") == std::string::npos); //since cid is optional
    }

    const std::string _passport_fields;
};

struct ComplexPassport : public SimplePassport
{
    using SimplePassport::SimplePassport;

    bool is_valid() const override {
        if(!SimplePassport::is_valid())
            return false;

        //since all fields are here, we can parse and check them
        std::istringstream iss{_passport_fields};
        std::string entry;
        bool valid = true;
        while(iss >> entry)
        {
            std::string field{entry.substr(0, 3)};
            std::string value{entry.substr(4)};

            if(field == "byr")
            {
                int year = std::stoi(value);
                valid &= (1920 <= year && year <= 2002);
            }
            else if(field == "iyr")
            {
                int year = std::stoi(value);
                valid &= (2010 <= year && year <= 2020);
            }
            else if(field == "eyr")
            {
                int year = std::stoi(value);
                valid &= (2020 <= year && year <= 2030);
            }
            else if(field == "hgt")
            {
                if(value.find("cm") != std::string::npos)
                {
                    int height = std::stoi(value.substr(0, 3));
                    valid &= (150 <= height && height <= 193);
                } else if(value.find("in") != std::string::npos)
                {
                    int height = std::stoi(value.substr(0, 2));
                    valid &= (59 <= height && height <= 76);
                } else {
                    valid = false;
                    break; //no need to continue
                }
            }
            else if(field == "hcl")
            {
                std::smatch match;
                std::regex pattern{"#[[:digit:]a-f]{6}"};
                valid &= std::regex_match(value, match, pattern);
            }
            else if(field == "ecl")
            {
                const std::set<std::string> eye_color{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
                valid &= eye_color.find(value) != std::end(eye_color);
            }
            else if(field == "pid")
            {
                std::smatch match;
                std::regex pattern{"[[:digit:]]{9}"};
                valid &= std::regex_match(value, match, pattern);
            }
            else if(field == "cid")
            {
                //ignore
            }

        }

        //std::cout << "Passport -> " << _passport_fields << ". Valid: " << std::boolalpha << valid << '\n';
        return valid;
    }
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
            auto passport = ComplexPassport(oss.str());
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
    auto passport = ComplexPassport(oss.str());
    if(passport.is_valid())
    {
        ++count_valid_passports;
    }

    std::cout << "Number of valid passports " << count_valid_passports << '\n';

    return 0;
}
