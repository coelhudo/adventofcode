#include <iostream>
#include <fstream>
#include <cassert>
#include <array>
#include <vector>
#include <iterator>
#include <functional>

enum class Code: char
    {
        S = 'S', N = 'N', W = 'W', E = 'E', L = 'L', R = 'R', F = 'F'
    };

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};
    std::vector<std::string> instructions{std::istream_iterator<std::string>{ifs}, std::istream_iterator<std::string>{}};

    const std::array<Code, 4> directions{Code::E, Code::S, Code::W, Code::N};

    auto move = [](int &horizontal_pos, int &vertical_pos, Code code, int value) {
        switch(code) {
        case Code::S:
            vertical_pos -= value;
            break;
        case Code::N:
            vertical_pos += value;
            break;
        case Code::W:
            horizontal_pos -= value;
            break;
        case Code::E:
            horizontal_pos += value;
            break;
        default:
            return;
        }
    };

    auto rotate = [&](int &waypoint_hpos, int &waypoint_vpos, Code code, int value) {
            //this like matrix rotation but simplified
            if(code == Code::R && value == 90 || code == Code::L && value == 270)
            {
                waypoint_hpos *= -1;
                std::swap(waypoint_hpos, waypoint_vpos);
            }
            else if(value == 180)
            {
                waypoint_hpos *= -1;
                waypoint_vpos *= -1;
            }
            else if(code == Code::R && value == 270 || code == Code::L && value == 90)
            {
                waypoint_vpos *= -1;
                std::swap(waypoint_hpos, waypoint_vpos);
            }
        };

    {
        int initial_dir = 0;
        int waypoint_hpos = 1;
        int waypoint_vpos = 0;
        int horizonal_pos = 0;
        int vertical_pos = 0;


        auto move_ship = std::bind(move, std::ref(horizonal_pos), std::ref(vertical_pos), std::placeholders::_1, std::placeholders::_2);
        auto rotate_waypoint = std::bind(rotate, std::ref(waypoint_hpos), std::ref(waypoint_vpos), std::placeholders::_1, std::placeholders::_2);

        for(std::string const& instruction : instructions)
            {
                const Code code = Code(instruction[0]);
                const int current_value = std::stoi(instruction.substr(1));
                switch(code)
                    {
                    case Code::S:
                    case Code::N:
                    case Code::W:
                    case Code::E:
                        move_ship(code, current_value);
                        break;
                    case Code::L:
                    case Code::R:
                        rotate_waypoint(code, current_value);
                        break;
                    case Code::F:
                        horizonal_pos += current_value * waypoint_hpos;
                        vertical_pos += current_value * waypoint_vpos;
                        break;
                    }
            }

        std::cout << "Part a\n";
        std::cout << std::abs(horizonal_pos) << " " << std::abs(vertical_pos) << '\n';
        std::cout << std::abs(horizonal_pos) + std::abs(vertical_pos) << '\n';
    }

    //part b

    {
        int waypoint_hpos = 10;
        int waypoint_vpos = 1;
        int ship_hpos = 0;
        int ship_vpos = 0;

        auto move_waypoint = std::bind(move, std::ref(waypoint_hpos), std::ref(waypoint_vpos), std::placeholders::_1, std::placeholders::_2);
        auto rotate_waypoint = std::bind(rotate, std::ref(waypoint_hpos), std::ref(waypoint_vpos), std::placeholders::_1, std::placeholders::_2);

        for(std::string const& instruction : instructions)
            {
                const Code code = Code(instruction[0]);
                const int current_value = std::stoi(instruction.substr(1));
                switch(code)
                    {
                    case Code::S:
                    case Code::N:
                    case Code::W:
                    case Code::E:
                        move_waypoint(code, current_value);
                        break;
                    case Code::L:
                    case Code::R:
                        rotate_waypoint(code, current_value);
                        break;
                    case Code::F:
                        ship_hpos += waypoint_hpos * current_value;
                        ship_vpos += waypoint_vpos * current_value;
                        break;
                    }
            }

        std::cout << "Part b\n";
        std::cout << std::abs(ship_hpos) << " " << std::abs(ship_vpos) << '\n';
        std::cout << std::abs(ship_hpos) + std::abs(ship_vpos) << '\n';
    }


    return 0;
}
