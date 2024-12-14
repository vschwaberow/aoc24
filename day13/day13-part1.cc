/**
 * @file day13-part1.cc
 * @brief Solution for Advent of Code 2024 Day 13 Part 1
 *
 * SPDX-License-Identifier: MIT
 *
 * @author Volker Schwaberow <volker@schwaberow.de>
 * @date 14.12.2024
 */

#include <iostream>
#include <vector>
#include <optional>
#include <limits>
#include <tuple>
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <print>

struct Button {
  int x_increment;
  int y_increment;
  int cost;
};

struct Prize {
  int x;
  int y;
};

struct Machine {
  Button button_a;
  Button button_b;
  Prize prize;
};

struct Solution {
  int tokens;
  int a_presses;
  int b_presses;
};

std::optional<Solution> SolveMachine(const Machine& machine, int max_presses) {
  const auto& [ax, ay, cost_a] = machine.button_a;
  const auto& [bx, by, cost_b] = machine.button_b;
  const auto& [px, py] = machine.prize;

  int min_tokens = std::numeric_limits<int>::max();
  std::optional<Solution> best_solution;

  if (bx == 0 || by == 0) {
    std::print("Skipping machine due to zero increment values in button B.\n");
    return std::nullopt;
  }

  int gcd_x = std::gcd(ax, bx);
  int gcd_y = std::gcd(ay, by);

  if (px % gcd_x != 0 || py % gcd_y != 0) {
    std::print("Prize ({}, {}) not reachable due to GCD constraints: gcd_x={}, gcd_y={}.\n", px, py, gcd_x, gcd_y);
    return std::nullopt;
  }

  std::print("Processing machine with Prize ({}, {}).\n", px, py);

  for (int a_presses = 0; a_presses <= max_presses; ++a_presses) {
    int remaining_x = px - a_presses * ax;
    int remaining_y = py - a_presses * ay;

    if (remaining_x % bx == 0 && remaining_y % by == 0) {
      int b_presses = remaining_x / bx;
      if (b_presses >= 0 && b_presses <= max_presses && remaining_y == b_presses * by) {
        int tokens = a_presses * cost_a + b_presses * cost_b;
        if (tokens < min_tokens) {
          min_tokens = tokens;
          best_solution = Solution{tokens, a_presses, b_presses};
        }
      }
    }
  }

  if (best_solution) {
    std::print("Solution found with A presses: {}, B presses: {}, Tokens: {}\n",
               best_solution->a_presses, best_solution->b_presses, best_solution->tokens);
  } else {
    std::print("No solution found for this machine.\n");
  }

  return best_solution;
}

std::vector<Machine> LoadMachinesFromFile(const std::string& filename) {
  std::ifstream file(filename);
  std::vector<Machine> machines;

  if (!file.is_open()) {
    std::print("Error: Could not open file {}\n", filename);
    return machines;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.rfind("Button A:", 0) == 0) {
      Machine machine;

      int ax, ay;
      sscanf(line.c_str(), "Button A: X+%d, Y+%d", &ax, &ay);
      machine.button_a = {ax, ay, 3};

      std::getline(file, line);
      int bx, by;
      sscanf(line.c_str(), "Button B: X+%d, Y+%d", &bx, &by);
      machine.button_b = {bx, by, 1};

      std::getline(file, line);
      int px, py;
      sscanf(line.c_str(), "Prize: X=%d, Y=%d", &px, &py);
      machine.prize = {px, py};

      machines.push_back(machine);
    }
  }

  return machines;
}

int main() {
  const int max_presses = 1000;
  const std::string input_file = "input.txt";

  std::vector<Machine> machines = LoadMachinesFromFile(input_file);
  if (machines.empty()) {
    std::print("No machines loaded from {}.\n", input_file);
    return 1;
  }

  int total_tokens = 0;
  int prizes_won = 0;

  for (size_t i = 0; i < machines.size(); ++i) {
    auto result = SolveMachine(machines[i], max_presses);
    if (result) {
      std::print("Machine {}: Prize won with {} A presses, {} B presses for {} tokens.\n",
                 i + 1, result->a_presses, result->b_presses, result->tokens);
      total_tokens += result->tokens;
      ++prizes_won;
    } else {
      std::print("Machine {}: No solution.\n", i + 1);
    }
  }

  std::print("\nTotal prizes won: {}\nTotal tokens spent: {}\n", prizes_won, total_tokens);
  return 0;
}
