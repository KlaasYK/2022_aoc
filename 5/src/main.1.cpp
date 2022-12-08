// Copyright 2022 Klaas Kliffen
// SPDX-License-Identifier: Apache-2.0

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <algorithm>
#include <vector>
#include <functional>
#include <iterator>
#include <numeric>
#include <stack>

// Borrowed from: https://stackoverflow.com/questions/2291802/is-there-a-c-iterator-that-can-iterate-over-a-file-line-by-line
namespace detail
{
    class Line : public std::string
    {
        // TODO: check C++ friends part again
        friend std::istream &operator>>(std::istream &is, Line &line)
        {
            return std::getline(is, line);
        }
    };

}

std::ifstream open_input_stream(int argc, char *argv[])
{
    if (argc < 2)
    {
        throw std::runtime_error("No input file provided");
    }
    return std::ifstream(argv[1]);
}

size_t const MAX_STACK = 8;  // 8, 3
size_t const NUM_STACKS = 9; // 9, 3

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<detail::Line> LineIt;

    // Read input file
    std::ifstream in(open_input_stream(argc, argv));
    std::vector<std::string> lines((LineIt(in)), LineIt());

    // Hardcoded stacks
    std::vector<std::stack<char>> stacks(NUM_STACKS, std::stack<char>());

    // Read in the stacks
    for (int i = 0; i != MAX_STACK; ++i)
        for (size_t j = 0; j != NUM_STACKS; ++j)
            if (lines[MAX_STACK - i - 1].length() > (1 + j * 4) && lines[MAX_STACK - i - 1][1 + j * 4] != ' ')
                stacks[j].push(lines[MAX_STACK - i - 1][1 + j * 4]);

    // Perform the operations
    for (size_t i = MAX_STACK + 2; i != lines.size(); ++i)
    {
        // Erase from
        lines[i].erase(0, 5);
        size_t length;
        size_t n_containers = std::stoul(lines[i], &length);
        lines[i].erase(0, 6 + length);
        size_t from_stack = std::stoul(lines[i], &length) - 1;
        lines[i].erase(0, 4 + length);
        size_t to_stack = std::stoul(lines[i]) - 1;

        for (size_t j = 0; j != n_containers; ++j)
        {
            char c = stacks[from_stack].top();
            stacks[from_stack].pop();
            stacks[to_stack].push(c);
        }
    }

    for (auto stack : stacks)
        std::cout << stack.top();
    std::cout << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
