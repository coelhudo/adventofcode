#include <iostream>
#include <fstream>
#include <cassert>
#include <array>
#include <functional>

enum class Code: char
{
    S = 'S', N = 'N', W = 'W', E = 'E', L = 'L', R = 'R', F = 'F'
};

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    const std::array<Code, 4> directions{Code::E, Code::S, Code::W, Code::N};

    Code direction = Code('E');
    int initial_dir = 0;
    int horizonal_pos = 0;
    int vertical_pos = 0;

    auto rotate = [&](Code code, int value) {
        if(code == Code::R) {
            initial_dir = (initial_dir + (value / 90)) % 4;
            return directions[initial_dir];
        } else if (code == Code::L) {
            initial_dir = (initial_dir - (value / 90)) % 4;
            if(initial_dir < 0)
                initial_dir += 4;
            return directions[initial_dir];
        }
        return direction;
    };

    auto move = [&](Code code, int value) {
        switch(code) {
        case Code::S:
            vertical_pos -= value;
            break;
        case Code::N:
            vertical_pos += value;
            break;
        case Code::W:
            horizonal_pos -= value;
            break;
        case Code::E:
            horizonal_pos += value;
            break;
        default:
            return;
        }
    };

    std::string line;
    while(ifs >> line)
    {
        const Code code = Code(line[0]);
        const int current_value = std::stoi(line.substr(1));
        switch(code)
        {
        case Code::S:
        case Code::N:
        case Code::W:
        case Code::E:
            move(code, current_value);
            break;
        case Code::L:
        case Code::R:
            direction = rotate(code, current_value);
            break;
        case Code::F:
            move(direction, current_value);
            break;
        }
    }

    std::cout << std::abs(horizonal_pos) << " " << std::abs(vertical_pos) << '\n';
    std::cout << std::abs(horizonal_pos) + std::abs(vertical_pos) << '\n';

    return 0;
}
