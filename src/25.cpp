#include <bits/c++config.h>
#include <iostream>
#include <fstream>
#include <cassert>

constexpr int value{20201227};

constexpr std::size_t handshake(int sub, std::size_t last_result, int loopsize = 1)
{
    return (last_result * sub) % value;
}

constexpr std::size_t handshake_loop(int sub, int loopsize)
{
    std::size_t result{1};
    for(int i = 0; i < loopsize; ++i)
    {
        result = handshake(sub, result);
    }

    return result;
}


int main(int argc, char *argv[])
{
    std::size_t card_subject_number = 8335663;
    std::size_t door_subject_number = 8614349;

    bool card_found{false};
    bool door_found{false};
    std::size_t card_last_handshake{1};
    std::size_t door_last_handshake{1};

    std::size_t card_loop{};
    std::size_t door_loop{};
    int loop_size{1};
    while(!card_loop || !door_found)
    {
        //instead of running the loop every time, just keep calling
        //handshake with the last result and accumulating its result
        //in last handshake variable.
        if((card_last_handshake = handshake(7, card_last_handshake)) == card_subject_number)
        {
            card_loop = loop_size;
            card_found = true;
        }

        if((door_last_handshake = handshake(7, door_last_handshake)) == door_subject_number)
        {
            door_loop = loop_size;
            door_found = true;
        }
        ++loop_size;
    }

    std::cout << "encryption key " << handshake_loop(door_subject_number, card_loop) << '\n';
    std::cout << "encryption key " << handshake_loop(card_subject_number, door_loop) << '\n';

    return 0;
}
