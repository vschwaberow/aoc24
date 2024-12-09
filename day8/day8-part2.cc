/**
 * @file day8-part2.cc
 * @brief Solution for Advent of Code 2024 Day 8 Part 2
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 09.12.2024
 */

#include <algorithm>
#include <cassert>
#include <charconv>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <fstream>

int main()
{
    std::vector<std::string> grid;
    {
        std::ifstream input_file("input.txt");
        std::string line;
        while (std::getline(input_file, line))
        {
            if (!line.empty())
            {
                grid.push_back(std::move(line));
            }
        }
    }

    const int height = grid.size();
    const int width = grid.empty() ? 0 : grid[0].size();

    struct Position
    {
        int x;
        int y;

        auto operator<=>(const Position &) const = default;
    };

    std::map<char, std::vector<Position>> frequency_to_positions;

    for (int y = 0; y < height; ++y)
    {
        const auto &line = grid[y];
        for (int x = 0; x < width; ++x)
        {
            char c = line[x];
            if (std::isalnum(c))
            {
                frequency_to_positions[c].push_back({x, y});
            }
        }
    }

    std::set<Position> antinode_positions;

    for (const auto &[frequency, positions] : frequency_to_positions)
    {
        if (positions.size() < 2)
        {
            continue;
        }

        for (size_t i = 0; i < positions.size(); ++i)
        {
            for (size_t j = i + 1; j < positions.size(); ++j)
            {
                const auto &pos1 = positions[i];
                const auto &pos2 = positions[j];

                int dx = pos2.x - pos1.x;
                int dy = pos2.y - pos1.y;

                int gcd = std::gcd(dx, dy);

                int step_x = dx / gcd;
                int step_y = dy / gcd;

                auto add_positions = [&](int start_x, int start_y, int dir)
                {
                    int x = start_x;
                    int y = start_y;
                    while (true)
                    {
                        x += dir * step_x;
                        y += dir * step_y;
                        if (x < 0 || x >= width || y < 0 || y >= height)
                        {
                            break;
                        }
                        antinode_positions.insert({x, y});
                    }
                };

                add_positions(pos1.x, pos1.y, 1);
                add_positions(pos1.x, pos1.y, -1);

                antinode_positions.insert(pos1);
                antinode_positions.insert(pos2);
            }
        }
    }

    std::cout << antinode_positions.size() << std::endl;

    return 0;
}
