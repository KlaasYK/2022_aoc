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

#include "lodepng.h"

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

int main()
try
{
    typedef std::istream_iterator<detail::Line> LineIt;

    // Read input file
    // std::ifstream in(open_input_stream(argc, argv));
    std::ifstream in("/home/kliffen/git/2022_aoc/8/input");
    std::vector<std::string> lines((LineIt(in)), LineIt());


    size_t const HEIGHT = lines.size();
    size_t const WIDTH = lines[0].size();

    // 40 - 170 range for greens

    // Convert lines to single array of tree
    std::vector<u_int8_t> mat(HEIGHT * WIDTH * 4, 0);
    for (size_t y = 0; y != HEIGHT; ++y)
        for (size_t x = 0; x != WIDTH; ++x)
        {
            u_int8_t val = static_cast<u_int8_t>(40 + (1.0 - (lines[y][x] - '0') / 9.0) * 130.0);
            mat[(y * WIDTH + x) * 4 + 0] = 0;       // R
            mat[(y * WIDTH + x) * 4 + 1] = val;     // G
            mat[(y * WIDTH + x) * 4 + 2] = 0;       // B
            mat[(y * WIDTH + x) * 4 + 3] = 255;     // A
        }

    lodepng::encode("trees.png", mat, WIDTH, HEIGHT);

}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
