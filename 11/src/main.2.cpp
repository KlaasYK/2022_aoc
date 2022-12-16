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

enum Op
{
    PLUS,
    MULT,
    POW
};

struct Monkey
{
    int monkey;
    std::deque<int64_t> items;
    Op op;
    int64_t value;
    int64_t modulo_test;
    int if_true;
    int if_false;
    u_int64_t inspects;
};

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<detail::Line> LineIt;

    // Read input file
    // std::ifstream in(open_input_stream(argc, argv));
    // std::ifstream in("/home/kliffen/git/2022_aoc/11/test");
    // std::vector<std::string> lines((LineIt(in)), LineIt());

    // test
    // std::vector<Monkey> monkeys{
    //     Monkey{0, {79, 98}, MULT, 19, 23, 2, 3, 0},
    //     Monkey{1, {54, 65, 75, 74}, PLUS, 6, 19, 2, 0, 0},
    //     Monkey{2, {79, 60, 97}, POW, 2, 13, 1, 3, 0},
    //     Monkey{3, {74}, PLUS, 3, 17, 0, 1, 0},
    // };

    std::vector<Monkey> monkeys{
        Monkey{0, {54, 89, 94}, MULT, 7, 17, 5, 3, 0},
        Monkey{1, {66, 71}, PLUS, 4, 3, 0, 3, 0},
        Monkey{2, {76, 55, 80, 55, 55, 96, 78}, PLUS, 2, 5, 7, 4, 0},
        Monkey{3, {93, 69, 76, 66, 89, 54, 59, 94}, PLUS, 7, 7, 5, 2, 0},
        Monkey{4, {80, 54, 58, 75, 99}, MULT, 17, 11, 1, 6, 0},
        Monkey{5, {69, 70, 85, 83}, PLUS, 8, 19, 2, 7, 0},
        Monkey{6, {89}, PLUS, 6, 2, 0, 1, 0},
        Monkey{7, {62, 80, 58, 57, 93, 56}, POW, 2, 13, 6, 4, 0},
    };

    for (size_t round = 0; round != 10000; ++round)
    {
        for (Monkey &m : monkeys)
        {
            while (m.items.size() != 0)
            {
                int64_t cur = m.items.front();
                m.items.pop_front();

                // Perform op
                switch (m.op)
                {
                case PLUS:
                    cur += m.value;
                    break;
                case MULT:
                    cur *= m.value;
                    break;
                case POW:
                    cur = cur * cur;
                    break;
                }

                // Test
                // cur = cur % 96577;

                // Input
                cur = cur % 9699690;


                // TODO: use fermat's little theorem

                if (cur % m.modulo_test == 0)
                    monkeys[m.if_true].items.push_back(cur);
                else
                    monkeys[m.if_false].items.push_back(cur);

                ++m.inspects;
            }
        }
    }

    std::sort(monkeys.begin(), monkeys.end(), [](Monkey a, Monkey b)
              { return a.inspects < b.inspects; });

    // Monkey 2 inspected items 40003 times.
    // Monkey 1 inspected items 80006 times.
    // Monkey 0 inspected items 119970 times.
    // Monkey 5 inspected items 119996 times.
    // Monkey 7 inspected items 120006 times.
    // Monkey 4 inspected items 151092 times.
    // Monkey 6 inspected items 159957 times.
    // Monkey 3 inspected items 159983 times.

    // 25590400731
    for (size_t i = 0; i != monkeys.size(); ++i)
    {
        std::cout << "Monkey " << monkeys[i].monkey << " inspected items " << monkeys[i].inspects << " times.\n";
    }
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
