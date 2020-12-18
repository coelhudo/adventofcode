#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <set>
#include <algorithm>
#include <tuple>
#include <vector>
#include <functional>

typedef std::tuple<int,int,int> Coordinate;

std::ostream & operator<<(std::ostream &os, Coordinate const& coord){
    os << std::get<0>(coord) << ' ' << std::get<1>(coord) << ' ' << std::get<2>(coord);
    return os;
}

Coordinate operator+(Coordinate lhs, Coordinate rhs) {
    return {std::get<0>(lhs) + std::get<0>(rhs),
            std::get<1>(lhs) + std::get<1>(rhs),
            std::get<2>(lhs) + std::get<2>(rhs)};
}

typedef std::set<Coordinate> Cubes3DSpace;

void apply_rule(Cubes3DSpace const& space, Cubes3DSpace const &neighbours_delta,
                Cubes3DSpace const& reference_cubes_space, Cubes3DSpace &new_cubes_space,
                std::function<bool(int)> rule,
                std::function<void(Coordinate)> process_inactive_neighbour = [](Coordinate neighbour){}) {
    for(auto cube: space)
    {
        int count{};
        for(auto delta : neighbours_delta) {
            auto neighbour = cube + delta;
            auto is_neighbour_active = cube != neighbour;
            is_neighbour_active &= (reference_cubes_space.find(neighbour) != std::end(reference_cubes_space));
            if(is_neighbour_active)
                ++count;
            else
                process_inactive_neighbour(neighbour);
        }
        if(rule(count))
            new_cubes_space.emplace(cube);
    }
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    Cubes3DSpace cubes_space;

    std::string line;
    int current_x = 0;
    while(ifs >> line) {
        for(int current_y = 0; current_y < line.size(); ++current_y) {
            if(line[current_y] == '#') {
                cubes_space.emplace(0, current_x, current_y);
            }
        }
        ++current_x;
    }

    Cubes3DSpace neighbours_delta;
    Cubes3DSpace initial_deltas;
    for(auto x :{0,1})
        for(auto y :{0,1})
            for(auto z: {0,1})
                initial_deltas.emplace(x,y,z);

    for(auto [x,y,z]: initial_deltas) {
        neighbours_delta.emplace(x,y,z);
        neighbours_delta.emplace(x*-1, y, z);
        neighbours_delta.emplace(x, y*-1, z);
        neighbours_delta.emplace(x, y, z*-1);
        neighbours_delta.emplace(x*-1, y*-1, z);
        neighbours_delta.emplace(x, y*-1, z*-1);
        neighbours_delta.emplace(x*-1, y, z*-1);
        neighbours_delta.emplace(x*-1, y*-1, z*-1);
    }

    int cycles = 6;
    while(cycles--)
    {
        Cubes3DSpace inactive_cubes_with_neighbours;
        Cubes3DSpace new_cubes_space;

        //Apply first rule on active cubes
        apply_rule(cubes_space, neighbours_delta, cubes_space, new_cubes_space,
                   [](int count) { return count == 2 || count == 3;},
                   [&inactive_cubes_with_neighbours](Coordinate coordinate) {
                       inactive_cubes_with_neighbours.emplace(coordinate);
                   });


        //Apply second rule on inactive cubes
        apply_rule(inactive_cubes_with_neighbours, neighbours_delta, cubes_space, new_cubes_space,
                   [](int count) { return count == 3;});

        cubes_space = new_cubes_space;
    }

    std::cout << "Part A\n";
    std::cout << cubes_space.size() << '\n';

    return 0;
}
