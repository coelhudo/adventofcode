#include <bits/c++config.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <string>
#include <list>
#include <set>
#include <algorithm>

namespace std
{
    //to have the list snapshot
    template<>
    struct hash<std::list<int>>
    {
        std::size_t operator()(std::list<int> const& vec) const {
            std::size_t seed = vec.size();
            for(auto& i : vec) {
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}

bool recursive_combat(std::list<int> &player_one_deck, std::list<int> &player_two_deck, int game)
{
    std::set<std::size_t> snapshot;
    int round = 1;
    while(!player_one_deck.empty() && !player_two_deck.empty())
    {
        auto deck_one_snapshot = std::hash<std::list<int>>()(player_one_deck);
        auto deck_two_snapshot = std::hash<std::list<int>>()(player_two_deck);

        if(snapshot.find(deck_one_snapshot) != snapshot.end() || snapshot.find(deck_two_snapshot) != snapshot.end())
            return true; //player one wins

        snapshot.insert(deck_one_snapshot);
        snapshot.insert(deck_two_snapshot);

        auto topcard_player_one = player_one_deck.front();
        player_one_deck.pop_front();
        auto topcard_player_two = player_two_deck.front();
        player_two_deck.pop_front();

        bool player_one_won = false;
        if(topcard_player_one > player_one_deck.size() || topcard_player_two > player_two_deck.size())
            player_one_won = topcard_player_one > topcard_player_two;
        else
        {
            std::list<int> copy_player_one_deck;
            std::copy_n(player_one_deck.begin(), topcard_player_one, std::inserter(copy_player_one_deck, copy_player_one_deck.begin()));

            std::list<int> copy_player_two_deck;
            std::copy_n(player_two_deck.begin(), topcard_player_two, std::inserter(copy_player_two_deck, copy_player_two_deck.begin()));

            player_one_won = recursive_combat(copy_player_one_deck, copy_player_two_deck, game+1);
        }

        if(player_one_won)
        {
            player_one_deck.push_back(topcard_player_one);
            player_one_deck.push_back(topcard_player_two);
        }
        else
        {
            player_two_deck.push_back(topcard_player_two);
            player_two_deck.push_back(topcard_player_one);
        }
        ++round;
    }

    return !player_one_deck.empty();
}


int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::list<int> player_one_original_deck;
    std::list<int> player_two_original_deck;

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
            player_one_original_deck.push_back(std::stoi(line));
        }
        else
        {
            player_two_original_deck.push_back(std::stoi(line));
        }
    }

    //Part 1: play the game
    auto player_one_deck = player_one_original_deck;
    auto player_two_deck = player_two_original_deck;
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

    auto compute_score = [](std::list<int> const& winner_deck){
        int multiplier = winner_deck.size();
        int accumulator{};
        for(auto card : winner_deck)
        {
            accumulator += card * multiplier--;
        }
        return accumulator;
    };


    std::cout << "Part 1: " << compute_score(winner_deck) << '\n';

    //Part 2: recursive game
    player_one_deck = player_one_original_deck;
    player_two_deck = player_two_original_deck;

    auto winner = recursive_combat(player_one_deck, player_two_deck, 1);

    winner_deck = player_one_deck.empty() ? player_two_deck : player_one_deck;
    for(auto card: winner_deck)
        std::cout << card << " ";
    std::cout << "\n";

    std::cout << "Part 2: " << compute_score(winner_deck) << '\n';

    return 0;
}
