#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <tuple>
#include <thread>
#include <chrono>

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

    void swap_nop_jmp()
    {
        if(op_code == OpCode::NOP){
            op_code = OpCode::JMP;
            name = "jmp";
        } else if(op_code == OpCode::JMP){
            op_code = OpCode::NOP;
            name = "nop";
        }
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

std::tuple<int, std::size_t> detect_loop(std::vector<Instruction> const& instructions)
{
    int counter{};
    std::vector<int> instructions_executed(instructions.size(), 0);
    // assuming that no instruction will try to make sp less than 0
    std::size_t sp = 0;
    // O(m) where m is the number of instructions since each
    //instructions can execute at most one time
    while(sp != instructions.size()) {
        instructions_executed[sp]++;
        auto instruction = instructions.at(sp);

        //std::cout << "Executing " << instruction << " at " << sp << '\n';
        switch(instruction.op_code)
            {
            case OpCode::JMP:
                if(instructions_executed[sp + instruction.value] == 1)
                    return std::make_tuple(counter, sp);
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

    return std::make_tuple(counter, sp);
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
    std::cout << "Part 1\n";
    std::cout << "Counter before loop: " << std::get<0>(result) << "" << '\n';
    if(std::get<1>(result) != instructions.size())
        std::cout << "Loop found at " << std::get<1>(result) << '\n';
    else
        std::cout << "No loop found\n";

    //part b
    std::cout << "Part 2\n";
    //O(n^2) where n is the number of instructions since we can have all instructions as jmp or nop
    {
        for(std::size_t i = 0; i != instructions.size(); ++i)
            {
                if(instructions.at(i).op_code == OpCode::ACC)
                    continue;

                instructions.at(i).swap_nop_jmp();
                result = detect_loop(instructions);
                instructions.at(i).swap_nop_jmp();
                if(std::get<1>(result) == instructions.size()) {
                    std::cout << "Loop found at " << i << " in a " << instructions.at(i) <<'\n';
                    break;
                }
            }
    }

    std::cout << "Counter before loop: " << std::get<0>(result) << "" << '\n';
    if(std::get<1>(result) != instructions.size())
        std::cout << "Loop found at " << std::get<1>(result) << '\n';
    else
        std::cout << "No loop found\n";

    return 0;
}
