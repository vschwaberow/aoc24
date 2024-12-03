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

    // Check first difference to determine direction
    bool increasing = levels[1] > levels[0];

    for (size_t i = 1; i < levels.size(); i++)
    {
        int diff = levels[i] - levels[i - 1];

        // Check if difference is between 1 and 3
        if (std::abs(diff) < 1 || std::abs(diff) > 3)
            return false;
        // Check if direction matches initial direction
        if (increasing && diff < 0)
            return false;
        if (!increasing && diff > 0)
            return false;
    }
    return true;
}

bool isSafeWithDampener(const std::vector<int> &levels)
{
    // Check if the levels are already gradual
    if (isGradual(levels))
        return true;

    // Try removing each level one by one and check if the remaining levels are gradual
    for (size_t i = 0; i < levels.size(); i++)
    {
        std::vector<int> modified = levels;
        modified.erase(modified.begin() + i);

        // If removing the level makes the sequence gradual, return true
        if (isGradual(modified))
            return true;
    }
    // If no single removal makes the sequence gradual, return false
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

    // Lambda function to process each line from the input file
    auto process_line = [](const std::string &line) -> bool
    {
        std::stringstream ss(line);
        std::vector<int> levels;
        int level;
        // Read integers from the line and store them in the levels vector
        while (ss >> level)
        {
            levels.push_back(level);
        }
        // Print the values of the first line for debugging purposes
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
        // Check if the levels are safe with a dampener
        return isSafeWithDampener(levels);
    };

    // Vector to store futures for asynchronous processing of lines
    std::vector<std::future<bool>> futures;
    
    // Launch asynchronous tasks for each line in the input
    for (const auto &l : lines)
    {
        futures.push_back(std::async(std::launch::async, process_line, l));
    }

    // Accumulate the results of the futures to count the number of safe reports
    int safe_reports = std::accumulate(futures.begin(), futures.end(), 0,
                                       [](int sum, std::future<bool> &fut)
                                       {
                                           return sum + (fut.get() ? 1 : 0);
                                       });

    // Output the number of safe reports with dampener
    std::cout << "Safe reports with Dampener: " << safe_reports << std::endl;
    return 0;
}