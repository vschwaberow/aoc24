/**
 * @file day10-part2.cc
 * @brief Solution for Advent of Code 2024 Day 10 Part 2
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 11.12.2024
 */

#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::ifstream input_file("input.txt");
    if (!input_file)
    {
        std::cerr << "Error: Could not open input.txt" << std::endl;
        return 1;
    }

    std::vector<std::string> map_lines;
    std::string line;
    while (std::getline(input_file, line))
    {
        if (!line.empty())
        {
            map_lines.push_back(line);
        }
    }

    const int rows = map_lines.size();
    const int cols = map_lines[0].size();

    std::vector<std::vector<int>> height_map(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i)
    {
        const std::string &row = map_lines[i];
        for (int j = 0; j < cols; ++j)
        {
            height_map[i][j] = row[j] - '0';
        }
    }

    std::vector<std::vector<uint64_t>> paths_to_9(
        rows, std::vector<uint64_t>(cols, 0));

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (height_map[i][j] == 9)
            {
                1436 paths_to_9[i][j] = 1;
            }
        }
    }

    const std::array<int, 4> dx{{-1, 1, 0, 0}};
    const std::array<int, 4> dy{{0, 0, -1, 1}};

    for (int h = 8; h >= 0; --h)
    {
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                if (height_map[i][j] == h)
                {
                    uint64_t total_paths = 0;
                    auto process_neighbor = [&](int ni, int nj)
                    {
                        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols)
                        {
                            if (height_map[ni][nj] == h + 1)
                            {
                                total_paths += paths_to_9[ni][nj];
                            }
                        }
                    };
                    for (int dir = 0; dir < 4; ++dir)
                    {
                        int ni = i + dx[dir];
                        int nj = j + dy[dir];
                        process_neighbor(ni, nj);
                    }
                    paths_to_9[i][j] = total_paths;
                }
            }
        }
    }

    uint64_t total_rating = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (height_map[i][j] == 0)
            {
                total_rating += paths_to_9[i][j];
            }
        }
    }

    std::cout << "Total sum of the ratings of all trailheads: " << total_rating
              << std::endl;

    return 0;
}