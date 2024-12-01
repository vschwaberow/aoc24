#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <ranges>
#include <numeric>
#include <concepts>

// ANSI escape codes for Amiga-style Rainbow colorful output
#define RESET "\033[0m"
#define AMIGA_RED "\033[1;31m"
#define AMIGA_ORANGE "\033[1;33m"
#define AMIGA_YELLOW "\033[1;93m"
#define AMIGA_GREEN "\033[1;32m"
#define AMIGA_CYAN "\033[1;36m"
#define AMIGA_BLUE "\033[1;34m"
#define AMIGA_PURPLE "\033[1;35m"

// ANSI copper bar gradient effect (thin line with per-character shading using palette data)
constexpr const char *COPPER_BAR_GRADIENT =
    "\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[38;2;133;5;5m─\033[38;2;105;70;185m─\033[0m\n";

// A function to load numbers from file and split into two lists by line
std::pair<std::vector<int>, std::vector<int>> LoadListsFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::vector<int> left_list;
    std::vector<int> right_list;
    int left, right;
    while (file >> left >> right)
    {
        left_list.push_back(left);
        right_list.push_back(right);
    }
    return {left_list, right_list};
}

int main()
{
    try
    {
        // Load the two lists from file
        auto [left_list, right_list] = LoadListsFromFile("lists.txt");

        // Sorting both lists to determine the minimal distance
        std::sort(left_list.begin(), left_list.end());
        std::sort(right_list.begin(), right_list.end());

        // Part 1: Calculate total distance by pairing smallest from left_list to smallest from right_list
        const auto distance_sum = std::transform_reduce(
            left_list.begin(), left_list.end(), right_list.begin(), 0LL,
            std::plus<>{}, [](int left, int right)
            { return std::abs(left - right); });

        std::cout << COPPER_BAR_GRADIENT;
        std::cout << AMIGA_CYAN << "Total distance between lists: " << AMIGA_YELLOW << distance_sum << RESET << '\n';
        std::cout << COPPER_BAR_GRADIENT;

        // Part 2: Calculate similarity score by counting occurrences in the right list
        std::unordered_map<int, int> right_list_counts;
        for (int value : right_list)
        {
            ++right_list_counts[value];
        }

        const auto similarity_score = std::transform_reduce(
            left_list.begin(), left_list.end(), 0LL,
            std::plus<>{}, [&right_list_counts](int value)
            { return value * right_list_counts[value]; });

        std::cout << COPPER_BAR_GRADIENT;
        std::cout << AMIGA_GREEN << "Similarity score between lists: " << AMIGA_ORANGE << similarity_score << RESET << '\n';
        std::cout << COPPER_BAR_GRADIENT;
    }
    catch (const std::exception &ex)
    {
        std::cerr << AMIGA_RED << "Error: " << ex.what() << RESET << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
