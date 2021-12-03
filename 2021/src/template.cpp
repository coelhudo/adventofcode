// Most of the files will follow this pattern
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }

    std::ifstream ifs{argv[1]};
    // this is gonna vary
    std::vector<int> inputs{std::istream_iterator<int>{ifs}, std::istream_iterator<int>{}};

    //or
    std::string line;
    while(std::getline(ifs, line)) {
        std::stringstream sstr{line};
        //parse the line here
    }

    return 0;
}
