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

/**
 * Outcome Map
 * A X = 3
 * A Y = 6
 * A Z = 0
 *
 * B X = 0
 * B Y = 3
 * B Z = 6
 *
 * C X = 6
 * C Y = 0
 * C Z = 3
 */
std::vector<unsigned> const SCORES{3, 6, 0,
                                   0, 3, 6,
                                   6, 0, 3};

/**
 * Line score, based on checking the outcome
*/
unsigned line_score(std::string const &line)
{
    // Score for chosen shape
    unsigned score = line[2] - 'W';
    // Add the outcome
    score += SCORES[3 * (line[0] - 'A') + (line[2] - 'X')];
    return score;
}

/**
 * Shape mapping
 * A X = 3
 * A Y = 1
 * A Z = 2
 *
 * B X = 1
 * B Y = 2
 * B Z = 3
 *
 * C X = 2
 * C Y = 3
 * C Z = 1
 */

std::vector<unsigned> const SHAPES{3, 1, 2,
                                   1, 2, 3,
                                   2, 3, 1};

/**
 * Second line score, based on choosing the shape
*/
unsigned line_score2(std::string const &line)
{
    // Score based on outcome
    unsigned score = (line[2] - 'X') * 3;
    // Score based on shape
    score += SHAPES[3 * (line[0] - 'A') + (line[2] - 'X')];
    return score;
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
    unsigned total = std::reduce(score.begin(), score.end(), 0, std::plus());
    std::cout << "Score part 1: " << total << '\n';

    // Part 2
    score.clear();
    std::transform(lines.begin(), lines.end(),
                   std::back_inserter(score),
                   line_score2);

    total = std::reduce(score.begin(), score.end(), 0, std::plus());
    std::cout << "Score part 2: " << total << '\n';

}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
