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
    std::ifstream in(open_input_stream(argc, argv));

    unsigned cur_value = 0;
    unsigned max_value = 0;
    std::string line;
    std::vector<unsigned> values;

    while (std::getline(in, line))
    {

        if (!line.empty())
            cur_value += std::stoul(line);

        // Don't loose the last elf; check for EOF
        if (line.empty() || in.peek() == std::ostream::traits_type::eof())
        {
            values.push_back(cur_value);
            max_value = std::max(max_value, cur_value);
            cur_value = 0;
        }
    }

    // Part 1
    std::cout << "Max Calories: " << max_value << '\n';

    // Part 2; reverse sort ;)
    std::sort(values.rbegin(), values.rend());
    std::cout << "Carried by top 3 elves: " << values[0] + values[1] + values[2] << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
