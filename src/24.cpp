#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
#include <sstream>
#include <tuple>

struct TilePosition;
std::ostream& operator<<(std::ostream& os, const TilePosition& c);

struct TilePosition
{
    bool operator<(TilePosition const& other) const
    {
        return pos < other.pos;
    }

    bool operator==(TilePosition const& other) const
    {
        return pos == other.pos;
    }

    void move_east()
    {
        std::get<0>(pos) += 1;
    }

    void move_west()
    {
        std::get<0>(pos) -= 1;
    }

    void move_southeast()
    {
        std::get<1>(pos) += 1;
    }

    void move_southwest()
    {
        std::get<0>(pos) -= 1;
        std::get<1>(pos) += 1;
    }

    void move_northeast()
    {
        std::get<0>(pos) += 1;
        std::get<1>(pos) -= 1;
    }

    void move_northwest()
    {
        std::get<1>(pos) -= 1;
    }

    std::tuple<int,int> pos{};
};

std::ostream& operator<<(std::ostream& os, const TilePosition& c)
{
    os << std::get<0>(c.pos) << " " << std::get<1>(c.pos);

    return os;
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::set<TilePosition> turned_tiles;
    std::ifstream ifs{argv[1]};
    std::string line;
    while(ifs >> line)
    {
        auto current_tile = TilePosition{};
        for(auto current_direction = line.begin(); current_direction != line.end(); ++current_direction)
        {
            if(*current_direction == 'e')
                current_tile.move_east();
            else if(*current_direction == 'w')
                current_tile.move_west();
            else
            {
                auto current = *current_direction++;
                if(current == 's' && *current_direction == 'e')
                    current_tile.move_southeast();
                else if(current == 's' && *current_direction == 'w')
                    current_tile.move_southwest();
                else if(current == 'n' && *current_direction == 'e')
                    current_tile.move_northeast();
                else if(current == 'n' && *current_direction == 'w')
                    current_tile.move_northwest();
            }

        }

        auto pos = turned_tiles.find(current_tile);
        if (pos != std::end(turned_tiles))
            turned_tiles.erase(pos);
        else
            turned_tiles.insert(current_tile);
    }

    std::cout << "Part 1: " << turned_tiles.size() << '\n';

    return 0;
}
