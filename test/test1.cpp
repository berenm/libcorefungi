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

  std::string                program = argv[0];
  std::vector< std::string > arguments;

  for (size_t i = 1; i < argc; ++i) {
    arguments.push_back(argv[i]);
  }

  corefungi::init(program, arguments);
  std::clog << corefungi::root << std::endl;

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

  corefungi::node m;
  std::cout << m << std::endl;

  auto sprinklings = corefungi::grow("foo.#.bla", m);
  std::cout << sprinklings << std::endl;
  for (auto& n : sprinklings) {
    n = "bla";
  }

  std::cout << m << std::endl;

  corefungi::put("foo.#.*", "bar", m);
  std::cout << m << std::endl;

  for (auto& pair : corefungi::items(n)) {
  std::cout << pair.first << ": " << pair.second << std::endl;
  }

  for (auto& key : corefungi::keys(n)) {
  std::cout << key << std::endl;
  }

  for (auto& value : corefungi::values(n)) {
  std::cout << value << std::endl;
  }

  std::cout << n << std::endl;

  try {
    corefungi::spore sp = corefungi::list {
    "bar", "bar"
    };
    std::cout << sp << std::endl;
    size_t v = sp;

    std::cout << sp << ": " << v << std::endl;
  } catch(std::exception const& e) {
    std::clog << "Exception raised: " << e.what() << std::endl;
  }

  try {
    corefungi::spore sp = "0";
    size_t           v  = sp;

    std::cout << sp << ": " << v << std::endl;
  } catch(std::exception const& e) {
    std::clog << "Exception raised: " << e.what() << std::endl;
  }

  return 0;
} // main
