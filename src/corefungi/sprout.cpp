/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi/sprout.hpp"

namespace corefungi {

  std::vector< std::function< bpo::options_description(bool const) > > sprout_options;

  sprout::sprout(std::string const& name) :
    name(name) {
    sprout_options.emplace_back(
      [this](bool const shorten) {
        bpo::options_description option_group(this->name + " options");

        for (auto const& option: this->options) {
          std::string name = option.name;
          std::replace(name.begin(), name.end(), '.', '-');

          if (shorten)
            option_group.add_options() ((name + "," + option.shortcut).c_str(), option, option.description.c_str());
          else
            option_group.add_options() ((this->name + "-" + name).c_str(), option, option.description.c_str());
        }

        return option_group;
      });
  }

  /*
   *    option(cfg::node& node, std::string const& name, std::string const& shortcut, std::string const& description);
   *    operator bpo::typed_value< T >*();
   *
   * template< typename T >
   * struct option {
   *  option(cfg::node& node, std::string const& key);
   *
   *  void operator()(T const& value);
   *  void operator()(std::vector< T > const& values);
   *
   *  operator bpo::typed_value< T >*();
   *  bpo::typed_value< T >* operator->();
   *
   *  std::string const name;
   *  std::string const description;
   *  std::string const shortcut;
   * };*/

}
