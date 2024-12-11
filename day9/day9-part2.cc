/**
 * @file day9-part2.cc
 * @brief Solution for Advent of Code 2024 Day 9 Part 2
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 09.12.2024
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

class DiskFragmenter
{
public:
    explicit DiskFragmenter(std::string_view disk_map) { ParseDiskMap(disk_map); }

    void Compact()
    {
        for (int file_id = static_cast<int>(file_lengths_.size()) - 1; file_id >= 0; --file_id)
        {
            MoveFile(file_id);
        }
    }

    [[nodiscard]] int64_t ComputeChecksum() const
    {
        int64_t checksum = 0;
        for (size_t pos = 0; pos < blocks_.size(); ++pos)
        {
            int file_id = blocks_[pos];
            if (file_id != kFreeBlock)
            {
                checksum += static_cast<int64_t>(pos) * file_id;
            }
        }
        return checksum;
    }

private:
    static constexpr int kFreeBlock = -1;

    void ParseDiskMap(std::string_view disk_map)
    {
        blocks_.clear();
        file_lengths_.clear();

        size_t idx = 0;
        int file_id = 0;
        auto disk_map_size = disk_map.size();

        auto CharToInt = [](char ch) -> int
        {
            assert(std::isdigit(ch));
            return ch - '0';
        };

        while (idx < disk_map_size)
        {
            int file_length = CharToInt(disk_map[idx]);
            file_lengths_.push_back(file_length);
            idx++;
            blocks_.insert(blocks_.end(), file_length, file_id);
            file_start_positions_[file_id] = static_cast<int>(blocks_.size()) - file_length;
            file_id++;
            if (idx >= disk_map_size)
                break;

            int free_length = CharToInt(disk_map[idx]);
            idx++;
            blocks_.insert(blocks_.end(), free_length, kFreeBlock);
        }
    }

    void MoveFile(int file_id)
    {
        int file_length = file_lengths_[file_id];
        int current_pos = file_start_positions_[file_id];

        auto free_span = FindLeftmostFreeSpan(file_length, current_pos);
        if (free_span)
        {
            int dest_pos = free_span.value();
            std::copy_n(blocks_.begin() + current_pos, file_length, blocks_.begin() + dest_pos);
            std::fill_n(blocks_.begin() + current_pos, file_length, kFreeBlock);
            file_start_positions_[file_id] = dest_pos;
        }
    }

    [[nodiscard]] std::optional<int> FindLeftmostFreeSpan(int file_length, int current_pos) const
    {
        int left = 0;
        while (left < current_pos)
        {
            while (left < current_pos && blocks_[left] != kFreeBlock)
            {
                ++left;
            }
            int right = left;
            while (right < current_pos && blocks_[right] == kFreeBlock)
            {
                ++right;
            }
            int span_length = right - left;
            if (span_length >= file_length)
            {
                return left;
            }
            left = right;
        }
        return std::nullopt;
    }

    std::vector<int> blocks_;
    std::vector<int> file_lengths_;
    std::unordered_map<int, int> file_start_positions_;
};

int main()
{
    std::ifstream input_file("input.txt");
    if (!input_file)
    {
        std::cerr << "Error opening input file 'input.txt'" << std::endl;
        return 1;
    }

    std::string disk_map_input;
    std::getline(input_file, disk_map_input);
    input_file.close();

    DiskFragmenter fragmenter(disk_map_input);

    fragmenter.Compact();

    int64_t checksum = fragmenter.ComputeChecksum();
    std::cout << checksum << std::endl;

    return 0;
}