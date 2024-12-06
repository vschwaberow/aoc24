/**
 * @file day6_part2.cc
 * @brief Solution for Advent of Code 2024 Day 6 Part 2
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 06.12.2024
 */

#include <expected>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <variant>
#include <vector>

class MazePathFinder
{
public:
    using Position = std::pair<int, int>;
    using QueueItem = std::tuple<int, int, int, int>;

    explicit MazePathFinder(std::string_view filename)
    {
        LoadMap(filename);
        InitializeDirections();
    }

    [[nodiscard]] int SolvePart2()
    {
        int blocked_paths = 0;
        std::vector<Position> positions;

        for (const auto &[pos, _] : map_)
        {
            positions.push_back(pos);
        }

        auto map_copy = map_;

        for (const auto &pos : positions)
        {
            map_ = map_copy;

            if (map_[pos] == '^' || map_[pos] == '#')
            {
                continue;
            }

            map_[pos] = '#';
            seen_.clear();
            seen_.insert({start_.first, start_.second, 0, 0});

            blocked_paths += std::get<int>(FindPath(false));
        }

        return blocked_paths;
    }

private:
    std::map<Position, char> map_;
    Position start_;
    mutable std::set<QueueItem> seen_;
    std::map<Position, Position> directions_;

    void LoadMap(std::string_view filename)
    {
        std::ifstream file(filename.data());
        if (!file)
        {
            throw std::runtime_error(std::format("Failed to open file: {}", filename));
        }

        std::string line;
        int row = 0;

        while (std::getline(file, line))
        {
            for (size_t col = 0; col < line.length(); ++col)
            {
                map_[{row, col}] = line[col];
                if (line[col] == '^')
                {
                    start_ = {row, col};
                }
            }
            ++row;
        }
    }

    void InitializeDirections()
    {
        directions_ = {
            {{-1, 0}, {0, 1}},
            {{0, 1}, {1, 0}},
            {{1, 0}, {0, -1}},
            {{0, -1}, {-1, 0}}};
    }

    [[nodiscard]] std::variant<int, std::set<QueueItem>> FindPath(bool is_part1) const
    {
        std::queue<QueueItem> queue;
        queue.push({start_.first, start_.second, -1, 0});

        while (!queue.empty())
        {
            auto [row, col, delta_row, delta_col] = queue.front();
            queue.pop();

            if (!is_part1 && seen_.contains({row, col, delta_row, delta_col}))
            {
                return 1;
            }

            is_part1 ? seen_.insert({row, col, 0, 0})
                     : seen_.insert({row, col, delta_row, delta_col});

            int next_row = row + delta_row;
            int next_col = col + delta_col;
            Position next_pos = {next_row, next_col};

            if (!map_.contains(next_pos))
            {
                return is_part1 ? std::variant<int, std::set<QueueItem>>{seen_} : 0;
            }

            if (map_.at(next_pos) == '#')
            {
                auto [next_delta_row, next_delta_col] = directions_.at({delta_row, delta_col});
                queue.push({row, col, next_delta_row, next_delta_col});
            }
            else
            {
                queue.push({next_row, next_col, delta_row, delta_col});
            }
        }

        return std::set<QueueItem>{};
    }
};

int main()
{
    try
    {
        MazePathFinder maze("input.txt");
        std::cout << maze.SolvePart2() << '\n';
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}