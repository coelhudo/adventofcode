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

std::size_t evaluate(tokens_iterator &token,
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
            numbers.push(evaluate(++token, [](tokens_iterator &token){ return *token != ")";}));
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

    std::size_t accumulator{};
    for(auto formula: formulas)
    {
        // for(auto token: formula)
        // {
        //     std::cout << token << ' ';
        // }
        // std::cout << ": ";

        auto token = std::begin(formula);
        std::size_t result = evaluate(token, [&formula](tokens_iterator &token){ return token != std::end(formula);});
        accumulator += result;
        // std::cout << result << '\n';
    }

    std::cout << "Part A\nResult: " << accumulator << '\n';

    return 0;
}
