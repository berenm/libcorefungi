/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

// 1. include the corefungi.hpp header.
#include "corefungi.hpp"

// 2. spread some sprouts in your libraries and executables.
namespace cfg = ::corefungi;
cfg::sprout const options = {
  "My library options", {
    { "my.switch",          "do something if this switch is used", cfg::bool_switch },
    { "my.integer.default", "an integer parameter with a default value",
                            cfg::of_type< size_t >(), cfg::default_ = 5 },
    { "my.integers",        "another integer parameter with multiple values",
                            cfg::of_type< std::vector< size_t > >(), cfg::composing = true,
                            cfg::long_name = "integer", cfg::short_name = "i" }
  }
};

int main(int argc, char const* argv[]) {
  // 3. fertilize the fungi from the command-line.
  cfg::init(argc, argv);

  // 4. collect the spores and cook them to your desired types.
  size_t default_integer = cfg::get("my.integer.default");
  std::cout << "my.integer.default is: " << default_integer << std::endl;

  std::vector< size_t > integers;
  auto const&           nodes = cfg::collect("my.integers.#");

  for (auto const& node : nodes) {
    size_t const integer = boost::get< cfg::spore >(node);
    integers.emplace_back(integer);
  }

  std::cout << "my.integers are: " << integers << std::endl;
}
