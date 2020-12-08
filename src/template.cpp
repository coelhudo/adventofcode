#include <iostream>
#include <fstream>
#include <cassert>

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    return 0;
}
