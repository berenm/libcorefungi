/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi/sprout.hpp"
#include "corefungi/option.hpp"

namespace corefungi {

  void sprouts::add(cfg::mold&& m) {
    sprouts::get_instance().builders.emplace_back(
      [ = ]() {
        bpo::options_description option_group(m.first);

        for (auto const& option: m.second) {
          std::string name = option.long_name;
          if (option.short_name != "")
            name += "," + option.short_name;

          option_group.add_options() (name.c_str(), new cfg::option(option), option.option_description.c_str());
        }

        return option_group;
      });
  }

  void sprouts::build(bpo::options_description& global) {
    for (auto const& build_option : sprouts::get_instance().builders) {
      global.add(build_option());
    }
  }

}
