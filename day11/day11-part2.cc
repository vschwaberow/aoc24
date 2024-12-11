/**
 * @file day11-part2.cc
 * @brief Solution for Advent of Code 2024 Day 11 Part 2
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 11.12.2024
 */

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

auto BigMultiply = [](const std::string &number_str, int multiplier) -> std::string
{
    std::string result;
    int carry = 0;
    for (auto it = number_str.rbegin(); it != number_str.rend(); ++it)
    {
        int digit = *it - '0';
        int temp = digit * multiplier + carry;
        carry = temp / 10;
        temp %= 10;
        result.insert(result.begin(), temp + '0');
    }
    while (carry > 0)
    {
        result.insert(result.begin(), (carry % 10) + '0');
        carry /= 10;
    }
    return result;
};

auto StripLeadingZeros = [](const std::string &str) -> std::string
{
    size_t non_zero_pos = str.find_first_not_of('0');
    if (non_zero_pos == std::string::npos)
    {
        return "0";
    }
    return str.substr(non_zero_pos);
};

int main()
{
    std::ifstream input_file("input.txt");
    if (!input_file.is_open())
    {
        std::cerr << "Failed to open input.txt" << std::endl;
        return 1;
    }

    std::unordered_map<std::string, int64_t> stones;

    std::string number;
    while (input_file >> number)
    {
        stones[StripLeadingZeros(number)] += 1;
    }

    const int total_blinks = 75;
    for (int blink = 0; blink < total_blinks; ++blink)
    {
        std::unordered_map<std::string, int64_t> new_stones;

        for (const auto &[number_str, count] : stones)
        {
            if (number_str == "0")
            {
                new_stones["1"] += count;
            }
            else if (number_str.length() % 2 == 0)
            {
                size_t mid = number_str.length() / 2;
                std::string left_str = StripLeadingZeros(number_str.substr(0, mid));
                std::string right_str = StripLeadingZeros(number_str.substr(mid));

                new_stones[left_str] += count;
                new_stones[right_str] += count;
            }
            else
            {
                std::string new_number = BigMultiply(number_str, 2024);
                new_stones[new_number] += count;
            }
        }

        stones = std::move(new_stones);
    }

    int64_t total_stones = 0;
    for (const auto &[_, count] : stones)
    {
        total_stones += count;
    }

    std::cout << total_stones << std::endl;
    return 0;
}