/**
 * @file day12-part1.cc
 * @brief Solution for Advent of Code 2024 Day 12 Part 1
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 13.12.2024
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <utility>
#include <set>
#include <fstream>

using GardenMap = std::vector<std::string>;
using RegionInfo = std::unordered_map<char, std::vector<std::pair<int, int>>>;

struct Region {
    int area = 0;
    int perimeter = 0;
};

class Garden {
public:
    explicit Garden(const std::string& file_path) {
        std::ifstream input_file(file_path);
        if (!input_file) {
            throw std::runtime_error("Error: Could not open input file.");
        }

        std::string line;
        while (std::getline(input_file, line)) {
            map_.push_back(line);
        }

        if (!map_.empty()) {
            rows_ = map_.size();
            cols_ = map_[0].size();
        } else {
            rows_ = cols_ = 0;
        }
    }

    int CalculateTotalFencePrice() {
        std::unordered_map<char, std::vector<Region>> regions;
        std::set<std::pair<int, int>> visited;

        for (int row = 0; row < rows_; ++row) {
            for (int col = 0; col < cols_; ++col) {
                char plant = map_[row][col];
                if (visited.find({row, col}) == visited.end()) {
                    Region region = ExploreRegion(row, col, plant, visited);
                    regions[plant].emplace_back(region);
                }
            }
        }

        return CalculatePrice(regions);
    }

private:
    GardenMap map_;
    int rows_;
    int cols_;
    const std::vector<std::pair<int, int>> directions_ = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    bool IsValid(int row, int col, char plant) const {
        return row >= 0 && row < rows_ && col >= 0 && col < cols_ && map_[row][col] == plant;
    }

    Region ExploreRegion(int row, int col, char plant, std::set<std::pair<int, int>>& visited) {
        Region region;
        std::queue<std::pair<int, int>> to_visit;
        to_visit.push({row, col});
        visited.insert({row, col});

        while (!to_visit.empty()) {
            auto [cur_row, cur_col] = to_visit.front();
            to_visit.pop();
            ++region.area;

            for (const auto& [dr, dc] : directions_) {
                int new_row = cur_row + dr;
                int new_col = cur_col + dc;

                if (IsValid(new_row, new_col, plant)) {
                    if (visited.find({new_row, new_col}) == visited.end()) {
                        to_visit.push({new_row, new_col});
                        visited.insert({new_row, new_col});
                    }
                } else {
                    ++region.perimeter;
                }
            }
        }

        return region;
    }

    int CalculatePrice(const std::unordered_map<char, std::vector<Region>>& regions) const {
        int total_price = 0;
        for (const auto& [plant, region_list] : regions) {
            for (const auto& region : region_list) {
                total_price += region.area * region.perimeter;
            }
        }
        return total_price;
    }
};

int main() {
    try {
        Garden garden("input.txt");
        std::cout << "Total Fence Price: " << garden.CalculateTotalFencePrice() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
