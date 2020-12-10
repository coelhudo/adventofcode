#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>


//O(n log n) since the construction of the set is the most expensive operation
int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    //O(n log n) to insert all elements from input
    std::set<int> adapters{std::istream_iterator<int>{ifs}, std::istream_iterator<int>{}};

    adapters.insert(0);

    //O(n)
    const int max_element = *std::max_element(std::cbegin(adapters), std::cend(adapters));
    adapters.insert(max_element + 3);

    std::vector<int> differences;

    //O(n)
    std::adjacent_difference(std::cbegin(adapters), std::cend(adapters), std::back_inserter(differences));

    //O(n)
    const int number_of_ones = std::count(std::cbegin(differences), std::cend(differences), 1);
    //O(n)
    const int number_of_threes = std::count(std::cbegin(differences), std::cend(differences), 3);

    std::cout << "Result " << number_of_ones * number_of_threes << '\n';

    return 0;
}
