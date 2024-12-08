/**
 * @file day8-part1.cc
 * @brief Solution for Advent of Code 2024 Day 8 Part 1
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 08.12.2024
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

using Position = std::pair<int, int>;

struct PositionComparator
{
    bool operator()(const Position &lhs, const Position &rhs) const
    {
        return lhs.first == rhs.first ? lhs.second < rhs.second
                                      : lhs.first < rhs.first;
    }
};

int main()
{
    std::vector<std::string> grid;
    std::string line;

    std::ifstream input_file("input.txt");
    if (!input_file)
    {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }
    while (std::getline(input_file, line))
    {
        if (!line.empty())
        {
            grid.push_back(line);
        }
    }
    input_file.close();

    const int height = grid.size();
    const int width = grid.empty() ? 0 : grid[0].size();

    std::map<char, std::vector<Position>> antennas;

    for (int y = 0; y < height; ++y)
    {
        const auto &row = grid[y];
        for (int x = 0; x < width; ++x)
        {
            char cell = row[x];
            if (cell != '.')
            {
                antennas[cell].emplace_back(x, y);
            }
        }
    }

    std::set<Position, PositionComparator> antinodes;

    auto is_within_bounds = [width, height](const Position &pos)
    {
        return pos.first >= 0 && pos.first < width && pos.second >= 0 &&
               pos.second < height;
    };

    for (const auto &[freq, positions] : antennas)
    {
        const auto &antenna_positions = positions;
        const int n = antenna_positions.size();

        for (int i = 0; i < n; ++i)
        {
            const Position &A = antenna_positions[i];
            for (int j = i + 1; j < n; ++j)
            {
                const Position &B = antenna_positions[j];

                Position C1{2 * A.first - B.first, 2 * A.second - B.second};
                Position C2{2 * B.first - A.first, 2 * B.second - A.second};

                if (is_within_bounds(C1))
                {
                    antinodes.insert(C1);
                }
                if (is_within_bounds(C2))
                {
                    antinodes.insert(C2);
                }
            }
        }
    }

    std::cout << antinodes.size() << std::endl;

    return 0;
}