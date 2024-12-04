/**
 * @file day4-part1.cc
 * @brief Solution for Advent of Code 2024 Day 4 Part 1
 *
 * This file contains a program that reads a grid from "input.txt" and searches
 * for occurrences of a specific word in all 8 possible directions. The search
 * is performed in parallel using multiple threads to speed up the process.
 * The total count of word occurrences is then printed to the standard output.
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 04.12.2024
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <atomic>
#include <mutex>

const std::vector<std::pair<int, int>> directions = {
    {0, 1}, {1, 0}, {1, 1}, {1, -1}, {-1, 0}, {0, -1}, {-1, -1}, {-1, 1}};

int searchInRange(const std::vector<std::string> &grid, const std::string &word, int startRow, int endRow)
{
    int count = 0;
    int rows = grid.size();
    int cols = grid[0].size();
    int wordLength = word.size();

    for (int x = startRow; x < endRow; ++x)
    {
        for (int y = 0; y < cols; ++y)
        {
            for (const auto &[dx, dy] : directions)
            {
                bool match = true;
                for (int i = 0; i < wordLength; ++i)
                {
                    int nx = x + i * dx;
                    int ny = y + i * dy;
                    if (nx < 0 || ny < 0 || nx >= rows || ny >= cols || grid[nx][ny] != word[i])
                    {
                        match = false;
                        break;
                    }
                }
                if (match)
                {
                    ++count;
                }
            }
        }
    }
    return count;
}

int countWordOccurrencesParallel(const std::vector<std::string> &grid, const std::string &word, int numThreads)
{
    int rows = grid.size();
    std::vector<std::future<int>> futures;

    int chunkSize = (rows + numThreads - 1) / numThreads;
    for (int t = 0; t < numThreads; ++t)
    {
        int startRow = t * chunkSize;
        int endRow = std::min(startRow + chunkSize, rows);

        if (startRow < endRow)
        {
            futures.push_back(std::async(std::launch::async, searchInRange, std::cref(grid), word, startRow, endRow));
        }
    }

    int totalOccurrences = 0;
    for (auto &fut : futures)
    {
        totalOccurrences += fut.get();
    }

    return totalOccurrences;
}

int main()
{
    std::ifstream inputFile("input.txt");
    if (!inputFile)
    {
        std::cerr << "Error: Unable to open input.txt\n";
        return 1;
    }

    std::vector<std::string> grid;
    std::string line;
    while (std::getline(inputFile, line))
    {
        grid.push_back(line);
    }
    inputFile.close();

    const std::string word = "XMAS";
    const int numThreads = std::thread::hardware_concurrency();

    std::cout << "Total occurrences of " << word << ": "
              << countWordOccurrencesParallel(grid, word, numThreads) << std::endl;

    return 0;
}
