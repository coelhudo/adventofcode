#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <set>
#include <algorithm>
#include <tuple>
#include <vector>
#include <functional>
#include <iterator>

typedef std::tuple<int,int,int> Coordinate;
typedef std::tuple<int,int,int,int> HyperCoordinate;

std::ostream & operator<<(std::ostream &os, Coordinate const& coord){
    os << std::get<0>(coord) << ' ' << std::get<1>(coord) << ' ' << std::get<2>(coord);
    return os;
}

std::ostream & operator<<(std::ostream &os, HyperCoordinate const& coord){
    os << std::get<0>(coord) << ' ' << std::get<1>(coord) << ' ' << std::get<2>(coord) << ' ' << std::get<3>(coord);
    return os;
}

Coordinate operator+(Coordinate lhs, Coordinate rhs) {
    return {std::get<0>(lhs) + std::get<0>(rhs),
        std::get<1>(lhs) + std::get<1>(rhs),
        std::get<2>(lhs) + std::get<2>(rhs)};
}

HyperCoordinate operator+(HyperCoordinate lhs, HyperCoordinate rhs) {
    return {std::get<0>(lhs) + std::get<0>(rhs),
        std::get<1>(lhs) + std::get<1>(rhs),
        std::get<2>(lhs) + std::get<2>(rhs),
        std::get<3>(lhs) + std::get<3>(rhs)};
}

typedef std::set<Coordinate> Cubes3DSpace;
typedef std::set<HyperCoordinate> Cubes4DSpace;

template<typename CubesSpaceT>
void apply_rule(CubesSpaceT const& space, CubesSpaceT const &neighbours_delta,
                CubesSpaceT const& reference_cubes_space, CubesSpaceT &new_cubes_space,
                std::function<bool(int)> rule,
                std::function<void(typename CubesSpaceT::value_type)> process_inactive_neighbour = [](typename CubesSpaceT::value_type neighbour){}) {
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

template<typename CubesSpaceT>
void execute(CubesSpaceT &cubes_space, CubesSpaceT const& neighbours_delta ) {
    int cycles = 6;
    while(cycles--)
    {
        CubesSpaceT inactive_cubes_with_neighbours;
        CubesSpaceT new_cubes_space;

        //Apply first rule on active cubes
        apply_rule<CubesSpaceT>(cubes_space, neighbours_delta, cubes_space, new_cubes_space,
                                             [](int count) { return count == 2 || count == 3;},
                                             [&inactive_cubes_with_neighbours](typename CubesSpaceT::value_type coordinate) {
                                                 inactive_cubes_with_neighbours.emplace(coordinate);
                                             });


        //Apply second rule on inactive cubes
        apply_rule<CubesSpaceT>(inactive_cubes_with_neighbours, neighbours_delta, cubes_space, new_cubes_space,
                                             [](int count) { return count == 3;});

        cubes_space = new_cubes_space;
    }
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};
    std::vector<std::string> input{std::istream_iterator<std::string>{ifs}, std::istream_iterator<std::string>{}};

    //Part A
    {
        Cubes3DSpace cubes_space;

        int current_x = 0;
        for(auto entry : input) {
            for(int current_y = 0; current_y < entry.size(); ++current_y) {
                if(entry[current_y] == '#') {
                    cubes_space.emplace(0, current_x, current_y);
                }
            }
            ++current_x;
        }

        Cubes3DSpace neighbours_delta;
        for(auto x :{-1,0,1})
            for(auto y :{-1,0,1})
                for(auto z: {-1,0,1})
                    neighbours_delta.emplace(x,y,z);

        execute<Cubes3DSpace>(cubes_space, neighbours_delta);

        std::cout << "Part A\n";
        std::cout << cubes_space.size() << '\n';
    }

    //Part B
    {
        Cubes4DSpace cubes_hyperspace;

        int current_x = 0;
        for(auto entry : input) {
            for(int current_y = 0; current_y < entry.size(); ++current_y) {
                if(entry[current_y] == '#') {
                    cubes_hyperspace.emplace(0, 0, current_x, current_y);
                }
            }
            ++current_x;
        }

        Cubes4DSpace neighbours_delta;

        for(auto x :{-1,0,1})
            for(auto y :{-1,0,1})
                for(auto z: {-1,0,1})
                    for(auto w: {-1,0,1})
                        neighbours_delta.emplace(x,y,z,w);

        execute<Cubes4DSpace>(cubes_hyperspace, neighbours_delta);

        std::cout << "Part B\n";
        std::cout << cubes_hyperspace.size() << '\n';
    }

    return 0;
}
