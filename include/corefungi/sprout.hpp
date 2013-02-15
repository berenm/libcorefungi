/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_SPROUT_HPP__
#define __COREFUNGI_SPROUT_HPP__

#include <string>
#include <vector>

#include <boost/program_options.hpp>

#include "corefungi/node_ref.hpp"

namespace corefungi {
  namespace cfg = ::corefungi;
  namespace bpo = ::boost::program_options;

  struct sprout : cfg::node_ref {
    sprout(std::string const& name);

    struct option {
      option(cfg::node& node, std::string const& name, std::string const& description) : option(node, name, "", description) {}
      option(cfg::node& node, std::string const& name, std::string const& shortcut, std::string const& description);

      operator bpo::value_semantic const* () const { return bpo::value< std::string >(); }

      // name(name),
      // description(description),
      // shortcut(shortcut)
      // {
      // bpo::value< T >::operator->notifier(*this);
      // }

      std::string const name;
      std::string const description;
      std::string const shortcut;
    };
    std::string const           name;
    std::vector< option > const options;
  };

}

#endif // ifndef __COREFUNGI_SPROUT_HPP__
