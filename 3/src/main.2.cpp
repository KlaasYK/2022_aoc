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



int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<detail::Line> LineIt;

    // Read input file
    std::ifstream in(open_input_stream(argc, argv));
    std::vector<std::string> lines((LineIt(in)), LineIt());

    unsigned total = 0;
    for (size_t i = 0; i != lines.size(); i += 3)
    {
        std::vector<unsigned> e1(radix(lines[i]));
        std::vector<unsigned> e2(radix(lines[i + 1]));
        std::vector<unsigned> e3(radix(lines[i + 2]));

        for (size_t j = 0; j != 52; ++j)
            if (e1[j] && e2[j] && e3[j])
            {
                total += j + 1;
                break;
            }

    }
    std::cout << "Priorities part 2: " << total << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
