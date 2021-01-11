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
        std::get<1>(pos) += 1;
        if(std::get<0>(pos) % 2 == 1 && std::get<1>(pos) == 0)
            std::get<1>(pos) += 1;
    }

    void move_west()
    {
        std::get<1>(pos) -= 1;
        if(std::get<0>(pos) % 2 == 1 && std::get<1>(pos) == 0)
            std::get<1>(pos) -= 1;
    }

    void move_southeast()
    {
        std::get<0>(pos) -= 1;
        if((!even_line() && positive_column()) || (even_line() && !positive_column()) || std::get<1>(pos) == 0)
            std::get<1>(pos) += 1;
    }

    void move_southwest()
    {
        std::get<0>(pos) -= 1;
        if((even_line() && positive_column()) || (!even_line() && !positive_column()) || std::get<1>(pos) == 0)
            std::get<1>(pos) -= 1;
    }

    void move_northeast()
    {
        std::get<0>(pos) += 1;
        if((!even_line() && positive_column()) || (even_line() && !positive_column()) || std::get<1>(pos) == 0)
            std::get<1>(pos) += 1;
    }

    void move_northwest()
    {
        std::get<0>(pos) += 1;
        if((even_line() && positive_column()) || (!even_line() && !positive_column()) || std::get<1>(pos) == 0)
            std::get<1>(pos) -= 1;
    }

    bool even_line() const
    {
        return std::get<0>(pos) % 2 == 0;
    }

    bool positive_column() const
    {
        return std::get<1>(pos) > 0;
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
    // std::stringstream ifs;
    // ifs << "sesenwnenenewseeswwswswwnenewsewsw";
    std::ifstream ifs{argv[1]};
    std::string line;
    while(ifs >> line)
    {
        // std::cout << line << '\n';
        auto current_direction = line.begin();
        auto current_tile = TilePosition{};

        while(current_direction != line.end())
        {
            // std::cout << current_tile << '\n';
            if(*current_direction == 'e' || *current_direction == 'w')
            {
                // std::cout << "move to " << *current_direction << '\n';
                if(*current_direction == 'e')
                    current_tile.move_east();
                else
                    current_tile.move_west();
                ++current_direction;
            }
            else
            {
                auto next = std::next(current_direction);
                // std::cout << "move to " << *current_direction << *next << '\n';
                if(*current_direction == 's' && *next == 'e')
                    current_tile.move_southeast();
                else if(*current_direction == 's' && *next == 'w')
                    current_tile.move_southwest();
                else if(*current_direction == 'n' && *next == 'e')
                    current_tile.move_northeast();
                else if(*current_direction == 'n' && *next == 'w')
                    current_tile.move_northwest();
                std::advance(current_direction, 2);
            }
        }

        // std::cout << "turning " << current_tile << '\n';

        auto pos = turned_tiles.find(current_tile);
        if (pos != std::end(turned_tiles))
        {
            // std::cout << "removing "<< *pos << '\n';
            turned_tiles.erase(pos);
        }
        else
        {
            // std::cout << "inserting " << *pos << '\n';
            turned_tiles.insert(current_tile);
        }
    }

    std::cout << "Part 1: " << turned_tiles.size() << '\n';

    return 0;
}
