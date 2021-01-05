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
        _upside.resize(10);
        _rightside.resize(10);
        _downside.resize(10);
        _leftside.resize(10);
    }

    TileCore() = default;
    TileCore(TileCore const&) = default;
    TileCore& operator=(TileCore const& other) = default;

    bool operator==(TileCore const& rhs) const
    {
        if(side_length != rhs.side_length)
            return false;

        for(int i = 0; i < side_length; ++i)
        {
            for(int j = 0; j < side_length; ++j)
            {
                if(this->operator()(i,j) != rhs(i,j))
                    return false;
            }
        }
        return true;
    }

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

    std::pair<int, int> index(int _row, int _column) const
    {
        int row = _row;
        int column = _column;
        if(h_flipped)
            column = side_length - _column - 1;
        if(v_flipped)
            row = side_length - _row - 1;
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
        invalid_cache();
    }

    void horizontal_flip()
    {
        h_flipped = !h_flipped;
        invalid_cache();
    }

    void vertical_flip()
    {
        v_flipped = !v_flipped;
        invalid_cache();
    }

    void invalid_cache()
    {
        invalid_upside = true;
        invalid_rightside = true;
        invalid_downside = true;
        invalid_leftside = true;
    }

    std::string const& upside() const
    {
        if(invalid_upside)
        {
            for(int i = 0; i < side_length; ++i)
                _upside[i] = this->operator()(0, i);
            invalid_upside = false;
        }

        return _upside;
    }

    std::string const& rightside() const
    {
        if(invalid_rightside)
        {
            for(int i = 0; i < side_length; ++i)
                _rightside[i] = this->operator()(i, 9);
            invalid_rightside = false;
        }

        return _rightside;
    }

    std::string const& downside() const
    {
        if(invalid_downside) {
            for(int i = 0; i < side_length; ++i)
                _downside[i] = this->operator()(9, i);
            invalid_downside = false;
        }
        return _downside;
    }

    std::string const& leftside() const
    {
        if(invalid_leftside)
        {
            for(int i = 0; i < side_length; ++i)
                _leftside[i] = this->operator()(i, 0);
            invalid_leftside = false;
        }

        return _leftside;
    }

    int base{0};
    std::string core;
    int side_length;
    mutable std::string _upside;
    mutable std::string _rightside;
    mutable std::string _downside;
    mutable std::string _leftside;
    mutable bool invalid_upside{true};
    mutable bool invalid_downside{true};
    mutable bool invalid_rightside{true};
    mutable bool invalid_leftside{true};
    bool h_flipped{false};
    bool v_flipped{false};
};

struct Tile
{
    explicit Tile(int id,
                  TileCore const& core) : id{id},
                                          core{core}
    {
    }

