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

#include "json.hpp"

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

using namespace nlohmann;

int in_order(json const &a, json const &b, bool verbose = false)
{
    if (verbose)
        std::cout << "comparing:\n" << a.dump() << '\n' << b.dump() << "\n\n";

    // If both are numbers, check a <= b
    if (a.is_number() && b.is_number())
    {
        // The C++20 <=> would have been cool here!

        // Can't decide if they are equal
        if (a.get<int>() == b.get<int>())
            return 0;

        return a.get<int>() < b.get<int>() ? 1 : -1;
    }

    // Both are arrays; check each element
    if (a.is_array() && b.is_array())
    {
        for (size_t i = 0; i != a.size(); ++i)
        {
            if (i >= b.size() || in_order(a[i], b[i], verbose) == -1)
                return -1;

            if (in_order(a[i], b[i], verbose) == 1)
                return 1;
        }

        // If they are equal, then can not make a decision
        return a.size() == b.size() ? 0 : 1;
    }

    // Either is an integer
    if (a.is_number())
        return in_order(json::array({a.get<int>()}), b, verbose);
    else
        return in_order(a, json::array({b.get<int>()}), verbose);
}

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<kyk::Line> LineIt;

    // Read input file
    // std::ifstream in(open_input_stream(argc, argv));
    std::ifstream in("/home/kliffen/git/2022_aoc/13/input");
    std::vector<std::string> lines((LineIt(in)), LineIt());

    auto end = std::remove_if(lines.begin(), lines.end(), [](std::string const &line)
                   { return line.empty(); });

    lines.erase(end, lines.end());
    // Add the divider packets
    lines.push_back("[[2]]");
    lines.push_back("[[6]]");

    // Sort the lines
    std::sort(lines.begin(), lines.end(), [](std::string const &a, std::string const &b){
        json f(json::parse(a));
        json s(json::parse(b));
        return in_order(f, s) == 1;
    });

    // Find the indices of the divider packets
    size_t p1 = 0;
    size_t p2 = 0;

    for (size_t i = 0; i != lines.size(); ++i)
    {
        if (lines[i] == "[[2]]")
            p1 = i + 1;

        if (lines[i] == "[[6]]")
        {
            p2 = i +1;
            break;
        }
    }

    std::cout << "Decoder key: " << p1 * p2 << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
