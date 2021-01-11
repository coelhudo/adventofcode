#include <iostream>
#include <fstream>
#include <cassert>
#include <unordered_set>
#include <sstream>
#include <tuple>
#include <vector>

struct HexTile;
std::ostream& operator<<(std::ostream& os, const HexTile& c);

struct HexTile
{
    bool operator<(HexTile const& other) const
    {
        return pos < other.pos;
    }

    bool operator==(HexTile const& other) const
    {
        return pos == other.pos;
    }

    HexTile& move_east()
    {
        std::get<0>(pos) += 1;
        return *this;
    }

    HexTile& move_west()
    {
        std::get<0>(pos) -= 1;
        return *this;
    }

    HexTile& move_south_east()
    {
        std::get<1>(pos) += 1;
        return *this;
    }

    HexTile& move_south_west()
    {
        std::get<0>(pos) -= 1;
        std::get<1>(pos) += 1;
        return *this;
    }

    HexTile& move_north_east()
    {
        std::get<0>(pos) += 1;
        std::get<1>(pos) -= 1;
        return *this;
    }

    HexTile& move_north_west()
    {
        std::get<1>(pos) -= 1;
        return *this;
    }

    std::tuple<int,int> pos{};
};

std::ostream& operator<<(std::ostream& os, const HexTile& c)
{
    os << std::get<0>(c.pos) << " " << std::get<1>(c.pos);

    return os;
}

namespace std
{
    template<>
    struct hash<HexTile>
    {
        std::size_t operator()(HexTile const& tile) const
        {
            auto seed = std::hash<int>()(std::get<0>(tile.pos)) + 0x9e3779b9 + (2<<6) + (2>>2);
            seed ^= std::hash<int>()(std::get<0>(tile.pos)) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            return seed;
        }
    };
}

int number_of_adjacent_turned_tiles(HexTile const& tile, std::unordered_set<HexTile> const& turned_tiles)
{
    int adjacent_turned_tiles = 0;

    auto count = [&adjacent_turned_tiles, &turned_tiles](HexTile const& tile)
    {
        if(turned_tiles.find(tile) != turned_tiles.cend())
            ++adjacent_turned_tiles;
    };

    count(HexTile(tile).move_east());
    count(HexTile(tile).move_west());
    count(HexTile(tile).move_south_east());
    count(HexTile(tile).move_south_west());
    count(HexTile(tile).move_north_east());
    count(HexTile(tile).move_north_west());

    return adjacent_turned_tiles;
}

HexTile parse_input(std::string const& input)
{
    auto current_tile = HexTile{};
    for(auto current_direction = input.begin(); current_direction != input.end(); ++current_direction)
    {
        if(*current_direction == 'e')
            current_tile.move_east();
        else if(*current_direction == 'w')
            current_tile.move_west();
        else
        {
            auto current = *current_direction++;
            if(current == 's' && *current_direction == 'e')
                current_tile.move_south_east();
            else if(current == 's' && *current_direction == 'w')
                current_tile.move_south_west();
            else if(current == 'n' && *current_direction == 'e')
                current_tile.move_north_east();
            else if(current == 'n' && *current_direction == 'w')
                current_tile.move_north_west();
        }

    }
    return current_tile;
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};
    std::string line;
    std::vector<HexTile> tiles;
    while(ifs >> line)
    {
        auto current_tile = parse_input(line);
        tiles.push_back(current_tile);
    }

    //Part 1
    {
        std::unordered_set<HexTile> turned_tiles;
        for(auto tile : tiles)
        {
            auto pos = turned_tiles.find(tile);
            if (pos != std::end(turned_tiles))
                turned_tiles.erase(pos);
            else
                turned_tiles.insert(tile);
        }
        std::cout << "Part 1: " << turned_tiles.size() << '\n';
    }

    //Part 2
    {
        std::unordered_set<HexTile> turned_tiles;
        for(auto tile : tiles)
        {
            auto pos = turned_tiles.find(tile);
            if (pos != std::end(turned_tiles))
                turned_tiles.erase(pos);
            else
                turned_tiles.insert(tile);
        }

        for(int i = 0; i < 100; ++i)
        {
            std::unordered_set<HexTile> new_turned_tiles;
            std::unordered_set<HexTile> tiles_to_search;
            for(auto tile : turned_tiles)
            {
                tiles_to_search.insert(tile);
                tiles_to_search.insert(HexTile(tile).move_east());
                tiles_to_search.insert(HexTile(tile).move_west());
                tiles_to_search.insert(HexTile(tile).move_south_east());
                tiles_to_search.insert(HexTile(tile).move_south_west());
                tiles_to_search.insert(HexTile(tile).move_north_east());
                tiles_to_search.insert(HexTile(tile).move_north_west());
            }

            for(auto tile : tiles_to_search)
            {
                auto adjacent_black_tiles = number_of_adjacent_turned_tiles(tile, turned_tiles);
                //no need to keep searching if it doesn't match any criteria
                if(!(adjacent_black_tiles != 0 && adjacent_black_tiles <= 2))
                    continue;
                auto pos = turned_tiles.find(tile);
                if(pos != turned_tiles.end())
                {
                    if(adjacent_black_tiles != 0 && adjacent_black_tiles <= 2)
                        new_turned_tiles.insert(tile);
                }
                else
                {
                    if(adjacent_black_tiles == 2)
                        new_turned_tiles.insert(tile);
                }
            }
            // std::cout << "Day " << i + 1 << ": " << new_turned_tiles.size() << '\n';
            turned_tiles = new_turned_tiles;
        }

        std::cout << "Part 2: " << turned_tiles.size() << '\n';
    }

    return 0;
}
