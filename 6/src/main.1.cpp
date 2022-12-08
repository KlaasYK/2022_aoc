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
    std::ifstream in("/home/kliffen/git/2022_aoc/6/input");
    std::vector<std::string> lines((LineIt(in)), LineIt());

    std::string const line(lines[0]);

    auto it = line.begin();

    for (size_t i = 0; i != line.length() - 4; ++i)
    {
        std::set<char> chars(it + i, it + i + 4);

        if (chars.size() == 4)
        {
            std::cout << i + 4 << '\n';
            break;
        }
    }

}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
