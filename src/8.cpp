#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <tuple>

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

std::ostream& operator<<(std::ostream &os, Instruction const& instruction)
{
    os << instruction.name << ' ' << instruction.value;
    return os;
}

std::tuple<int, std::size_t, bool> detect_loop(std::vector<Instruction> const& instructions)
{
    int counter{};
    std::set<int> instructions_executed;
    // assuming that no instruction will try to make sp less than 0
    std::size_t sp = 0;
    std::size_t sp_before = 0;
    // O(m) where m is the number of instructions since each
    //instructions can execute at most one time
    while(sp != instructions.size()) {
        if(std::find(std::cbegin(instructions_executed),
                     std::cend(instructions_executed), sp) != std::cend(instructions_executed))
            return std::make_tuple(counter, sp_before, true);

        instructions_executed.insert(sp);
        auto instruction = instructions.at(sp);

        //std::cout << "Executing " << instruction << '\n';
        //std::cout << sp << " " << sp_before << "\n";
        sp_before = sp;
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

    return std::make_tuple(counter, sp_before, false);
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

    //part a
    auto result = detect_loop(instructions);
    std::cout << "Counter before loop: " << std::get<0>(result) << "" << '\n';
    if(std::get<2>(result))
        std::cout << "Loop found at " << std::get<1>(result) << '\n';
    else
        std::cout << "No loop found\n";


    return 0;
}
