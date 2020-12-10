#include <iostream>
#include <fstream>
#include <cassert>
#include <set>
#include <map>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <cmath>

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

    //part a
    std::cout << number_of_ones << " " << number_of_threes << '\n';
    std::cout << "Result " << number_of_ones * number_of_threes << '\n';

    //part b
    std::map<int, int> consecutive_ones_counter;
    int ones = 0;
    for(auto i : differences) {
        if(i == 1) {
            ones++;
        } else if(ones >= 1) {
            consecutive_ones_counter[ones]++;
            ones = 0;
        }
    }

    //Explanation:
    //
    //For example, if we have a list of adapters like this:
    //
    //0, 1, 2, 3, 6
    //
    //We cannot simply have a combination based on the number of subsets of 1, 2, 3: {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3} {} (this is 8 since the number of subsets is 2^number of elements) due to the constraints.
    //However, {0, 1, 2, 6} is not a valid combination of adapters, since, in the subset {1,2}, the difference between 6 and 2 is 4 jolts and we can only have a difference of 3 jolts at maximum and 1 jolts at minimum. So, the valid subsets would be {3}, {1, 3}, {2, 3}, {1, 2, 3} since they respect the constraints.

    //So, I realized that the number of combinations depends on the number of consecutive ones. From the example above, the adjacent difference is
    //1, 1, 1, 3

    //If we have 3 ones in sequence, we have to keep at least one number since 0, 6 is not a valid adapter sequence. That means that we will have only 4 valid adapters combinations instead of 8 without the restriction based on 2^2. If we had 4 consecutive ones, we would look for the number of subsets of a set of size 3. Always number of elements - 1. F

    //So, for each superset, we have to calculate the number of valid subsets.

    //I pre computed values of number of valid subsets since the number of consecutive ones for this problem is 4 at maximum.
    //I could have computed this using dynamic programming. Creating the superset for each number of consecutives ones found in the adjancet numbers container and searching for the valid subsets using the constraints in the problem description (1, 2, 3 jolts at maximum from the outlet to the device).

    std::size_t result = 1;

    std::map<int, int> combinations = {{0, 1}, {1, 2}, {2, 4}, {3, 7}, {4, 13}};

    //O(n) where n is the size of differences container (which is input size - 1)
    for(auto i : consecutive_ones_counter) {
        auto [ones, count] = i;
        std::cout << ones << " " << count << '\n';
        if(ones > 1) {
            result *= std::pow(combinations[ones-1], count);
        }
    }

    std::cout << result << '\n';

    return 0;
}
