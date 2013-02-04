/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi/corefungi.hpp"
#include "corefungi/ostream.hpp"

#include <iostream>

int main(int argc, char const* argv[])
{
    corefungi::node n;

    n = corefungi::dict {
        { "value", corefungi::value { "bla" } },
        { "list-value", corefungi::list { "bar", "bar" } },
        { "dict-value", corefungi::dict { { "bar", "bar" } } }
    };

    std::cout << n << std::endl;

    return 0;
}
