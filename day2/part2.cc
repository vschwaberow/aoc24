#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <future>
#include <numeric>
#include <fstream>

bool isGradual(const std::vector<int> &levels)
{
    if (levels.size() < 2)
        return true;

    bool increasing = levels[1] > levels[0];

    for (size_t i = 1; i < levels.size(); i++)
    {
        int diff = levels[i] - levels[i - 1];
        if (std::abs(diff) < 1 || std::abs(diff) > 3)
            return false;
        if (increasing && diff < 0)
            return false;
        if (!increasing && diff > 0)
            return false;
    }
    return true;
}

bool isSafeWithDampener(const std::vector<int> &levels)
{
    if (isGradual(levels))
        return true;

    for (size_t i = 0; i < levels.size(); i++)
    {
        std::vector<int> modified = levels;
        modified.erase(modified.begin() + i);

        if (isGradual(modified))
            return true;
    }
    return false;
}

int main()
{
    std::vector<std::string> lines;
    std::string line;
    std::ifstream input("list.txt");

    if (!input.is_open())
    {
        std::cerr << "Error: Can't open the list.txt" << std::endl;
        return 1;
    }

    while (std::getline(input, line))
    {
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }

    std::cout << "Read lines: " << lines.size() << std::endl;

    auto process_line = [](const std::string &line) -> bool
    {
        std::stringstream ss(line);
        std::vector<int> levels;
        int level;
        while (ss >> level)
        {
            levels.push_back(level);
        }
        static bool first = true;
        if (first)
        {
            std::cout << "Values of first line: ";
            for (int n : levels)
            {
                std::cout << n << " ";
            }
            std::cout << std::endl;
            first = false;
        }
        return isSafeWithDampener(levels);
    };

    std::vector<std::future<bool>> futures;
    for (const auto &l : lines)
    {
        futures.push_back(std::async(std::launch::async, process_line, l));
    }

    int safe_reports = std::accumulate(futures.begin(), futures.end(), 0,
                                       [](int sum, std::future<bool> &fut)
                                       {
                                           return sum + (fut.get() ? 1 : 0);
                                       });

    std::cout << "Safe reports with Dampener: " << safe_reports << std::endl;
    return 0;
}