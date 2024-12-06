/**
 * @file day6-part1.cc
 * @brief Solution for Advent of Code 2024 Day 6 Part 1
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 06.12.2024
 */

#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <string>
#include <tuple>
#include <stdexcept>
#include <chrono>

struct PairHash
{
    std::size_t operator()(const std::pair<int, int> &p) const noexcept
    {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

enum class Direction
{
    kUp,
    kRight,
    kDown,
    kLeft
};

class GuardMap
{
public:
    explicit GuardMap(const std::string &filename)
    {
        LoadMap(filename);
        FindGuard();
    }

    [[nodiscard]] std::size_t SimulateGuardPath()
    {
        visited_positions_.insert({guard_x_, guard_y_});

        while (true)
        {
            auto [next_x, next_y] = GetNextPosition(guard_x_, guard_y_, guard_dir_);

            if (!IsWithinBounds(next_x, next_y))
                break;

            if (map_[next_x][next_y] == '#')
            {
                guard_dir_ = TurnRight(guard_dir_);
            }
            else
            {
                guard_x_ = next_x;
                guard_y_ = next_y;
                visited_positions_.insert({guard_x_, guard_y_});
            }
        }

        return visited_positions_.size();
    }

private:
    std::vector<std::string> map_;
    int guard_x_ = 0, guard_y_ = 0;
    Direction guard_dir_ = Direction::kUp;
    std::unordered_set<std::pair<int, int>, PairHash> visited_positions_;

    void LoadMap(const std::string &filename)
    {
        std::ifstream input_file(filename);
        if (!input_file.is_open())
        {
            throw std::runtime_error("Error: Unable to open file '" + filename + "'");
        }

        std::string line;
        while (std::getline(input_file, line))
        {
            map_.push_back(line);
        }

        if (map_.empty())
        {
            throw std::runtime_error("Error: Map file is empty.");
        }
    }

    void FindGuard()
    {
        for (int i = 0; i < static_cast<int>(map_.size()); ++i)
        {
            for (int j = 0; j < static_cast<int>(map_[i].size()); ++j)
            {
                if (map_[i][j] == '^')
                {
                    guard_x_ = i;
                    guard_y_ = j;
                    return;
                }
            }
        }
        throw std::runtime_error("Error: Guard's starting position (^) not found.");
    }

    [[nodiscard]] bool IsWithinBounds(int x, int y) const
    {
        return x >= 0 && x < static_cast<int>(map_.size()) &&
               y >= 0 && y < static_cast<int>(map_[0].size());
    }

    [[nodiscard]] static std::pair<int, int> GetNextPosition(int x, int y, Direction dir)
    {
        switch (dir)
        {
        case Direction::kUp:
            return {x - 1, y};
        case Direction::kRight:
            return {x, y + 1};
        case Direction::kDown:
            return {x + 1, y};
        case Direction::kLeft:
            return {x, y - 1};
        }
        return {x, y};
    }

    [[nodiscard]] static Direction TurnRight(Direction dir)
    {
        switch (dir)
        {
        case Direction::kUp:
            return Direction::kRight;
        case Direction::kRight:
            return Direction::kDown;
        case Direction::kDown:
            return Direction::kLeft;
        case Direction::kLeft:
            return Direction::kUp;
        }
        return dir;
    }
};

int main()
{
    try
    {
        const std::string input_file = "input.txt";

        auto start_time = std::chrono::high_resolution_clock::now();

        GuardMap guard_map(input_file);
        std::size_t visited_positions = guard_map.SimulateGuardPath();

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end_time - start_time;

        std::cout << "Number of distinct positions visited: " << visited_positions << '\n';
        std::cout << "Execution time: " << elapsed_time.count() << " seconds\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