    bool matches(Tile & tile) {
        if(tile.upside() == upside() || tile.downside() == upside() || tile.leftside() == upside() || tile.rightside() == upside())
        {
            upside_neighbour(&tile);
        }
        else if(tile.upside() == downside() || tile.downside() == downside() || tile.leftside() == downside() || tile.rightside() == downside())
        {
            downside_neighbour(&tile);
        }
        else if(tile.upside() == leftside() || tile.downside() == leftside() || tile.leftside() == leftside() || tile.rightside() == leftside())
        {
            leftside_neighbour(&tile);
        }
        else if(tile.upside() == rightside() || tile.downside() == rightside() || tile.leftside() == rightside() || tile.rightside() == rightside())
        {
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
        tile.horizontal_flip(); //undo flip
        tile.vertical_flip();
        const bool result = matches(tile);
        tile.vertical_flip(); //undo flip
        return result;
    }

    bool operator<(Tile const&other) const
    {
        return id < other.id;
    }

    std::string const& upside() const
    {
        return core.upside();
    }

    std::string const& rightside() const
    {
        return core.rightside();
    }

    std::string const& downside() const
    {
        return core.downside();
    }

    std::string const& leftside() const
    {
        return core.leftside();
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
        neighbours.at(base % 4) = tile;
    }

    void rightside_neighbour(Tile *tile)
    {
        neighbours.at((base + 1) % 4)= tile;
    }

    void downside_neighbour(Tile *tile)
    {
        neighbours.at((base + 2) % 4) = tile;
    }

    void leftside_neighbour(Tile *tile)
    {
        neighbours.at((base + 3) % 4) = tile;
    }

    void rotate(int times)
    {
        base = base + ((4 - times) % 4);
        core.rotate(times);
    }

    void horizontal_flip()
    {
        core.horizontal_flip();
    }

    void vertical_flip()
    {
        core.vertical_flip();
    }

    int n_neighbours() const
    {
        return std::count_if(neighbours.cbegin(), neighbours.cend(), [](Tile const* tile) { return tile != nullptr;});
    }

    int id;
    int base{0};
    TileCore core;
    std::array<Tile*, 4> neighbours{};
};

std::ostream &operator<<(std::ostream &os, TileCore const& tile_core)
{
    for(int i = 0; i < tile_core.side_length; ++i)
    {
        for(int j = 0; j < tile_core.side_length; ++j)
        {
            os << tile_core(i, j);
        }
        os << '\n';
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, Tile const& tile)
{
    os << "Id: " << tile.id << '\n';
    os << "upside:\n" << tile.upside() << '\n';
    os << "rightside:\n" << tile.rightside() << '\n';
    os << "downside:\n" << tile.downside() << '\n';
    os << "leftside:\n" << tile.leftside() << '\n';
    os << "core: \n";
    os << tile.core << '\n';


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
        auto correct_upside_neighbour = [&tile]{ return tile->upside() == tile->upside_neighbour()->downside();};
        auto correct_rightside_neighbour = [&tile]{ return tile->rightside() == tile->rightside_neighbour()->leftside();};
        auto correct_downside_neighbour = [&tile]{ return tile->downside() == tile->downside_neighbour()->upside();};
        auto correct_leftside_neighbour = [&tile]{ return tile->leftside() == tile->leftside_neighbour()->rightside();};

        if(tile->upside_neighbour() && !correct_upside_neighbour())
        {
            auto upside_neighbour = [&tile]{ return tile->upside_neighbour();};
            fix(upside_neighbour, correct_upside_neighbour);
        }

        if(tile->rightside_neighbour() && !correct_rightside_neighbour())
        {
            auto rightside_neighbour = [&tile]{ return tile->rightside_neighbour();};
            fix(rightside_neighbour, correct_rightside_neighbour);
        }

        if(tile->downside_neighbour() && !correct_downside_neighbour())
        {
            auto downside_neighbour = [&tile]{ return tile->downside_neighbour();};
            fix(downside_neighbour, correct_downside_neighbour);
        }

        if(tile->leftside_neighbour() && !correct_leftside_neighbour())
        {
            auto leftside_neighbour = [&tile]{ return tile->leftside_neighbour();};
            fix(leftside_neighbour, correct_leftside_neighbour);
        }
    }

    bool fix(std::function<Tile*()> neighbour, std::function<bool()> correct_position) const
    {
        Tile *current_neighbour = neighbour();
        current_neighbour->horizontal_flip();

        if(correct_position())
            return true;

        current_neighbour->horizontal_flip(); //go back to the original state

        current_neighbour->vertical_flip();
        if(correct_position())
            return true;

        current_neighbour->vertical_flip(); //go back to the original state

        for(int i = 0; i < 5 && !correct_position(); ++i) //we rotate 4 times since when the rotation doesn't fix we go back to the original position
            current_neighbour->rotate(1);

        if(correct_position())
            return true;

        current_neighbour->horizontal_flip();
        for(int i = 0; i < 5 && !correct_position(); ++i) //we rotate 4 times since when the rotation doesn't fix we go back to the original position
            current_neighbour->rotate(1);

        if(correct_position())
            return true;

        current_neighbour->horizontal_flip(); //go back to the original state

        current_neighbour->vertical_flip();
        for(int i = 0; i < 5 && !correct_position(); ++i) //we rotate 4 times since when the rotation doesn't fix we go back to the original position
            current_neighbour->rotate(1);

        if(correct_position())
            return true;

        return false;
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
        std::ostringstream core;
        for(int i = 0; i < 10; ++i)
        {
            std::getline(ifs, line);
            core << line;
        }
        //order matters here
        tiles.emplace_back(tile_id, TileCore{10, core.str()});
    }

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

    // First pass: find neighbouring tiles
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
