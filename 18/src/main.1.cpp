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
#include <deque>
#include <set>
#include <regex>
#include <cmath>

// Borrowed from: https://stackoverflow.com/questions/2291802/is-there-a-c-iterator-that-can-iterate-over-a-file-line-by-line
namespace kyk
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

struct vec3
{
    int x;
    int y;
    int z;

    inline bool operator==(vec3 const &rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    inline vec3 operator-(vec3 const &rhs) const
    {
        return vec3{x - rhs.x, y - rhs.y, z - rhs.z};
    }

    inline vec3 operator+(vec3 const &rhs) const
    {
        return vec3{x + rhs.x, y + rhs.y, z + rhs.z};
    }
};

std::regex const COORD("(\\d+),(\\d+),(\\d+)");

// Neighbours
std::vector<vec3> const NB{
    vec3{-1, 0, 0},
    vec3{1, 0, 0},
    vec3{0, -1, 0},
    vec3{0, 1, 0},
    vec3{0, 0, -1},
    vec3{0, 0, 1},
};

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<kyk::Line> LineIt;

    // Read input file
    // std::ifstream in(open_input_stream(argc, argv));
    std::ifstream in("/home/kliffen/git/2022_aoc/18/input");
    std::vector<std::string> lines((LineIt(in)), LineIt());

    // Parse the lines
    std::vector<vec3> cubes;
    int WIDTH = 0;
    int HEIGHT = 0;
    int DEPTH = 0;

    std::smatch match;
    for (auto const &line : lines)
        if (std::regex_match(line, match, COORD))
        {
            // Offset by 1; to make sure there is space before
            int x = std::stoi(match[1]) + 1;
            int y = std::stoi(match[2]) + 1;
            int z = std::stoi(match[3]) + 1;

            // Add 2 to ensure empty space after the obsidian
            WIDTH = std::max(WIDTH, x + 2);
            HEIGHT = std::max(HEIGHT, y + 2);
            DEPTH = std::max(DEPTH, z + 2);

            cubes.push_back(vec3{x, y, z});
        }

    int XY = WIDTH * HEIGHT;
    // Construct 3D array
    std::vector<bool> volume(WIDTH * HEIGHT * DEPTH, false);
    for (auto const &cube : cubes)
        volume[cube.z * XY + cube.y * WIDTH + cube.x] = true;

    // Scan from 3 sides

    size_t count = 0;

    // Scanner
    // From XY Plane
    for (size_t x = 0; x != WIDTH; ++x)
        for (size_t y = 0; y != HEIGHT; ++y)
            for (size_t z = 1; z != DEPTH; ++z)
                if (volume[z * XY + y * WIDTH + x] != volume[(z - 1) * XY + y * WIDTH + x])
                    ++count;

    // From XZ Plane
    for (size_t x = 0; x != WIDTH; ++x)
        for (size_t z = 0; z != DEPTH; ++z)
            for (size_t y = 1; y != HEIGHT; ++y)
                if (volume[z * XY + y * WIDTH + x] != volume[z * XY + (y - 1) * WIDTH + x])
                    ++count;

    // From YZ Plane
    for (size_t y = 0; y != HEIGHT; ++y)
        for (size_t z = 0; z != DEPTH; ++z)
            for (size_t x = 1; x != WIDTH; ++x)
                if (volume[z * XY + y * WIDTH + x] != volume[z * XY + y * WIDTH + x - 1])
                    ++count;

    // Naive approach
    // for (auto const &cube : cubes)
    //     for (auto const &n : NB)
    //     {
    //         vec3 l{cube + n};
    //         if (!volume[l.z * XY + l.y * WIDTH + l.x])
    //             ++count;
    //     }

    std::cout << "Surface area: " << count << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
