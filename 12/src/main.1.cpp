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

struct entry
{
    vec2 pos;
    std::vector<vec2> path;
};

inline int diff(std::vector<int8_t> const &tiles, vec2 const &a, vec2 const &b, size_t const &W)
{
    return tiles[b.y * W + b.x] - tiles[a.y * W + a.x];
}

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<detail::Line> LineIt;

    // Read input file
    // std::ifstream in(open_input_stream(argc, argv));
    std::ifstream in("/home/kliffen/git/2022_aoc/12/input");
    std::vector<std::string> lines((LineIt(in)), LineIt());

    size_t const HEIGHT = lines.size();
    size_t const WIDTH = lines[0].size();
    std::vector<int8_t> tiles(WIDTH * HEIGHT, 128);
    std::vector<bool> visited(WIDTH * HEIGHT, false);

    vec2 start;
    vec2 end;

    for (int y = 0; y != HEIGHT; ++y)
        for (int x = 0; x != WIDTH; ++x)
            switch (lines[y][x])
            {
            case 'S':
                tiles[y * WIDTH + x] = 0;
                start = vec2{x, y};
                break;
            case 'E':
                tiles[y * WIDTH + x] = 25;
                end = vec2{x, y};
                break;
            default:
                tiles[y * WIDTH + x] = lines[y][x] - 'a';
                break;
            }

    std::deque<entry> q{{start, {}}};
    std::vector<vec2> final_path;
    std::vector<vec2> ops{vec2{0, 1}, vec2{1, 0}, vec2{0, -1}, vec2{-1, 0}};

    while (!q.empty())
    {
        entry cur(q.front());
        q.pop_front();

        for (auto const &op : ops)
        {
            entry n = cur;
            n.path.push_back(cur.pos);
            n.pos = cur.pos + op;

            if (n.pos.x < 0 || n.pos.y < 0 || n.pos.y == HEIGHT || n.pos.x == WIDTH || visited[n.pos.y * WIDTH + n.pos.x])
                continue; // skip

            if (diff(tiles, cur.pos, n.pos, WIDTH) < 2)
            {
                if (n.pos == end)
                {
                    final_path = cur.path;
                    break;
                }

                visited[n.pos.y * WIDTH + n.pos.x] = true;
                q.push_back(n);
            }
        }
    }

    std::cout << "Path length: " << final_path.size() + 1 << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
