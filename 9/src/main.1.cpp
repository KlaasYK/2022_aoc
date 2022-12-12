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

// Operations map
std::vector<vec2> OPS{
    vec2{-1, -1},
    vec2{-1, -1},
    vec2{0, -1},
    vec2{1, -1},
    vec2{1, -1},
    vec2{-1, -1},
    vec2{0, 0},
    vec2{0, 0},
    vec2{0, 0},
    vec2{1, -1},
    vec2{-1, 0},
    vec2{0, 0},
    vec2{0, 0},
    vec2{0, 0},
    vec2{1, 0},
    vec2{-1, 1},
    vec2{0, 0},
    vec2{0, 0},
    vec2{0, 0},
    vec2{1, 1},
    vec2{-1, 1},
    vec2{-1, 1},
    vec2{0, 1},
    vec2{1, 1},
    vec2{1, 1},
};

void update_pos(vec2 const &h, vec2 &t)
{
    vec2 x{h - t};      // determine relative pos
    x = x + vec2{2, 2}; // offset
    t = t + OPS[x.y * 5 + x.x];
}

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<detail::Line> LineIt;

    // Read input file
    // std::ifstream in(open_input_stream(argc, argv));
    std::ifstream in("/home/kliffen/git/2022_aoc/9/input");
    std::vector<std::string> lines((LineIt(in)), LineIt());

    std::vector<vec2> rope{2, vec2{0, 0}};
    std::vector<vec2> locs{vec2{0, 0}};

    for (auto &line : lines)
    {
        vec2 op{0, 0};

        switch (line[0])
        {
        case 'U':
            op = vec2{0, 1};
            break;
        case 'D':
            op = vec2{0, -1};
            break;
        case 'L':
            op = vec2{-1, 0};
            break;
        case 'R':
            op = vec2{1, 0};
            break;
        }

        // determine count;
        line.erase(0, 2);
        size_t count = stoul(line);

        for (size_t c = 0; c != count; c++)
        {
            // Move the head
            rope[0] = rope[0] + op;

            // Update the rest of the rope
            for (size_t i = 1; i != rope.size(); ++i)
                update_pos(rope[i - 1], rope[i]);

            // Keep track of the rope tail
            // FIXME: don't use this horrible inefficient find unique :')
            if (std::find(locs.begin(), locs.end(), rope.back()) == locs.end())
                locs.push_back(rope.back());
        }
    }

    // 6625 (too high!)
    // 6464 was correct
    std::cout << "Unique tail locations: " << locs.size() << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
