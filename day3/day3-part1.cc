/**
 * @file day3-part1.cc
 * @brief Solution for Advent of Code 2024 Day 3 Part 1
 *
 * This file contains a program that reads a file named "list.txt", searches
 * for patterns of the form "mul(x,y)" where x and y are numbers between 1 and
 * 999, multiplies these numbers, and sums the results. The final sum is then
 * printed to the standard output.
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 03.12.2024
 */

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class MulSolver
{
public:
    explicit MulSolver(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open input file");
        }
        content_ = std::string(std::istreambuf_iterator<char>(file),
                               std::istreambuf_iterator<char>());
    }

    uint64_t Solve() const
    {
        uint64_t sum = 0;
        std::regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");

        for (auto it = std::sregex_iterator(content_.begin(), content_.end(), pattern);
             it != std::sregex_iterator(); ++it)
        {
            const auto &match = *it;
            if (match.size() != 3)
                continue;

            try
            {
                uint64_t x = std::stoull(match[1].str());
                uint64_t y = std::stoull(match[2].str());

                if (x > 999 || y > 999)
                    continue;
                sum += x * y;
            }
            catch (const std::exception &)
            {
                continue;
            }
        }

        return sum;
    }

private:
    std::string content_;
};

int main()
{
    try
    {
        MulSolver solver("list.txt");
        std::cout << solver.Solve() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}