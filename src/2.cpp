#include <iostream>
#include <fstream>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <sstream>
#include <ios>


int main(int argc, char *argv[])
{
    assert(("expected input", argc == 2));

    using password_entry_t = std::tuple<int, int, char, std::string>;

    std::vector<password_entry_t> passwords_list;

    std::ifstream ifs{argv[1]};

    while(!ifs.eof())
    {
        std::string str;
        if(std::getline(ifs, str))
        {
            //"remove" special characters
            auto sep = str.find('-');
            str[sep] = ' ';
            auto colon = str.find(':');
            str[colon] = ' ';

            std::istringstream isbuf{str};
            int left_bound, right_bound;
            char valid_char;
            std::string password;
            isbuf >> left_bound >> right_bound >> valid_char >> password;
            passwords_list.emplace_back(left_bound, right_bound, valid_char, password);
        }
    }

    auto is_valid_policy_1 = [](password_entry_t const& password_entry)
    {
        int left_bound, right_bound;
        char valid_char;
        std::string password;
        std::tie(left_bound, right_bound, valid_char, password) = password_entry;

        auto n_valid_char = std::count(std::begin(password), std::end(password), valid_char);
        bool should_could = left_bound <= n_valid_char && n_valid_char <= right_bound;
        return should_could;
    };

    auto valid_passwords = std::count_if(std::begin(passwords_list), std::end(passwords_list), is_valid_policy_1);

    std::cout << "valid passwords according to policy 1: " << valid_passwords << '\n';

    auto is_valid_policy_2 = [](password_entry_t const& password_entry)
    {
        int pos1, pos2;
        char valid_char;
        std::string password;
        std::tie(pos1, pos2, valid_char, password) = password_entry;

        bool contains_valid1 = password[pos1-1] == valid_char;
        bool contains_valid2 = password[pos2-1] == valid_char;

        if(contains_valid1 && contains_valid2)
            return false;

        return contains_valid1 || contains_valid2;
    };

    valid_passwords = std::count_if(std::begin(passwords_list), std::end(passwords_list), is_valid_policy_2);

    std::cout << "valid passwords according to policy 2: " << valid_passwords << '\n';

    return 0;
}
