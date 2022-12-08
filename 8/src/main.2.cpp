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

size_t calc_score(size_t xs, size_t ys, std::vector<uint8_t> const &mat, size_t WIDTH, size_t HEIGHT)
{
    size_t H = mat[ys * WIDTH + xs];

    // Look left
    size_t left = 0;
    for (int x = xs - 1; x != -1; --x)
    {
        ++left;
        if (mat[ys * WIDTH + x] >= H)
            break;
    }

    // Look right
    size_t right = 0;
    for (int x = xs + 1; x != WIDTH; ++x)
    {
        ++right;
        if (mat[ys * WIDTH + x] >= H)
            break;
    }

    // Look up
    size_t up = 0;
    for (int y = ys - 1; y != -1; --y)
    {
        ++up;
        if (mat[y * WIDTH + xs] >= H)
            break;
    }

    // Look down
    size_t down = 0;
    for (int y = ys + 1; y != HEIGHT; ++y)
    {
        ++down;
        if (mat[y * WIDTH + xs] >= H)
            break;
    }

    return left * right * up * down;
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

    std::vector<size_t> scores(WIDTH * HEIGHT, 0);
    for (size_t y = 0; y != HEIGHT; ++y)
        for (size_t x = 0; x != WIDTH; ++x)
            scores[y * WIDTH + x] = calc_score(x, y, mat, WIDTH, HEIGHT);

    std::cout << "main.2: " << *std::max_element(scores.begin(), scores.end()) << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
