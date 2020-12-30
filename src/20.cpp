#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>

struct Tile
{
    std::string name;
    std::string upside;
    std::string downside;
    std::string leftside;
    std::string rightside;
    std::string reverse_upside;
    std::string reverse_downside;
    std::string reverse_leftside;
    std::string reverse_rightside;

    explicit Tile(std::string const &name,
                  std::string const &upside,
                  std::string const &downside,
                  std::string const &leftside,
                  std::string const &rightside) : name{name},
                                                  upside{upside},
                                                  reverse_upside{upside},
                                                  downside{downside},
                                                  reverse_downside{downside},
                                                  leftside{leftside},
                                                  reverse_leftside{leftside},
                                                  rightside{rightside},
                                                  reverse_rightside{rightside}

    {
        std::reverse(reverse_upside.begin(), reverse_upside.end());
        std::reverse(reverse_downside.begin(), reverse_downside.end());
        std::reverse(reverse_leftside.begin(), reverse_leftside.end());
        std::reverse(reverse_rightside.begin(), reverse_rightside.end());
    }

    bool matches_any(std::string const& side) const {
        return side == upside || side == downside || side == leftside || side == rightside ||
            side == reverse_upside || side == reverse_downside || side == reverse_leftside || side == reverse_rightside;
    }
};

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::vector<Tile> tiles;

    std::ifstream ifs{argv[1]};
    std::string line;
    while(!ifs.eof())
    {
        std::getline(ifs, line);
        if(line.empty())
            continue;
        std::string tile_name{line};
        std::getline(ifs, line);
        std::string upside{line};
        std::string leftside(".", 10);
        std::string rightside(".", 10);
        leftside[0] = upside[0];
        rightside[0] = upside[9];
        for(int i = 1; i < 9; ++i)
        {
            std::getline(ifs, line);
            leftside[i] = line[0];
            rightside[i] = line[9];
        }
        std::getline(ifs, line);
        std::string downside{line};
        leftside[9] = downside[0];
        rightside[9] = downside[9];
        tiles.emplace_back(tile_name, upside, downside, leftside, rightside);
    }

    // for(auto tile: tiles)
    // {
    //     std::cout << tile.name << '\n';
    //     std::cout << tile.upside << '\n';
    //     std::cout << tile.downside << '\n';
    //     std::cout << tile.leftside << '\n';
    //     std::cout << tile.rightside << '\n';
    //     std::cout << "=============" << "\n";
    // }

    std::map<std::string, std::set<std::string>> counter;
    for(auto tile: tiles)
    {
        for(auto current_tile: tiles)
        {
            if(tile.name == current_tile.name)
                continue;

            if(current_tile.matches_any(tile.upside))
                counter[tile.name].insert(current_tile.name);
            else if(current_tile.matches_any(tile.downside))
                counter[tile.name].insert(current_tile.name);
            else if(current_tile.matches_any(tile.leftside))
                counter[tile.name].insert(current_tile.name);
            else if(current_tile.matches_any(tile.rightside))
                counter[tile.name].insert(current_tile.name);
        }
    }

    auto extract_id = [](std::string const& tile_name)
    {
        std::istringstream iss{tile_name};
        std::string number;
        iss >> number;
        iss >> number;
        return number.substr(0, number.size()-1);
    };

    std::size_t answer{1};
    for(auto [tile_name, count]: counter)
    {
        if(count.size() == 2)
        {
            // std::cout << extract_id(tile_name) << ": ";
            // for(auto side: count)
            //     std::cout << extract_id(side) << ' ';
            // std::cout << '\n';
            answer *= std::stoi(extract_id(tile_name));
        }
    }

    std::cout << "Part 1: " << answer << '\n';

    return 0;
}
