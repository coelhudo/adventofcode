#include <algorithm>
#include <iostream>
#include <fstream>
#include <cassert>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <sstream>
#include <functional>


struct TileCore
{
    explicit TileCore(int side_length, std::string const& core) : side_length{side_length},
                                                                  core{core}
    {
    }

    TileCore(TileCore const&) = default;
    TileCore& operator=(TileCore const& other) = default;

    char operator()(int row, int column) const
    {
        auto [new_row, new_column] = index(row, column);
        return core[new_row * side_length + new_column];
    }

    char& operator()(int row, int column)
    {
        auto [new_row, new_column] = index(row, column);
        return core[new_row * side_length + new_column];
    }

    std::pair<int, int> index(int row, int column) const
    {
        switch(base)
        {
        case 0:
            return {row, column};
        case 1:
            return {side_length - column - 1, row};
        case 2:
            return {side_length - row - 1, side_length - column - 1};
        case 3:
            return {column, side_length - row - 1};
        default:
            throw std::runtime_error("Not supposed to reach this");
        }
    }

    void rotate(int times)
    {
        base = (base + times) % 4;
    }

    void horizontal_flip()
    {
        TileCore copy{*this};
        for(int i = 0, icopy = 0; i < side_length; ++i, ++icopy)
        {
            for(int j = 0, jcopy = side_length - 1; j < side_length; ++j, --jcopy)
            {
                this->operator()(icopy, jcopy) = copy(i, j);
            }
        }
    }

    void vertical_flip()
    {
        TileCore copy{*this};
        for(int i = 0, icopy = side_length - 1; i < side_length; ++i, --icopy)
        {
            for(int j = 0, jcopy = 0; j < side_length; ++j, ++jcopy)
            {
                this->operator()(icopy, jcopy) = copy(i, j);
            }
        }
    }

    int base{0};
    std::string core;
    int side_length;
};

struct Tile
{
    explicit Tile(int id,
                  std::string const &upside,
                  std::string const &downside,
                  std::string const &leftside,
                  std::string const &rightside,
                  TileCore const& core) : id{id},
                                          sides{upside, rightside, downside, leftside},
                                          core{core}
    {
    }

    bool assign_matches(std::string const& side) const {
        return side == upside() || side == downside() || side == leftside() || side == rightside();
    }

    bool assign_matches_reversed(std::string side) const {
        std::reverse(side.begin(), side.end());
        return assign_matches(side);
    }

    bool operator<(Tile const&other) const
    {
        return id < other.id;
    }

    std::string const& upside() const
    {
        return sides.at(base % 4);
    }

    std::string const& rightside() const
    {
        return sides.at((base + 1) % 4);
    }

    std::string const& downside() const
    {
        return sides.at((base + 2) % 4);
    }

    std::string const& leftside() const
    {
        return sides.at((base + 3) % 4);
    }

    std::string & upside()
    {
        return sides.at(base % 4);
    }

    std::string & rightside()
    {
        return sides.at((base + 1) % 4);
    }

    std::string & downside()
    {
        return sides.at((base + 2) % 4);
    }

    std::string & leftside()
    {
        return sides.at((base + 3) % 4);
    }

    void rotate(int times)
    {
        base = (base + times) % 4;

        core.rotate(times);
    }

    void horizontal_flip()
    {
        std::swap(rightside(), leftside());
        auto &up = upside();
        std::reverse(up.begin(), up.end());
        auto &down = downside();
        std::reverse(down.begin(), down.end());

        core.horizontal_flip();
    }

    void vertical_flip()
    {
        std::swap(upside(), downside());
        auto &left = leftside();
        std::reverse(left.begin(), left.end());
        auto &right = rightside();
        std::reverse(right.begin(), right.end());

        core.vertical_flip();
    }

    int id;
    int base{0};
    std::array<std::string, 4> sides;
    TileCore core;
    std::set<Tile*> neighbours;
};

std::ostream &operator<<(std::ostream &os, Tile const& tile)
{
    os << "Id: " << tile.id << '\n';
    os << "upside: " << tile.upside() << '\n';
    os << "rightside: " << tile.rightside() << '\n';
    os << "downside: " << tile.downside() << '\n';
    os << "leftside: " << tile.leftside() << '\n';
    os << "core: \n";
    for(int i = 0; i < tile.core.side_length; ++i)
    {
        os << " ";
        for(int j = 0; j < tile.core.side_length; ++j)
        {
            os << tile.core(i, j) << ' ';
        }
        os << '\n';
    }

    return os;
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::vector<Tile> tiles;

    auto extract_id = [](std::string const& tile_name)
    {
        std::istringstream iss{tile_name};
        std::string number;
        iss >> number;
        iss >> number;
        return std::stoi(number.substr(0, number.size()-1));
    };

    std::ifstream ifs{argv[1]};
    std::string line;
    while(!ifs.eof())
    {
        std::getline(ifs, line);
        if(line.empty())
            continue;

        int tile_id{extract_id(line)};
        std::getline(ifs, line);
        std::string upside{line};
        std::string leftside(".", 10);
        std::string rightside(".", 10);
        leftside[0] = upside[0];
        rightside[0] = upside[9];
        std::ostringstream core;
        for(int i = 1; i < 9; ++i)
        {
            std::getline(ifs, line);
            core << line.substr(1,8);
            leftside[i] = line[0];
            rightside[i] = line[9];
        }
        std::getline(ifs, line);
        std::string downside{line};
        leftside[9] = downside[0];
        rightside[9] = downside[9];

        //order matters here
        tiles.emplace_back(tile_id, upside, rightside, downside, leftside, TileCore{8, core.str()});
    }

    // for(auto tile: tiles)
    // {
    //     std::cout << tile;
    // }

    for(auto &tile: tiles)
    {
        for(auto &current_tile: tiles)
        {
            if(tile.id == current_tile.id)
                continue;

            if(current_tile.assign_matches(tile.upside()) || current_tile.assign_matches(tile.downside()) ||
               current_tile.assign_matches(tile.leftside()) || current_tile.assign_matches(tile.rightside()))
            {
                tile.neighbours.insert(&current_tile);
            }
            else if(current_tile.assign_matches_reversed(tile.upside()) || current_tile.assign_matches_reversed(tile.downside()) ||
                    current_tile.assign_matches_reversed(tile.leftside()) || current_tile.assign_matches_reversed(tile.rightside()))
            {
                tile.neighbours.insert(&current_tile);
            }

        }
    }

    //First pass: find neighbouring tiles
    std::size_t answer{1};
    for(auto tile : tiles)
    {
        if(tile.neighbours.size() == 2)
            answer *= tile.id;
    }

    std::cout << "Part 1: " << answer << '\n';

    //Second pass: rotate to correct pos
    for(auto tile : tiles)
    {
        if(tile.neighbours.size() == 2)
        {
            // std::cout << "Tile =============\n";
            // std::cout << tile << "\n";
            // std::cout << "Neighbours =============\n";
            // for(auto side: tile.neighbours)
            //     std::cout << *side << '\n';
            // std::cout << "=============\n";
        }
    }

    std::cout << tiles.at(0) << '\n';
    // tiles.at(0).rotate(1);
    // std::cout << tiles.at(0) << '\n';
    // tiles.at(0).rotate(1);
    // std::cout << tiles.at(0) << '\n';
    // tiles.at(0).rotate(1);
    // std::cout << tiles.at(0) << '\n';
    // tiles.at(0).rotate(1);
    // std::cout << tiles.at(0) << '\n';

    tiles.at(0).horizontal_flip();
    std::cout << tiles.at(0) << '\n';

    return 0;
}
