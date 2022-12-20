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

struct vec2
{
    int x;
    int y;

    inline bool operator==(vec2 const &rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    inline vec2 operator-(vec2 const &rhs) const
    {
        return vec2{x - rhs.x, y - rhs.y};
    }

    inline vec2 operator+(vec2 const &rhs) const
    {
        return vec2{x + rhs.x, y + rhs.y};
    }
};

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<kyk::Line> LineIt;

    // Read input file
    // std::ifstream in(open_input_stream(argc, argv));
    std::ifstream in("/home/kliffen/git/2022_aoc/14/input");
    std::vector<std::string> lines((LineIt(in)), LineIt());

    std::regex coord("(\\d+),(\\d+)");

    // Parse the lines
    std::vector<std::vector<vec2>> rocks;
    int X_MAX = 0;
    int Y_MAX = 0;
    for (auto const &line : lines)
    {
        std::vector<vec2> v;
        auto it = std::sregex_iterator(line.begin(), line.end(), coord);
        auto end = std::sregex_iterator();
        for (it; it != end; ++it)
        {
            int x = std::stoi((*it)[1]);
            int y = std::stoi((*it)[2]);
            X_MAX = std::max(X_MAX, x + 1);
            Y_MAX = std::max(Y_MAX, y + 1);
            v.push_back(vec2{x, y});
        }
        rocks.push_back(v);
    }

    std::cout << "W: " << X_MAX << " H: " << Y_MAX << '\n';
    std::vector<bool> map(X_MAX * Y_MAX, false);
    // Construct the map

    for (auto const &rock : rocks)
        for (size_t i = 1; i != rock.size(); ++i)
        {
            vec2 diff(rock[i] - rock[i - 1]);
            size_t y_min = std::min(rock[i].y, rock[i - 1].y);
            size_t y_max = std::max(rock[i].y, rock[i - 1].y);
            size_t x_min = std::min(rock[i].x, rock[i - 1].x);
            size_t x_max = std::max(rock[i].x, rock[i - 1].x);

            // Vertical line
            if (diff.x == 0)
            {
                for (size_t y = y_min; y <= y_max; ++y)
                {
                    map[y * X_MAX + x_min] = true;
                }
            }
            else
            { // Horizontal line
                for (size_t x = x_min; x <= x_max; ++x)
                {
                    map[y_min * X_MAX + x] = true;
                }
            }
        }

    size_t rounds = 0;

    while(true)
    {
        ++rounds;
        vec2 c{500, 0};
        bool can_move = true;
        bool abyss = false;
        while (can_move)
        {
            // Drop down as far as possible
            while (!map[(c.y + 1) * X_MAX + c.x] && (c.y + 1) < Y_MAX)
                ++c.y;

            if (c.y >= Y_MAX)
            {
                abyss = true;
                break;
            }

            // Try left, then right, otherwise settled
            if (!map[(c.y + 1) * X_MAX + c.x - 1])
                c = c + vec2{-1, 1};
            else if (!map[(c.y + 1) * X_MAX + c.x + 1])
                c = c + vec2{1, 1};
            else
            {
                map[c.y * X_MAX + c.x] = true;
                can_move = false;
            }
        }

        if (abyss)
            break;
    }


    for (size_t y = 0; y != Y_MAX; ++y)
    {
        for (size_t x = 0; x != X_MAX; ++x)
            if (y == 0 && x == 500)
                std::cout << '+';
            else
                std::cout << (map[y * X_MAX + x] ? '#' : '.');
        std::cout << '\n';
    }

    std::cout << "Units: " << (rounds - 1 ) << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
