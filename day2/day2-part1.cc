/**
 * @file day2-part1.cc
 * @brief Safety check for level sequences in a list.
 *
 * This program reads a list of level sequences from a file named "list.txt".
 * Each line in the file represents a sequence of integer levels. The program
 * checks if each sequence is gradual, meaning the levels either consistently
 * increase or decrease with differences between 1 and 3. The check is performed
 * concurrently for each line to improve performance. The program outputs the
 * number of sequences that are considered safe.
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 02.12.2024
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <execution>
#include <future>
#include <numeric>

// Helper function to check if levels are gradually increasing or decreasing
bool isGradual(const std::vector<int> &levels)
{
    if (levels.size() < 2)
        return true;

    // Check first difference to determine direction
    bool increasing = levels[1] > levels[0];

    for (size_t i = 1; i < levels.size(); i++)
    {
        int diff = levels[i] - levels[i - 1];

        // Check if difference is between 1 and 3
        if (std::abs(diff) < 1 || std::abs(diff) > 3)
        {
            return false;
        }

        // Check if direction matches initial direction
        if (increasing && diff < 0)
            return false;
        if (!increasing && diff > 0)
            return false;
    }

    return true;
}

int main()
{
    std::ifstream infile("list.txt");
    if (!infile.is_open())
    {
        std::cerr << "Error: Unable to open file list.txt" << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;

    // Read all lines from file
    while (std::getline(infile, line))
    {
        lines.push_back(line);
    }
    infile.close();

    // Concurrently process each line for safety checks
    auto process_line = [](const std::string &line) -> bool
    {
        std::stringstream ss(line);
        std::vector<int> levels;
        int level;
        while (ss >> level)
        {
            levels.push_back(level);
        }
        return isGradual(levels);
    };

    std::vector<std::future<bool>> futures;
    for (const auto &l : lines)
    {
        futures.push_back(std::async(std::launch::async, process_line, l));
    }

    int safe_reports = std::accumulate(futures.begin(), futures.end(), 0, [](int sum, std::future<bool> &fut)
                                       { return sum + (fut.get() ? 1 : 0); });

    std::cout << "Number of safe reports: " << safe_reports << std::endl;
    return 0;
}
