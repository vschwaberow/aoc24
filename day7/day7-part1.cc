/**
 * @file day7-part1.cc
 * @brief Solution for Advent of Code 2024 Day 7 Part 1
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 07.12.2024
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <functional>

using Target = long long;
using Numbers = std::vector<long long>;
using Operators = std::vector<char>;

std::pair<Target, Numbers> ParseLine(const std::string &line)
{
    auto colon_pos = line.find(':');
    if (colon_pos == std::string::npos)
    {
        throw std::runtime_error("Invalid line format: Missing ':'");
    }

    Target target = std::stoll(line.substr(0, colon_pos));
    Numbers numbers;

    std::istringstream number_stream(line.substr(colon_pos + 1));
    long long number;
    while (number_stream >> number)
    {
        numbers.push_back(number);
    }

    return {target, numbers};
}

void GenerateOperatorCombinations(size_t num_slots, const std::function<void(const Operators &)> &callback)
{
    size_t num_combinations = 1 << num_slots; // 2^n combinations
    for (size_t mask = 0; mask < num_combinations; ++mask)
    {
        Operators ops;
        for (size_t i = 0; i < num_slots; ++i)
        {
            ops.push_back((mask & (1 << i)) ? '*' : '+');
        }
        callback(ops);
    }
}

long long EvaluateExpression(const Numbers &numbers, const Operators &operators)
{
    assert(numbers.size() == operators.size() + 1);
    long long result = numbers[0];

    for (size_t i = 0; i < operators.size(); ++i)
    {
        switch (operators[i])
        {
        case '+':
            result += numbers[i + 1];
            break;
        case '*':
            result *= numbers[i + 1];
            break;
        default:
            throw std::logic_error("Invalid operator");
        }
    }

    return result;
}

bool MatchesTarget(Target target, const Numbers &numbers)
{
    size_t num_slots = numbers.size() - 1;
    bool found = false;

    GenerateOperatorCombinations(num_slots, [&](const Operators &ops)
                                 {
        if (EvaluateExpression(numbers, ops) == target)
        {
            found = true;
        } });

    return found;
}

long long ComputeCalibrationSum(const std::string &filename)
{
    std::ifstream input_file(filename);
    if (!input_file.is_open())
    {
        throw std::runtime_error("Unable to open input file");
    }

    long long total_sum = 0;
    std::string line;

    while (std::getline(input_file, line))
    {
        if (line.empty())
        {
            continue;
        }

        try
        {
            auto [target, numbers] = ParseLine(line);
            if (MatchesTarget(target, numbers))
            {
                total_sum += target;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Warning: " << e.what() << " in line: " << line << '\n';
        }
    }

    return total_sum;
}

int main()
{
    constexpr char kInputFile[] = "input.txt";

    try
    {
        long long calibration_result = ComputeCalibrationSum(kInputFile);
        std::cout << "Total Calibration Result: " << calibration_result << '\n';
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
