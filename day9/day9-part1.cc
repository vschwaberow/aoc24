/**
 * @file day9-part1.cc
 * @brief Solution for Advent of Code 2024 Day 9 Part 1
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 09.12.2024
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::ifstream in("input.txt");
    std::string line;
    std::getline(in, line);

    std::vector<int> disk;
    {
        int file_id = 0;
        bool file_mode = true;
        for (char c : line)
        {
            int length = c - '0';
            if (file_mode)
            {
                for (int i = 0; i < length; ++i)
                {
                    disk.push_back(file_id);
                }
                file_id++;
            }
            else
            {
                for (int i = 0; i < length; ++i)
                {
                    disk.push_back(-1);
                }
            }
            file_mode = !file_mode;
        }
    }

    auto leftmost_free = [&]()
    {
        for (int i = 0; i < (int)disk.size(); ++i)
        {
            if (disk[i] == -1)
                return i;
        }
        return -1;
    };

    auto rightmost_file = [&]()
    {
        for (int i = (int)disk.size() - 1; i >= 0; --i)
        {
            if (disk[i] != -1)
                return i;
        }
        return -1;
    };

    while (true)
    {
        int lf = leftmost_free();
        if (lf == -1)
            break;
        int rf = rightmost_file();
        if (rf == -1 || rf < lf)
            break;
        int f = disk[rf];
        disk[rf] = -1;
        disk[lf] = f;
    }

    long long checksum = 0;
    for (int i = 0; i < (int)disk.size(); ++i)
    {
        if (disk[i] != -1)
            checksum += (long long)i * disk[i];
    }

    std::cout << checksum << "\n";
}
