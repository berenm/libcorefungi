/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi/corefungi.hpp"
#include "corefungi/ostream.hpp"

#include <iostream>

int main(int argc, char const* argv[]) {
  corefungi::node n;

  // *INDENT-OFF*/
  n = corefungi::dict {
    { "value", corefungi::value { "bla" } },
    { "list-value", corefungi::list { "bar", "bar" } },
    { "dict-value", corefungi::dict { { "bar", "bar" } } },
    { "dict-value-2",
      corefungi::dict {
        { "bar", corefungi::list { "bar0", "bar1" } }
      }
    }
  };
 // *INDENT-ON*/

  std::cout << corefungi::collect("*.bar.#0", n) << std::endl;

  // for (auto const& pair : corefungi::items(n)) {
  // std::cout << pair->first << ": " << pair->second << std::endl;
  // }

  // for (auto const& key : corefungi::keys(n)) {
  // std::cout << key << std::endl;
  // }

  // for (auto const& value : corefungi::values(n)) {
  // std::cout << *value << std::endl;
  // }

  // std::cout << n << std::endl;

  // corefungi::spore sp = corefungi::list {
  // "bar", "bar"
  // };
  // std::cout << sp << std::endl;
  // size_t v = sp;

  // std::cout << sp << ": " << v << std::endl;

  // corefungi::spore sp2 = "test";
  // size_t           v2  = sp2;

  return 0;
}
