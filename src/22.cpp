#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <list>

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::list<int> player_one_deck;
    std::list<int> player_two_deck;

    bool is_player_one_deck = true;

    std::ifstream ifs{argv[1]};
    std::string line;
    std::getline(ifs, line); //skip first player line
    while(std::getline(ifs, line))
    {
        if(line.empty())
        {
            std::getline(ifs, line); //skip second player line
            is_player_one_deck = false;
            continue;
        }

        if(is_player_one_deck)
        {
            player_one_deck.push_back(std::stoi(line));
        }
        else
        {
            player_two_deck.push_back(std::stoi(line));
        }
    }

    //Part 1: play the game
    while(!player_one_deck.empty() && !player_two_deck.empty())
    {
        auto topcard_player_one = player_one_deck.front();
        player_one_deck.pop_front();
        auto topcard_player_two = player_two_deck.front();
        player_two_deck.pop_front();
        if(topcard_player_one > topcard_player_two)
        {
            player_one_deck.push_back(topcard_player_one);
            player_one_deck.push_back(topcard_player_two);
        }
        else
        {
            player_two_deck.push_back(topcard_player_two);
            player_two_deck.push_back(topcard_player_one);
        }
    }

    auto winner_deck = player_one_deck.empty() ? player_two_deck : player_one_deck;

    int multiplier = winner_deck.size();
    int accumulator{};
    for(auto card : winner_deck)
    {
        accumulator += card * multiplier--;
    }

    std::cout << "Part 1: " << accumulator << '\n';

    return 0;
}
