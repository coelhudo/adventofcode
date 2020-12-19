#include <bits/c++config.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <functional>

void execute_operation(std::stack<std::size_t> &numbers, std::stack<std::string> &operations)
{
    auto lhs = numbers.top();
    numbers.pop();
    auto rhs = numbers.top();
    numbers.pop();
    auto op = operations.top();
    operations.pop();

    if(op == "+") {
        numbers.push(lhs+rhs);
    }
    else {
        numbers.push(lhs*rhs);
    }
}

typedef std::vector<std::string>::iterator tokens_iterator;

std::size_t evaluate_a(tokens_iterator &token,
                       std::function<bool(tokens_iterator&)> terminate_condition){
    std::stack<std::string> operations;
    std::stack<std::size_t> numbers;

    while(terminate_condition(token))
    {
        if(numbers.size() == 2)
            execute_operation(numbers, operations);

        if(*token == "+" || *token == "*")
        {
            operations.push(*token);
        }
        else if(*token == "(")
        {
            numbers.push(evaluate_a(++token, [](tokens_iterator &token){ return *token != ")";}));
        }
        else
        {
            numbers.push(std::stoi(*token));
        }
        ++token;
    }

    execute_operation(numbers, operations);

    assert(operations.size() == 0);
    assert(numbers.size() == 1);

    return numbers.top();
}

std::size_t evaluate_b(tokens_iterator &token,
                       std::function<bool(tokens_iterator&)> terminate_condition){
    std::stack<std::string> operations;
    std::stack<std::size_t> numbers;

    while(terminate_condition(token))
    {
        if(*token == "+" || *token == "*")
        {
            operations.push(*token);
        }
        else if(*token == "(")
        {
            numbers.push(evaluate_b(++token, [](tokens_iterator &token){ return *token != ")";}));
        }
        else
        {
            numbers.push(std::stoi(*token));
        }
        ++token;
    }

    //save all multiplication for later while executes all add
    //operations
    std::stack<std::string> mul_operations;
    std::stack<std::size_t> mul_numbers;
    while(!operations.empty()) {
        if(operations.top() == "*")
        {
            mul_numbers.push(numbers.top());
            numbers.pop();
            mul_operations.push(operations.top());
            operations.pop();
        } else {
            execute_operation(numbers, operations);
        }
    }

    mul_numbers.push(numbers.top());
    numbers.pop();

    //exeucte multiplications operations
    while(!mul_operations.empty())
    {
        execute_operation(mul_numbers, mul_operations);
    }

    assert(operations.size() == 0);
    assert(numbers.size() == 0);
    assert(mul_operations.size() == 0);
    assert(mul_numbers.size() == 1);

    return mul_numbers.top();
}

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};
    std::string line;

    std::vector<std::vector<std::string>> formulas;

    while(std::getline(ifs, line))
    {
        std::istringstream iss{line};
        std::string token;
        std::vector<std::string> tokens;
        while(iss >> token)
        {
            if(auto pos = std::find(std::begin(token), std::end(token), '('); pos != std::end(token))
            {
                int count = 0;
                while (*pos == '(')
                {
                    tokens.push_back("(");
                    pos = std::find(pos+1, std::end(token), '(');
                    ++count;
                }
                tokens.push_back(token.substr(count));
            }
            else if(auto pos = std::find(std::rbegin(token), std::rend(token), ')'); pos != std::rend(token))
            {
                int count = 0;
                while (*pos == ')')
                {
                    pos = std::find(pos+1, std::rend(token), ')');
                    ++count;
                }
                tokens.push_back(token.substr(0, token.size()-count));
                while(count--)
                    tokens.push_back(")");
            }
            else
            {
                tokens.push_back(token);
            }

        }

        formulas.push_back(tokens);
    }

    std::size_t accumulator_part_a{};
    std::size_t accumulator_part_b{};
    for(auto formula: formulas)
    {
        // for(auto token: formula)
        // {
        //     std::cout << token << ' ';
        // }
        // std::cout << ": \n";

        {
            auto token = std::begin(formula);
            std::size_t result = evaluate_a(token, [&formula](tokens_iterator &token){ return token != std::end(formula);});
            accumulator_part_a += result;
            // std::cout << "part a " << result << '\n';
        }

        {
            auto token = std::begin(formula);
            std::size_t result = evaluate_b(token, [&formula](tokens_iterator &token){ return token != std::end(formula);});
            accumulator_part_b += result;
            // std::cout << "part b " << result << '\n';
        }

    }

    std::cout << "Part A\nResult: " << accumulator_part_a << '\n';
    std::cout << "Part B\nResult: " << accumulator_part_b << '\n';


    return 0;
}
