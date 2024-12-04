/**
 * @file day4-part2.cc
 * @brief Solution for Advent of Code 2024 Day 4 Part 2
 *
 * This file contains a program that reads a grid from "input.txt" and searches
 * for occurrences of the pattern "X-MAS" in all 8 possible directions.
 * The search is performed in parallel using multiple threads to speed up
 * the process.
 *
 * The total count of pattern occurrences is then printed to the
 * standard output.
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 04.12.2024
 */

#include <algorithm>
#include <optional>
#include <format>
#include <fstream>
#include <future>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <thread>
#include <vector>

class GridSearcher
{
public:
    explicit GridSearcher(std::string_view filename)
    {
        LoadGrid(filename);
    }

    [[nodiscard]] std::optional<int> CountXMASPatterns() const
    {
        if (grid_.empty() || grid_[0].empty())
        {
            return std::nullopt;
        }
        return CountXMASParallel(std::thread::hardware_concurrency());
    }

private:
    void LoadGrid(std::string_view filename)
    {
        std::ifstream file(filename.data());
        if (!file)
        {
            throw std::runtime_error(
                std::format("Unable to open file: {}", filename));
        }

        std::string line;
        while (std::getline(file, line))
        {
            grid_.push_back(std::move(line));
        }
    }

    [[nodiscard]] bool CheckDiagonal(size_t x, size_t y, int dx, int dy) const
    {
        if (x + dx >= grid_.size() || x - dx >= grid_.size() ||
            y + dy >= grid_[0].size() || y - dy >= grid_[0].size())
        {
            return false;
        }

        char m = grid_[x - dx][y - dy];
        char s = grid_[x + dx][y + dy];

        return (m == 'M' && s == 'S') || (m == 'S' && s == 'M');
    }

    [[nodiscard]] bool IsXMASPattern(size_t x, size_t y) const
    {
        if (x >= grid_.size() || y >= grid_[0].size() || grid_[x][y] != 'A')
        {
            return false;
        }

        bool diagonal1 = CheckDiagonal(x, y, 1, 1);
        bool diagonal2 = CheckDiagonal(x, y, 1, -1);

        return diagonal1 && diagonal2;
    }

    [[nodiscard]] int SearchRange(size_t start_row, size_t end_row) const
    {
        int count = 0;
        for (size_t i = start_row; i < end_row; ++i)
        {
            for (size_t j = 1; j < grid_[i].size() - 1; ++j)
            {
                if (IsXMASPattern(i, j))
                {
                    count++;
                }
            }
        }
        return count;
    }

    [[nodiscard]] int CountXMASParallel(unsigned int num_threads) const
    {
        std::vector<std::future<int>> futures;
        futures.reserve(num_threads);

        const size_t chunk_size = (grid_.size() + num_threads - 1) / num_threads;

        for (size_t t = 0; t < num_threads; ++t)
        {
            size_t start_row = t * chunk_size;
            size_t end_row = std::min(start_row + chunk_size, grid_.size());

            if (start_row < end_row)
            {
                futures.push_back(
                    std::async(std::launch::async,
                               &GridSearcher::SearchRange,
                               this,
                               start_row,
                               end_row));
            }
        }

        int total = 0;
        for (auto &future : futures)
        {
            total += future.get();
        }
        return total;
    }

    std::vector<std::string> grid_;
};

int main()
{
    try
    {
        GridSearcher searcher("input.txt");

        if (auto result = searcher.CountXMASPatterns())
        {
            std::cout << std::format("Total occurrences of X-MAS: {}\n", *result);
            return 0;
        }
        else
        {
            std::cerr << "Error: Empty or invalid grid\n";
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << std::format("Error: {}\n", e.what());
        return 1;
    }
}