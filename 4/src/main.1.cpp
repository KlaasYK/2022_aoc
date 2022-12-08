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

struct Range
{
    unsigned min;
    unsigned max;
};

class Ranges
{
    Range left;
    Range right;

public:
    Ranges(std::string const &line);
    bool contained() const;

private:
    bool is_range_contained_by(Range a, Range b) const;
};

// Parses Ranges from line
Ranges::Ranges(std::string const &line)
{
    size_t comma = line.find(',');
    std::string const left_r = line.substr(0, comma);
    std::string const right_r = line.substr(comma + 1);

    // Left
    size_t dash = left_r.find('-');
    left.min = std::stoull(left_r.substr(0, dash));
    left.max = std::stoull(left_r.substr(dash + 1));

    dash = right_r.find('-');
    right.min = std::stoull(right_r.substr(0, dash));
    right.max = std::stoull(right_r.substr(dash + 1));
}

bool Ranges::contained() const
{
    return is_range_contained_by(left, right) || is_range_contained_by(right, left);
}

bool Ranges::is_range_contained_by(Range a, Range b) const
{
    return a.min >= b.min || a.max <= b.max;
}

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<detail::Line> LineIt;

    // Read input file
    std::ifstream in(open_input_stream(argc, argv));
    std::vector<std::string> lines((LineIt(in)), LineIt());

    std::vector<bool> overlaps;
    std::transform(
        lines.begin(),
        lines.end(),
        std::back_inserter(overlaps),
        [](std::string const &line)
        {
            Ranges const ranges(line);
            return ranges.contained();
        });


    size_t count = std::count(overlaps.begin(), overlaps.end(), true);
    std::cout << "Ranges contained: " << count << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
