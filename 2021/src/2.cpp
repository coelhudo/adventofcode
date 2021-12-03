#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <tuple>

class Submarine
{
public:
    void forward(int direction) { horizontal += direction; };
    void down(int direction) { vertical += direction; };
    void up(int direction) { vertical -= direction; };

    int horizontal{0};
    int vertical{0};
};

class ComplexSubmarine: public Submarine
{
public:
    void forward(int direction) {
        horizontal += direction;
        vertical += (aim * direction);
    }
    void down(int direction) { aim += direction; };
    void up(int direction) { aim -= direction; };

    int aim{0};
};

template<typename Sub>
int process_moves(auto moves) {
    Sub sub;
    for(auto [command, direction] : moves) {
        if (command == "forward") {
            sub.forward(direction);
        } else if (command == "down") {
            sub.down(direction);
        } else if (command == "up") {
            sub.up(direction);
        } else {
            throw std::runtime_error("Impossible has happened; invalid command");
        }
    }

    return sub.horizontal * sub.vertical;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::vector<std::tuple<std::string, int>> moves;
    std::ifstream ifs{argv[1]};
    std::string line;
    while(std::getline(ifs, line)) {
        int direction;
        std::string command;
        std::stringstream iss{line};
        iss >> command >> direction;
        moves.emplace_back(command, direction);
    }

    std::cout << "Part 1: " << process_moves<Submarine>(moves) << '\n';
    std::cout << "Part 2: " << process_moves<ComplexSubmarine>(moves) << '\n';

    return 0;
}
