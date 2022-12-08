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
#include <set>
#include <regex>

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

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<detail::Line> LineIt;

    // Read input file
    // std::ifstream in(open_input_stream(argc, argv));
    std::ifstream in("/home/kliffen/git/2022_aoc/8/input");
    std::vector<std::string> lines((LineIt(in)), LineIt());

    size_t const HEIGHT = lines.size();
    size_t const WIDTH = lines[0].size();

    // Convert lines to single array of tree
    std::vector<u_int8_t> mat(HEIGHT * WIDTH, 0);
    for (size_t y = 0; y != HEIGHT; ++y)
        for (size_t x = 0; x != WIDTH; ++x)
            mat[y * WIDTH + x] = lines[y][x] - '0';

    // "Scan" the array from each direction
    // Store all visible in an array too
    std::vector<bool> visible(HEIGHT * WIDTH, false);

    // From left to right
    for (size_t y = 1; y != HEIGHT - 1; ++y)
    {
        int max_height = mat[y * WIDTH];
        for (size_t x = 1; x != WIDTH - 1; ++x)
        {
            if (mat[y * WIDTH + x] > max_height)
            {
                visible[y * WIDTH + x] = true;
                max_height = mat[y * WIDTH + x];
                if (max_height == 9)
                    break;
            }
        }
    }

    // From right to left
    for (size_t y = 1; y != HEIGHT - 1; ++y)
    {
        int max_height = mat[y * WIDTH + WIDTH - 1];
        for (size_t x = WIDTH - 2; x != 0; --x)
        {
            if (mat[y * WIDTH + x] > max_height)
            {
                visible[y * WIDTH + x] = true;
                max_height = mat[y * WIDTH + x];
                if (max_height == 9)
                    break;
            }
        }
    }

    // From top to bottom
    for (size_t x = 1; x != WIDTH - 1; ++x)
    {
        int max_height = mat[x];
        for (size_t y = 1; y != HEIGHT - 1; ++y)
        {
            if (mat[y * WIDTH + x] > max_height)
            {
                visible[y * WIDTH + x] = true;
                max_height = mat[y * WIDTH + x];
                if (max_height == 9)
                    break;
            }
        }
    }


    // From bottom to top
    for (size_t x = 1; x != WIDTH - 1; ++x)
    {
        int max_height = mat[(HEIGHT - 1) * WIDTH + x];
        for (size_t y = HEIGHT - 2; y != 0; --y)
        {
            if (mat[y * WIDTH + x] > max_height)
            {
                visible[y * WIDTH + x] = true;
                max_height = mat[y * WIDTH + x];
                if (max_height == 9)
                    break;
            }
        }
    }




    size_t count = 2 * WIDTH + 2 * HEIGHT - 4; // Borders
    count += std::count(visible.begin(), visible.end(), true);

    std::cout << "main.1: " << count << '\n';


    for (size_t i = 0; i != visible.size(); ++i)
    {
        std::cout << visible[i] ? 1 : 0;
        if (i % WIDTH == WIDTH-1)
            std::cout << '\n';
    }

}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
