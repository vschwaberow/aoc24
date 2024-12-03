/**
 * @file day3-part2.cc
 * @brief Solution for Advent of Code 2024 Day 3 Part 2
 *
 * This file contains a program that reads a file named "list.txt", searches
 * for patterns of the form "mul(x,y)" where x and y are numbers between 1 and
 * 999, and multiplies these numbers if multiplication is enabled. It also
 * processes "do()" and "don't()" instructions to enable or disable 
 * multiplication.
 * 
 * The final sum of the multiplications is then printed to the standard output.
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
#include <variant>
#include <span>
#include <algorithm>

class InstructionProcessor
{
public:
    enum class InstructionType
    {
        kMultiply,
        kEnable,
        kDisable
    };

    struct Instruction
    {
        InstructionType type;
        size_t position;
        std::variant<std::pair<uint64_t, uint64_t>, std::monostate> operands;
    };

    explicit InstructionProcessor(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open input file");
        }
        content_ = std::string(std::istreambuf_iterator<char>(file),
                               std::istreambuf_iterator<char>());
        ParseInstructions();
    }

    uint64_t Solve() const
    {
        uint64_t sum = 0;
        bool multiplication_enabled = true;

        for (const auto &instruction : instructions_)
        {
            switch (instruction.type)
            {
            case InstructionType::kMultiply:
            {
                if (!multiplication_enabled)
                    continue;
                const auto &[x, y] = std::get<std::pair<uint64_t, uint64_t>>(
                    instruction.operands);
                sum += x * y;
                break;
            }
            case InstructionType::kEnable:
                multiplication_enabled = true;
                break;
            case InstructionType::kDisable:
                multiplication_enabled = false;
                break;
            }
        }

        return sum;
    }

private:
    void ParseInstructions()
    {
        ParseMultiplyInstructions();
        ParseControlInstructions();
        SortInstructions();
    }

    void ParseMultiplyInstructions()
    {
        std::regex mul_pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");

        for (auto it = std::sregex_iterator(content_.begin(), content_.end(),
                                            mul_pattern);
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

                instructions_.push_back({.type = InstructionType::kMultiply,
                                         .position = static_cast<size_t>(match.position()),
                                         .operands = std::make_pair(x, y)});
            }
            catch (const std::exception &)
            {
                continue;
            }
        }
    }

    void ParseControlInstructions()
    {
        std::regex do_pattern(R"(do\(\))");
        std::regex dont_pattern(R"(don't\(\))");

        for (auto it = std::sregex_iterator(content_.begin(), content_.end(),
                                            do_pattern);
             it != std::sregex_iterator(); ++it)
        {
            instructions_.push_back({.type = InstructionType::kEnable,
                                     .position = static_cast<size_t>(it->position()),
                                     .operands = std::monostate{}});
        }

        for (auto it = std::sregex_iterator(content_.begin(), content_.end(),
                                            dont_pattern);
             it != std::sregex_iterator(); ++it)
        {
            instructions_.push_back({.type = InstructionType::kDisable,
                                     .position = static_cast<size_t>(it->position()),
                                     .operands = std::monostate{}});
        }
    }

    void SortInstructions()
    {
        std::sort(instructions_.begin(), instructions_.end(),
                  [](const auto &a, const auto &b)
                  {
                      return a.position < b.position;
                  });
    }

    std::string content_;
    std::vector<Instruction> instructions_;
};

int main()
{
    try
    {
        InstructionProcessor processor("list.txt");
        std::cout << processor.Solve() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}