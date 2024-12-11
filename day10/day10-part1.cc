/**
 * @file day10-part1.cc
 * @brief Solution for Advent of Code 2024 Day 10 Part 1
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 11.12.2024
 */

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>

constexpr int kMaxHeight = 9;
constexpr int kMinHeight = 0;

using Position = std::pair<int, int>;

class TopographicMap
{
public:
    explicit TopographicMap(const std::string &filename) { LoadMap(filename); }

    int CalculateTotalTrailheadScores();

private:
    void LoadMap(const std::string &filename);

    int CalculateTrailheadScore(const Position &start_pos);

    std::vector<std::vector<int>> map_data_;
    int rows_ = 0;
    int cols_ = 0;
};

void TopographicMap::LoadMap(const std::string &filename)
{
    std::ifstream input_file(filename);
    if (!input_file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(input_file, line))
    {
        std::vector<int> row;
        for (char ch : line)
        {
            if (ch >= '0' && ch <= '9')
            {
                row.push_back(ch - '0');
            }
        }
        if (!row.empty())
        {
            map_data_.emplace_back(std::move(row));
        }
    }

    rows_ = static_cast<int>(map_data_.size());
    cols_ = rows_ > 0 ? static_cast<int>(map_data_[0].size()) : 0;
}

int TopographicMap::CalculateTrailheadScore(const Position &start_pos)
{
    std::set<Position> visited;
    std::set<Position> reached_nines;

    const auto IsValidPosition = [this](int x, int y)
    {
        return x >= 0 && x < rows_ && y >= 0 && y < cols_;
    };

    int score = 0;
    std::queue<Position> queue;
    queue.push(start_pos);
    visited.insert(start_pos);

    while (!queue.empty())
    {
        auto [x, y] = queue.front();
        queue.pop();
        int current_height = map_data_[x][y];

        if (current_height == kMaxHeight)
        {
            reached_nines.insert({x, y});
            continue;
        }

        const std::vector<Position> directions = {
            {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1}};

        for (const auto &[nx, ny] : directions)
        {
            if (IsValidPosition(nx, ny) && visited.find({nx, ny}) == visited.end())
            {
                int neighbor_height = map_data_[nx][ny];
                if (neighbor_height == current_height + 1)
                {
                    visited.insert({nx, ny});
                    queue.push({nx, ny});
                }
            }
        }
    }

    score = static_cast<int>(reached_nines.size());
    return score;
}

int TopographicMap::CalculateTotalTrailheadScores()
{
    int total_score = 0;

    const auto IsTrailhead = [this](int x, int y)
    {
        return map_data_[x][y] == kMinHeight;
    };

    for (int x = 0; x < rows_; ++x)
    {
        for (int y = 0; y < cols_; ++y)
        {
            if (IsTrailhead(x, y))
            {
                total_score += CalculateTrailheadScore({x, y});
            }
        }
    }

    return total_score;
}

int main()
{
    const std::string filename = "input.txt";
    auto topo_map = std::make_unique<TopographicMap>(filename);
    int total_score = topo_map->CalculateTotalTrailheadScores();
    std::cout << total_score << std::endl;
    return 0;
}