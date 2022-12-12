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
    std::ifstream in("/home/kliffen/git/2022_aoc/10/input");
    std::vector<std::string> lines((LineIt(in)), LineIt());

    int X = 1;
    std::vector<int> values;

    values.push_back(X);    // cycle 0

    for (auto line : lines)
    {
        // NOOP
        values.push_back(X);
        if (line[0] == 'a')
        {
            values.push_back(X);    // Already pushed 1
            X += std::stoi(line.erase(0, 5));
        }
    }

    int sum = 0;
    for (size_t i = 20; i <= values.size(); i += 40)
    {
        std::cout << "cyle: " << i << " X: " << values[i] << '\n';
        sum += i * values[i];
    }

    std::cout << "sum: " << sum << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
