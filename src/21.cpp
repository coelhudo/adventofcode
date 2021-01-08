#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
#include <regex>
#include <iterator>
#include <unordered_map>
#include <map>
#include <set>

struct Food
{
    std::set<int> ingredients_code;
    std::set<std::string> allergens;
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
    std::set<std::string> all_allergens;

    while(std::getline(ifs, line))
    {
        auto current = std::sregex_iterator{line.begin(), line.end(), pattern};
        auto end = std::sregex_iterator{};
        bool allergen = false;
        std::set<int> ingredients_code;
        std::set<std::string> allergens;
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
                all_allergens.insert(match[0]);
            }
        }
        foods.push_back({ingredients_code, allergens});
    }

    //create a map containing the allergen as key and a set of all
    //codes where the allergen is present
    std::map<std::string, std::set<int>> single_allergen;
    for(auto current_allergen : all_allergens)
    {
        std::set<int> current_allergen_ingredients;
        for(auto [ingredients, allergens] : foods)
        {

            if(allergens.find(current_allergen) == allergens.end())
                continue;

            if(current_allergen_ingredients.empty())
            {
                current_allergen_ingredients = ingredients;
            }
            else
            {
                std::set<int> ingredients_intersection;
                std::set_intersection(current_allergen_ingredients.begin(), current_allergen_ingredients.end(),
                                      ingredients.begin(), ingredients.end(),
                                      std::inserter(ingredients_intersection, ingredients_intersection.begin()));
                current_allergen_ingredients = ingredients_intersection;
            }
        }
        single_allergen[current_allergen] = current_allergen_ingredients;
    }

    //found which ingredients has only a single occurrence, remove
    //them from the other allergen set of ingredients. keep doing that
    //until all allergens have only one ingredient associated with it.
    while(!std::all_of(single_allergen.begin(),
                       single_allergen.end(),
                       [](auto entry){ return entry.second.size() == 1;}))
    {
        //first allergen with only a single ingredient
        auto it = std::find_if(single_allergen.begin(), single_allergen.end(), [](auto entry){
            return entry.second.size() == 1;
        });

        while(it != single_allergen.end())
        {
            //remove this ingredient from other allergens if present
            std::for_each(single_allergen.begin(), single_allergen.end(), [&it](auto &entry)
            {
                if(entry.second.size() == 1)
                    return;

                auto single_ingredient = *it->second.begin();
                auto pos = entry.second.find(single_ingredient);
                if(pos != entry.second.end())
                {
                    entry.second.erase(pos);
                }
            });

            //find the next allergen with a single ingredient associated with it
            it = std::find_if(std::next(it), single_allergen.end(), [](auto entry){
                return entry.second.size() == 1;
            });
        }
    }

    //count number of times when this element appears
    int allergen_ingredients_counter{};
    int total_ingredients{};
    for(auto [ingredients, _] : foods)
    {
        for(auto [allergen, ingredient_set] : single_allergen)
        {
            auto ingredient = *ingredient_set.begin();
            allergen_ingredients_counter += std::count(ingredients.begin(), ingredients.end(), ingredient);
        }
        total_ingredients += ingredients.size();
    }

    std::cout << allergen_ingredients_counter << '\n';
    std::cout << total_ingredients << '\n';
    std::cout << "Part 1: " << total_ingredients - allergen_ingredients_counter << '\n';

    return 0;
}
