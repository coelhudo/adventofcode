#include <algorithm>
#include <cstddef>
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
                  std::string const &rightside,
                  std::string const &downside,
                  std::string const &leftside,
                  TileCore const& core) : id{id},
                                          sides{upside, rightside, downside, leftside},
                                          core{core}
    {
    }

    bool matches(Tile & tile) {
        if(tile.upside() == upside() || tile.downside() == upside() || tile.leftside() == upside() || tile.rightside() == upside())
        {
            // std::cout << id << " -> " << tile.id << " | ";
            // std::cout << upside() << ": " << tile.upside() << ", " << tile.downside() << ", " << tile.leftside() << ", " << tile.rightside() << '\n';
            upside_neighbour(&tile);
        }
        else if(tile.upside() == downside() || tile.downside() == downside() || tile.leftside() == downside() || tile.rightside() == downside())
        {
            // std::cout << id << " -> " << tile.id << " | ";
            // std::cout << downside() << ": " << tile.upside() << ", " << tile.downside() << ", " << tile.leftside() << ", " << tile.rightside() << '\n';
            downside_neighbour(&tile);
        }
        else if(tile.upside() == leftside() || tile.downside() == leftside() || tile.leftside() == leftside() || tile.rightside() == leftside())
        {
            // std::cout << id << " -> " << tile.id << " | ";
            // std::cout << leftside() << ": " << tile.upside() << ", " << tile.downside() << ", " << tile.leftside() << ", " << tile.rightside() << '\n';
            leftside_neighbour(&tile);
        }
        else if(tile.upside() == rightside() || tile.downside() == rightside() || tile.leftside() == rightside() || tile.rightside() == rightside())
        {
            // std::cout << id << " -> " << tile.id << " | ";
            // std::cout << rightside() << ": " << tile.upside() << ", " << tile.downside() << ", " << tile.leftside() << ", " << tile.rightside() << '\n';
            rightside_neighbour(&tile);
        }
        else
        {
            return false;
        }

        return true;
    }

    bool matches_flipped(Tile & tile) {
        tile.horizontal_flip();
        if(matches(tile))
            return true;
        tile.vertical_flip();
        return matches(tile);
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

    Tile const* upside_neighbour() const
    {
        return neighbours.at(base % 4);
    }

    Tile const* rightside_neighbour() const
    {
        return neighbours.at((base + 1) % 4);
    }

    Tile const* downside_neighbour() const
    {
        return neighbours.at((base + 2) % 4);
    }

    Tile const* leftside_neighbour() const
    {
        return neighbours.at((base + 3) % 4);
    }

    Tile * upside_neighbour()
    {
        return neighbours.at(base % 4);
    }

    Tile * rightside_neighbour()
    {
        return neighbours.at((base + 1) % 4);
    }

    Tile * downside_neighbour()
    {
        return neighbours.at((base + 2) % 4);
    }

    Tile * leftside_neighbour()
    {
        return neighbours.at((base + 3) % 4);
    }

    void upside_neighbour(Tile *tile)
    {
        possible_neighbours.at(base % 4).push_back(tile);
        neighbours.at(base % 4) = tile;
    }

    void rightside_neighbour(Tile *tile)
    {
        possible_neighbours.at((base + 1) % 4).push_back(tile);
        neighbours.at((base + 1) % 4)= tile;
    }

    void downside_neighbour(Tile *tile)
    {
        possible_neighbours.at((base + 2) % 4).push_back(tile);
        neighbours.at((base + 2) % 4) = tile;
    }

    void leftside_neighbour(Tile *tile)
    {
        possible_neighbours.at((base + 3) % 4).push_back(tile);
        neighbours.at((base + 3) % 4) = tile;
    }

    void rotate(int times)
    {
        base = base + ((4 - times) % 4);
        core.rotate(times);
    }

    void horizontal_flip()
    {
        std::swap(rightside(), leftside());
        auto rn = rightside_neighbour();
        auto ln = leftside_neighbour();
        rightside_neighbour(ln);
        leftside_neighbour(rn);
        auto &up = upside();
        std::reverse(up.begin(), up.end());
        auto &down = downside();
        std::reverse(down.begin(), down.end());

        core.horizontal_flip();
    }

    void vertical_flip()
    {
        std::swap(upside(), downside());
        auto un = upside_neighbour();
        auto dn = downside_neighbour();
        upside_neighbour(dn);
        downside_neighbour(un);
        auto &left = leftside();
        std::reverse(left.begin(), left.end());
        auto &right = rightside();
        std::reverse(right.begin(), right.end());

        core.vertical_flip();
    }

    int n_neighbours() const
    {
        return std::count_if(neighbours.cbegin(), neighbours.cend(), [](Tile const* tile) { return tile != nullptr;});
    }

    int id;
    int base{0};
    std::array<std::string, 4> sides{};
    TileCore core;
    std::array<Tile*, 4> neighbours{};
    std::array<std::vector<Tile*>, 4> possible_neighbours{};
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

struct TileVisitor
{
    virtual ~TileVisitor() {}

    void visit(Tile *tile)
    {
        if(tile == nullptr || visited_tiles_id.find(tile->id) != visited_tiles_id.cend())
            return;

        process(tile);
        visited_tiles_id.insert(tile->id);

        visit(tile->upside_neighbour());
        visit(tile->rightside_neighbour());
        visit(tile->downside_neighbour());
        visit(tile->leftside_neighbour());
    }

    virtual void process(Tile *tile) const
    {
        std::cout << "processed " << tile->id << '\n';

        if(tile->upside_neighbour())
            std::cout << "  upside neighbour " << tile->upside_neighbour()->id << '\n';
        if(tile->rightside_neighbour())
            std::cout << "  rightside neighbour " << tile->rightside_neighbour()->id << '\n';
        if(tile->downside_neighbour())
            std::cout << "  downside neighbour " << tile->downside_neighbour()->id << '\n';
        if(tile->leftside_neighbour())
            std::cout << "  leftside neighbour " << tile->leftside_neighbour()->id << '\n';
    }

    std::set<int> visited_tiles_id;
};

struct FixTileVisitor: public TileVisitor
{
    void process(Tile *tile) const override
    {
        std::cout << tile->id << '\n';
        auto correct_upside_neighbour = [&tile]{ return tile->upside() == tile->upside_neighbour()->downside();};
        auto correct_rightside_neighbour = [&tile]{ return tile->rightside() == tile->rightside_neighbour()->leftside();};
        auto correct_downside_neighbour = [&tile]{ return tile->downside() == tile->downside_neighbour()->upside();};
        auto correct_leftside_neighbour = [&tile]{ return tile->leftside() == tile->leftside_neighbour()->rightside();};

        if(tile->upside_neighbour() && !correct_upside_neighbour())
        {
            auto upside_neighbour = [&tile]{ return tile->upside_neighbour();};
            std::cout << "fixing neighbours of " << tile->id << '\n';
            std::cout << "must fix upside neighbour\n";
            fix(upside_neighbour, correct_upside_neighbour);
            // if(!fix(upside_neighbour, neighbour_fixed))
            // {
            //     std::cout << "fixing neighbours of " << tile->id << '\n';
            //     std::cout << "must fix upside neighbour\n";
            //     std::cout << *tile << '\n';
            //     std::cout << *tile->upside_neighbour() << '\n';
            // }
        }

        if(tile->rightside_neighbour() && !correct_rightside_neighbour())
        {
            auto rightside_neighbour = [&tile]{ return tile->rightside_neighbour();};
            std::cout << "fixing neighbours of " << tile->id << '\n';
            std::cout << "must fix righside neighbour\n";
            fix(rightside_neighbour, correct_rightside_neighbour);
            // if(!fix(rightside_neighbour, neighbour_fixed))
            // {
            //     std::cout << "fixing neighbours of " << tile->id << '\n';
            //     std::cout << "must fix righside neighbour\n";
            //     std::cout << *tile << '\n';
            //     std::cout << *tile->rightside_neighbour() << '\n';
            // }
        }
        if(tile->downside_neighbour() && !correct_downside_neighbour())
        {
            auto downside_neighbour = [&tile]{ return tile->downside_neighbour();};
            std::cout << "fixing neighbours of " << tile->id << '\n';
            std::cout << "must fix downside neighbour\n";
            fix(downside_neighbour, correct_downside_neighbour);
            // if(!fix(downside_neighbour, neighbour_fixed))
            // {
            //     std::cout << "fixing neighbours of " << tile->id << '\n';
            //     std::cout << "must fix downside neighbour\n";
            //     std::cout << *tile << '\n';
            //     std::cout << *tile->downside_neighbour() << '\n';
            // }
        }

        if(tile->leftside_neighbour() && !correct_leftside_neighbour())
        {
            auto leftside_neighbour = [&tile]{ return tile->leftside_neighbour();};
            std::cout << "fixing neighbours of " << tile->id << '\n';
            std::cout << "must fix leftside neighbour\n";
            fix(leftside_neighbour, correct_leftside_neighbour);
            // {
            //     std::cout << "fixing neighbours of " << tile->id << '\n';
            //     std::cout << "must fix leftside neighbour\n";
            //     std::cout << *tile << '\n';
            //     std::cout << *tile->leftside_neighbour() << '\n';
            // }
        }
    }

    bool fix(std::function<Tile*()> neighbour, std::function<bool()> correct_position) const
    {
        Tile *current_neighbour = neighbour();
        current_neighbour->horizontal_flip();

        if(correct_position())
        {
            // std::cout << "fix by horizontal_flip\n";
            return true;
        }
        current_neighbour->horizontal_flip(); //go back to the original state

        current_neighbour->vertical_flip();
        if(correct_position())
        {
            // std::cout << "fix by vertical flip\n";
            return true;
        }
        current_neighbour->vertical_flip(); //go back to the original state

        for(int i = 0; i < 5 && !correct_position(); ++i) //we rotate 4 times since when the rotation doesn't fix we go back to the original position
            current_neighbour->rotate(1);

        if(correct_position())
        {
            // std::cout << "fix by rotation\n";
            return true;
        }

        current_neighbour->horizontal_flip();
        for(int i = 0; i < 5 && !correct_position(); ++i) //we rotate 4 times since when the rotation doesn't fix we go back to the original position
            current_neighbour->rotate(1);

        if(correct_position())
        {
            // std::cout << "fix by horizontal flip + rotation\n";
            return true;
        }
        current_neighbour->horizontal_flip(); //go back to the original state

        current_neighbour->vertical_flip();
        for(int i = 0; i < 5 && !correct_position(); ++i) //we rotate 4 times since when the rotation doesn't fix we go back to the original position
            current_neighbour->rotate(1);

        if(correct_position())
        {
            // std::cout << "fix by vertical flip + rotation \n";
            return true;
        }

        return false;
    }


    bool correct_upside_neighbour(Tile *tile) const
    {
        return tile->upside_neighbour() && tile->upside() == tile->upside_neighbour()->downside();
    }

    bool correct_rightside_neighbour(Tile *tile) const
    {
        return tile->rightside_neighbour() && tile->rightside() == tile->rightside_neighbour()->leftside();
    }

    bool correct_downside_neighbour(Tile *tile) const
    {
        return tile->downside_neighbour() && tile->downside() == tile->downside_neighbour()->upside();
    }

    bool correct_leftside_neighbour(Tile *tile) const
    {
        return tile->leftside_neighbour() && tile->leftside() == tile->leftside_neighbour()->rightside();
    }
};

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

            if(!tile.matches(current_tile))
                tile.matches_flipped(current_tile);
        }
    }

    //First pass: find neighbouring tiles
    std::size_t answer{1};
    for(auto tile : tiles)
    {
        if(tile.n_neighbours() == 2)
            answer *= tile.id;
    }

    std::cout << "Part 1: " << answer << '\n';

    auto current_tile_it = std::find_if(tiles.begin(), tiles.end(), [](Tile const& tile)
    {
        return tile.n_neighbours() == 2;
    });

    FixTileVisitor tile_visitor;
    tile_visitor.visit(&*current_tile_it);

    TileVisitor tv;
    tv.visit(&*current_tile_it);

    return 0;
}
