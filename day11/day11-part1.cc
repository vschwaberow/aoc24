/**
 * @file day11-part1.cc
 * @brief Solution for Advent of Code 2024 Day 11 Part 1
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 11.12.2024
 */

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

using StoneNumber = std::string;

class InputReader
{
public:
    explicit InputReader(const std::string &file_path)
    {
        file_stream_.open(file_path);
        if (!file_stream_.is_open())
        {
            throw std::runtime_error("Could not open input file.");
        }
    }

    ~InputReader() { file_stream_.close(); }

    std::vector<StoneNumber> ReadInitialStones()
    {
        std::vector<StoneNumber> stones;
        std::string number;
        while (file_stream_ >> number)
        {
            stones.push_back(number);
        }
        return stones;
    }

private:
    std::ifstream file_stream_;
};

std::pair<StoneNumber, StoneNumber> SplitStone(const StoneNumber &number)
{
    size_t length = number.size();
    size_t half = length / 2;
    StoneNumber left = number.substr(0, half);
    StoneNumber right = number.substr(half);
    left.erase(0, left.find_first_not_of('0'));
    right.erase(0, right.find_first_not_of('0'));

    if (left.empty())
        left = "0";
    if (right.empty())
        right = "0";
    return {left, right};
}

std::vector<StoneNumber> TransformStone(const StoneNumber &stone_number)
{
    if (stone_number == "0")
    {
        return {"1"};
    }

    if (stone_number.size() % 2 == 0)
    {
        auto [left, right] = SplitStone(stone_number);
        return {std::move(left), std::move(right)};
    }

    auto MultiplyBy2024 = [](const StoneNumber &num_str) -> StoneNumber
    {
        std::vector<int> digits;
        for (char c : num_str)
        {
            digits.push_back(c - '0');
        }
        std::reverse(digits.begin(), digits.end());

        int multiplier = 2024;
        std::vector<int> result;
        int carry = 0;
        for (size_t i = 0; i < digits.size(); ++i)
        {
            int prod = digits[i] * multiplier + carry;
            result.push_back(prod % 10);
            carry = prod / 10;
        }
        while (carry > 0)
        {
            result.push_back(carry % 10);
            carry /= 10;
        }
        StoneNumber product;
        std::for_each(result.rbegin(), result.rend(),
                      [&product](int digit)
                      { product.push_back('0' + digit); });
        return product;
    };

    StoneNumber new_stone = MultiplyBy2024(stone_number);
    return {std::move(new_stone)};
}

int main()
{
    try
    {
        InputReader reader("input.txt");
        std::vector<StoneNumber> stones = reader.ReadInitialStones();

        constexpr int kTotalBlinks = 25;

        for (int blink = 0; blink < kTotalBlinks; ++blink)
        {
            std::vector<StoneNumber> new_stones;
            new_stones.reserve(stones.size() * 2);

            std::for_each(stones.begin(), stones.end(), [&new_stones](const StoneNumber &stone)
                          {
        auto transformed = TransformStone(stone);
        new_stones.insert(new_stones.end(), transformed.begin(), transformed.end()); });
            stones = std::move(new_stones);
        }

        std::cout << "After " << kTotalBlinks << " blinks, there are " << stones.size()
                  << " stones." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}