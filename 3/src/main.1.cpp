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

std::vector<unsigned> radix(std::string const &compartment)
{
    std::vector<unsigned> r(52, 0);

    for (auto c : compartment)
        if (std::isupper(c))
            ++r[26 + c - 'A'];  // index; so a-z -> 0-25
        else
            ++r[c - 'a'];

    return r;
}

unsigned line_score(std::string const &line)
{
    size_t const pivot = line.size() / 2;
    std::string first(line.cbegin(), line.cbegin() + pivot);
    std::string second(line.cbegin() + pivot, line.cend());

    std::vector<unsigned> r1(radix(first));
    std::vector<unsigned> r2(radix(second));

    for (size_t i = 0; i != 52; ++i)
    {
        if (r1[i] && r2[i])
            return i + 1;   // index -> rank
    }
    throw std::runtime_error("Invalid line");
}

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<detail::Line> LineIt;

    // Read input file
    std::ifstream in(open_input_stream(argc, argv));
    std::vector<std::string> lines((LineIt(in)), LineIt());

    // Map line_score
    std::vector<unsigned> score;
    std::transform(lines.begin(), lines.end(),
                   std::back_inserter(score),
                   line_score);

    // Part 1
    unsigned total = std::accumulate(score.begin(), score.end(), 0);
    std::cout << "Priorities part 1: " << total << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
