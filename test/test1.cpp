/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi.hpp"
#include "ostream.hpp"

#include <iostream>

int main(int argc, char const* argv[]) {
  namespace cfg = ::corefungi;

  cfg::node n;

  std::string                program = argv[0];
  std::vector< std::string > arguments;

  for (size_t i = 1; i < argc; ++i) {
    arguments.push_back(argv[i]);
  }

  cfg::init(program, arguments);
  std::clog << "command: " << cfg::command_sprout << std::endl;
  std::clog << "local: " << cfg::local_sprout << std::endl;
  std::clog << "global: " << cfg::global_sprout << std::endl;
  std::clog << "system: " << cfg::system_sprout << std::endl;

  // *INDENT-OFF*/
  n = cfg::dict {
    { "value", cfg::value { "bla" } },
    { "list-value", cfg::list { "bar", "bar" } },
    { "dict-value", cfg::dict { { "bar", "bar" } } },
    { "dict-value-2",
      cfg::dict {
        { "bar", cfg::list { "bar0", "bar1" } }
      }
    }
  };
 // *INDENT-ON*/

  std::cout << cfg::collect(n, "*.bar.#0") << std::endl;

  cfg::node m;
  std::cout << m << std::endl;

  auto sprinklings = cfg::grow(m, "foo.#.bla");
  std::cout << sprinklings << std::endl;
  for (auto& n : sprinklings) {
    n = "bla";
  }

  std::cout << m << std::endl;

  cfg::put(m, "foo.#.*", "bar");
  std::cout << m << std::endl;

  for (auto& pair : cfg::items(n)) {
    std::cout << pair.first << ": " << pair.second << std::endl;
  }

  for (auto& key : cfg::keys(n)) {
    std::cout << key << std::endl;
  }

  for (auto& value : cfg::values(n)) {
    std::cout << value << std::endl;
  }

  std::cout << n << std::endl;

  try {
    cfg::spore sp = cfg::list {
      "bar", "bar"
    };
    std::cout << sp << std::endl;
    size_t v = sp;

    std::cout << sp << ": " << v << std::endl;
  } catch (std::exception const& e) {
    std::clog << "Exception raised: " << e.what() << std::endl;
  }

  try {
    cfg::spore sp = "0";
    size_t     v  = sp;

    std::cout << sp << ": " << v << std::endl;
  } catch (std::exception const& e) {
    std::clog << "Exception raised: " << e.what() << std::endl;
  }

  return 0;
} // main
