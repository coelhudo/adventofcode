#include <iostream>
#include <fstream>
#include <cassert>
#include <regex>
#include <unordered_map>
#include <unordered_set>

struct Food
{
    std::unordered_set<int> ingredients_code;
    std::unordered_set<std::string> allergens;
};

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};
    std::string line;
    std::regex pattern{R"(([\w+]+))"};

    std::unordered_map<std::string, int> ingredients_dictionary; //just to make it easy to visualize
    int count{};

    std::vector<Food> foods;

    while(std::getline(ifs, line))
    {
        auto current = std::sregex_iterator{line.begin(), line.end(), pattern};
        auto end = std::sregex_iterator{};
        bool allergen = false;
        std::unordered_set<int> ingredients_code;
        std::unordered_set<std::string> allergens;
        for(;current != end; ++current)
        {
            std::smatch match = *current;
            if(match[0] == "contains")
            {
                allergen = true;
                continue;
            }

            if(!allergen)
            {
                auto it = ingredients_dictionary.find(match[0]);
                if(it == ingredients_dictionary.end())
                    ingredients_dictionary[match[0]] = ++count;
                ingredients_code.insert(ingredients_dictionary[match[0]]);
            }
            else
            {
                allergens.insert(match[0]);
            }
        }

        foods.push_back({ingredients_code, allergens});
    }

    for(auto food: foods)
    {
        auto [ingredient, allergens] = food;
        std::cout << ingredient.size() << " " << allergens.size() << '\n';
    }

    return 0;
}
