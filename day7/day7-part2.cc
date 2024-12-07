/**
 * @file day7-part2.cc
 * @brief Solution for Advent of Code 2024 Day 7 part 2
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 07.12.2024
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class EquationSolver
{
public:
    explicit EquationSolver(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file)
        {
            throw std::runtime_error("Could not open input file");
        }

        std::string line;
        while (std::getline(file, line))
        {
            ParseLine(line);
        }
    }

    uint64_t SolvePartTwo() const
    {
        uint64_t sum = 0;
        for (const auto &eq : equations_)
        {
            if (CanSolveEquation(eq))
            {
                sum += eq.target;
            }
        }
        return sum;
    }

private:
    struct Equation
    {
        uint64_t target;
        std::vector<uint64_t> numbers;
    };
    std::vector<Equation> equations_;

    void ParseLine(const std::string &line)
    {
        std::istringstream iss(line);
        Equation eq;
        std::string temp;

        std::getline(iss, temp, ':');
        eq.target = std::stoull(temp);

        uint64_t num;
        while (iss >> num)
        {
            eq.numbers.push_back(num);
        }

        equations_.push_back(eq);
    }

    bool CanSolveEquation(const Equation &eq) const
    {
        return TryAllOperatorCombinations(eq.numbers, eq.target);
    }

    uint64_t ApplyOperator(uint64_t left, uint64_t right, int op) const
    {
        switch (op)
        {
        case 0:
            return left + right;
        case 1:
            return left * right;
        case 2:
            return std::stoull(std::to_string(left) + std::to_string(right));
        default:
            throw std::runtime_error("Invalid operator");
        }
    }

    bool TryAllOperatorCombinations(const std::vector<uint64_t> &numbers,
                                    uint64_t target) const
    {
        if (numbers.empty())
            return false;
        if (numbers.size() == 1)
            return numbers[0] == target;

        int num_ops = numbers.size() - 1;
        int max_combinations = 1;
        for (int i = 0; i < num_ops; ++i)
        {
            max_combinations *= 3;
        }

        for (int combination = 0; combination < max_combinations; ++combination)
        {
            std::vector<int> operators;
            int temp = combination;
            for (int i = 0; i < num_ops; ++i)
            {
                operators.push_back(temp % 3);
                temp /= 3;
            }

            uint64_t result = numbers[0];
            for (size_t i = 0; i < operators.size(); ++i)
            {
                result = ApplyOperator(result, numbers[i + 1], operators[i]);
            }

            if (result == target)
            {
                return true;
            }
        }

        return false;
    }
};

int main()
{
    try
    {
        EquationSolver solver("input.txt");
        uint64_t result = solver.SolvePartTwo();
        std::cout << "Part 2 result: " << result << std::endl;
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}