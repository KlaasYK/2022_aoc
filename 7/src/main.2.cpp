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

struct Node
{
    std::string name;
    size_t size;
    std::vector<Node> children;
    Node *parent;
};

// Prepare the regex
std::regex const cd_root("\\$ cd /");
std::regex const cd_parent("\\$ cd ..");
std::regex const cd_dir("\\$ cd (.*)");
std::regex const dir_line("dir (.*)");
std::regex const file_line("(\\d+) (.*)");

inline bool is_dir(Node const &node)
{
    return node.children.size() != 0;
}

size_t calculate_dir_size(Node &node)
{
    size_t total = 0;
    for (Node &c : node.children)
    {
        if (is_dir(c))
            total += calculate_dir_size(c);
        else
            total += c.size; // might catch empty directory
    }
    node.size = total; // cache the size
    return total;
}

size_t find_smallest(Node const &node, size_t to_free, size_t smallest = std::numeric_limits<size_t>::max())
{
    // This dir is too small; ignore
    if (node.size < to_free)
        return std::numeric_limits<size_t>::max();

    // Check yourself
    size_t s = std::min(smallest, node.size);

    // Check all child directories;
    for (auto const &c : node.children)
        if (is_dir(c))
            s = std::min(s, find_smallest(c, to_free, s));

    return s;
}

int main(int argc, char *argv[])
try
{
    typedef std::istream_iterator<detail::Line> LineIt;

    // Read input file
    // std::ifstream in(open_input_stream(argc, argv));
    std::ifstream in("/home/kliffen/git/2022_aoc/7/input");
    std::vector<std::string> lines((LineIt(in)), LineIt());

    // Construct the Filesystem
    Node root{"/", 0, std::vector<Node>(), nullptr};

    // Execute the commands
    Node *cwd = nullptr;
    for (auto const &line : lines)
    {
        std::smatch match;

        // $ cd /
        if (std::regex_match(line, cd_root))
        {
            cwd = &root;
            continue;
        }

        if (std::regex_match(line, cd_parent))
        {
            cwd = cwd->parent;
            continue;
        }

        // $ cd {dir}
        if (std::regex_match(line, match, cd_dir))
        {
            for (size_t i = 0; i != cwd->children.size(); ++i)

                if (cwd->children[i].name == match[1])
                {
                    cwd = &cwd->children[i];
                    break;
                }
            continue;
        }

        // Add directory to cwd
        if (std::regex_match(line, match, dir_line))
        {
            cwd->children.push_back(Node{match[1], 0, std::vector<Node>(), cwd});
            continue;
        }

        if (std::regex_match(line, match, file_line))
        {
            cwd->children.push_back(Node{match[2], std::stoull(match[1]), std::vector<Node>(), cwd});
            continue;
        }

        // $ ls is a noop
    }

    // Tree constructed

    size_t const total_size = calculate_dir_size(root);
    std::cout << "total_size: " << total_size << '\n';

    size_t const device_size = 70000000;
    size_t const update_size = 30000000;
    size_t const to_free = update_size - (device_size - total_size);

    std::cout << "to_free: " << to_free << '\n';

    // device_size:
    // total_size:  41609574
    // max_size:     1778099

    size_t max_size = find_smallest(root, to_free);
    std::cout << "smallest: " << max_size << '\n';
}
catch (std::exception const &ex)
{
    std::cerr << ex.what() << '\n';
    return 1;
}
