// Most of the files will follow this pattern
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <iterator>

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout << "missing input file\n";
        return -1;
    }
    std::ifstream ifs{argv[1]};
    std::string line;

    std::vector<std::vector<std::string>> entries;
    std::vector<std::vector<std::string>> results;
    while(std::getline(ifs, line)) {
        auto it = line.find( '|');
        if (it == std::string::npos)
            continue;

        std::stringstream ss(line.substr(0, it-1));
        std::string word;
        std::vector<std::string> a;
        while(!ss.eof()) {
            ss >> word;
            a.emplace_back(word);
        }
        entries.emplace_back(a);

        std::stringstream ss2(line.substr(it+1));
        std::vector<std::string> b;
        while(!ss2.eof()) {
            ss2 >> word;
            b.emplace_back(word);
        }
        results.emplace_back(b);
    }

    int counter = 0;
    for(auto r : results) {
        for (auto o : r) {
            if(o.size() == 2 || o.size() == 3 || o.size() == 4 || o.size() == 7)
                ++counter;
        }
    }

    std::cout << "Part 1: " << counter << '\n';

    return 0;
}
