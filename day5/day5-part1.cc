/**
 * @file day5-part1.cc
 * @brief Solution for Advent of Code 2024 Day 5 Part 1
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

    constexpr size_t CHUNK_SIZE = 1024;
    constexpr auto DELIMITER = '|';
}

class TopologicalValidator final
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

    [[nodiscard]] auto processChunk(std::span<const PageSequence> chunk) const
        -> std::vector<int32_t>
    {
        std::vector<int32_t> middle_pages;
        middle_pages.reserve(chunk.size());

        for (const auto &sequence : chunk)
        {
            if (validateSequence(sequence))
            {
                middle_pages.push_back(sequence[sequence.size() / 2]);
            }
        }
        return middle_pages;
    }

public:
    explicit TopologicalValidator(const std::filesystem::path &filepath)
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

    TopologicalValidator(const TopologicalValidator &) = delete;
    TopologicalValidator &operator=(const TopologicalValidator &) = delete;
    TopologicalValidator(TopologicalValidator &&) noexcept = default;
    TopologicalValidator &operator=(TopologicalValidator &&) noexcept = default;
    ~TopologicalValidator() = default;
};

int main()
{
    try
    {
        const TopologicalValidator validator("input.txt");
        std::cout << validator.computeMiddlePageSum() << '\n';
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
}