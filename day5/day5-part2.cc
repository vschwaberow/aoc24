/**
 * @file day5-part2.cc
 * @brief Solution for Advent of Code 2024 Day 5 Part 2
 * 
 * I am using Kahn's algorithm to perform topological sorting. Maybe
 * I could use a different algorithm to improve performance. But
 * Kahn's algorithm is simple (!!!) and works well for this problem.
 *
 * SPDX-License-Identifier: MIT
 *
 * @date 05.12.2024
 */

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <numeric>
#include <queue>
#include <ranges>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace
{
    using DependencyGraph = std::unordered_map<int32_t, std::unordered_set<int32_t>>;
    using PageSequence = std::vector<int32_t>;
    using UpdateList = std::vector<PageSequence>;
    constexpr auto DELIMITER = '|';
    constexpr size_t CHUNK_SIZE = 1024;
}

class TopologicalSorter final
{
private:
    DependencyGraph dependencies_;
    UpdateList updates_;

    [[nodiscard]] static auto parseRule(std::string_view line) noexcept
        -> std::pair<int32_t, int32_t>
    {
        const auto delimiter_pos = line.find(DELIMITER);
        return {
            std::stoi(std::string(line.substr(0, delimiter_pos))),
            std::stoi(std::string(line.substr(delimiter_pos + 1)))};
    }

    [[nodiscard]] static auto parseUpdate(std::string_view line)
        -> PageSequence
    {
        PageSequence update;
        std::stringstream ss{std::string(line)};
        std::string token;

        while (std::getline(ss, token, ','))
        {
            update.push_back(std::stoi(token));
        }
        return update;
    }

    [[nodiscard]] auto validateSequence(std::span<const int32_t> sequence) const noexcept
        -> bool
    {
        std::unordered_set<int32_t> visited;
        visited.reserve(sequence.size());

        for (const auto current : sequence)
        {
            if (const auto it = dependencies_.find(current); it != dependencies_.end())
            {
                if (const auto &deps = it->second;
                    std::ranges::any_of(visited, [&deps](const auto page)
                                        { return deps.contains(page); }))
                {
                    return false;
                }
            }
            visited.insert(current);
        }
        return true;
    }

    [[nodiscard]] auto topologicalSort(const PageSequence &sequence) const
        -> PageSequence
    {
        std::unordered_map<int32_t, int32_t> inDegree;
        std::unordered_map<int32_t, std::unordered_set<int32_t>> adj;

        for (const auto page : sequence)
        {
            inDegree[page] = 0;
        }

        for (const auto page : sequence)
        {
            if (const auto it = dependencies_.find(page); it != dependencies_.end())
            {
                for (const auto dep : it->second)
                {
                    if (inDegree.contains(dep))
                    {
                        adj[page].insert(dep);
                        ++inDegree[dep];
                    }
                }
            }
        }

        std::priority_queue<int32_t, std::vector<int32_t>, std::greater<>> pq;
        for (const auto &[page, degree] : inDegree)
        {
            if (degree == 0)
            {
                pq.push(page);
            }
        }

        PageSequence result;
        result.reserve(sequence.size());

        while (!pq.empty())
        {
            const auto current = pq.top();
            pq.pop();
            result.push_back(current);

            for (const auto neighbor : adj[current])
            {
                --inDegree[neighbor];
                if (inDegree[neighbor] == 0)
                {
                    pq.push(neighbor);
                }
            }
        }

        return result;
    }

    [[nodiscard]] auto processChunk(std::span<const PageSequence> chunk) const
        -> std::vector<int32_t>
    {
        std::vector<int32_t> middle_pages;
        middle_pages.reserve(chunk.size());

        for (const auto &sequence : chunk)
        {
            if (!validateSequence(sequence))
            {
                const auto sorted = topologicalSort(sequence);
                middle_pages.push_back(sorted[sorted.size() / 2]);
            }
        }
        return middle_pages;
    }

public:
    explicit TopologicalSorter(const std::filesystem::path &filepath)
    {
        if (!std::filesystem::exists(filepath))
        {
            throw std::runtime_error("File not found: " + filepath.string());
        }

        std::ifstream file(filepath, std::ios::in);
        if (!file)
        {
            throw std::runtime_error("Failed to open file: " + filepath.string());
        }

        std::string line;
        bool parsing_updates = false;

        while (std::getline(file, line))
        {
            if (line.empty())
            {
                parsing_updates = true;
                continue;
            }

            if (!parsing_updates)
            {
                const auto [before, after] = parseRule(line);
                dependencies_[before].insert(after);
            }
            else
            {
                updates_.push_back(parseUpdate(line));
            }
        }
    }

    [[nodiscard]] auto computeMiddlePageSum() const -> int32_t
    {
        const size_t num_chunks = (updates_.size() + CHUNK_SIZE - 1) / CHUNK_SIZE;
        std::vector<std::future<std::vector<int32_t>>> futures;
        futures.reserve(num_chunks);

        for (size_t i = 0; i < num_chunks; ++i)
        {
            const size_t start = i * CHUNK_SIZE;
            const size_t end = std::min(start + CHUNK_SIZE, updates_.size());

            futures.push_back(std::async(
                std::launch::async,
                [this](auto first, auto last)
                {
                    return this->processChunk(std::span(
                        updates_.begin() + first,
                        updates_.begin() + last));
                },
                start,
                end));
        }

        int32_t total_sum = 0;
        for (auto &future : futures)
        {
            const auto middle_pages = future.get();
            total_sum += std::accumulate(
                middle_pages.begin(),
                middle_pages.end(),
                0);
        }

        return total_sum;
    }

    TopologicalSorter(const TopologicalSorter &) = delete;
    TopologicalSorter &operator=(const TopologicalSorter &) = delete;
    TopologicalSorter(TopologicalSorter &&) noexcept = default;
    TopologicalSorter &operator=(TopologicalSorter &&) noexcept = default;
    ~TopologicalSorter() = default;
};

int main()
{
    try
    {
        const TopologicalSorter sorter("input.txt");
        std::cout << sorter.computeMiddlePageSum() << '\n';
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
}