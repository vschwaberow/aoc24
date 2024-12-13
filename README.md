# 🎄 Advent of Code 2024 C++ Solutions

## Overview
Solutions for Advent of Code 2024 challenges implemented in Modern C++. Currently solving:
- Day 1: Calibration Values Parser
- Day 2: Nuclear Reindeer Reactor Safety System
- Day 3: Snowflake Pattern Recognition
- Day 4: Elven Accounting Ledger
- Day 5: Santa's Sleigh Balancing Algorithm
- Day 6: Gift Wrapping Optimization
- Day 7: Reindeer Route Planning
- Day 8: Toy Manufacturing Quality Control
- Day 9: North Pole Weather Simulation
- Day 10: Christmas Light Synchronization
- Day 11: Cookie Recipe Analyzer

## Build Requirements
- CMake 3.26+
- C++26 capable compiler (GCC 13+ or Clang 16+)

## Quick Start

```bash
git clone https://github.com/vschwaberow/aoc24
cd aoc24
mkdir build && cd build
cmake ..
cmake --build .
```
## Some notes

Although there are CmakeLists.txt files in each day's folder, the main CMakeLists.txt file in the root directory is the one that should be used to build the project. Please make sure that you have the right
input file in the build directory before running the executable. Otherwise, the program will not be able to read the input file and will exit with an error message. I will fix this in the future by copying the input files to the build directory automatically.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

