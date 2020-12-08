#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

enum class OpCode {JMP, NOP, ACC};

struct Instruction
{
    Instruction(std::string const& name, int value): name{name}, value{value}
    {
        if(name == "jmp")
            op_code = OpCode::JMP;
        else if(name == "nop")
            op_code = OpCode::NOP;
        else //assuming it is acc
            op_code = OpCode::ACC;
    }

    std::string name;
    int value;
    OpCode op_code;
};

std::ostream &operator<<(std::ostream &os, Instruction const& instruction)
{
    os << instruction.name << ' ' << instruction.value;
    return os;
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    std::vector<Instruction> instructions;

    std::string line;

    //load instructions (we assume that everything is correct)
    //O(n) where n is the input file size
    while(std::getline(ifs, line))
        {
            const std::string name{line.substr(0, 3)};
            //not checking for std::invalid_argument or std::out_of_range exceptions
            const int value = std::stoi(line.substr(4));

            instructions.emplace_back(name, value);
        }

    //execute instructions
    int counter = 0;
    int sp = 0;

    std::set<int> instructions_executed;

    while(true) {
        if(std::find(std::cbegin(instructions_executed),
                     std::cend(instructions_executed), sp) != std::cend(instructions_executed))
            break;

        instructions_executed.insert(sp);
        auto instruction = instructions.at(sp);

        //std::cout << "Executing " << instruction << '\n';

        switch(instruction.op_code)
            {
            case OpCode::JMP:
                sp += instruction.value;
                break;
            case OpCode::ACC:
                counter += instruction.value;
            case OpCode::NOP:
                ++sp;
                break;
            default:
                std::cerr << "this is not suppose to happen\n";
            }
    }

    std::cout << "Counter before loop: " << counter << '\n';

    return 0;
}
