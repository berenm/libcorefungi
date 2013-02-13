/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi.hpp"

#include "corefungi/mutate.hpp"

#include <boost/lexical_cast.hpp>

namespace corefungi {
  namespace cfg = ::corefungi;

  namespace {

    void grow_list(std::string const& step, cfg::node& n) {
      if (!cfg::is_a< cfg::list >(n))
        n = cfg::list {};

      auto&        l    = boost::get< cfg::list >(n);
      size_t const size = step.size() > 1 ? boost::lexical_cast< size_t >(step.substr(1)) + 1 : 1;

      if (l.size() < size)
        l.resize(size);
    }

    void grow_dict(std::string const& step, cfg::node& n) {
      if (!cfg::is_a< cfg::dict >(n))
        n = cfg::dict {};

      if (step == "*")
        return;

      auto& d = boost::get< cfg::dict >(n);
      if (d.find(step) == d.end())
        d.emplace(step, cfg::node {});
    }

  }

  cfg::ref_list grow(cfg::node& node, std::string const& path) {
    auto const mutation = [](std::string const& step, cfg::node_ref& r, cfg::ref_list& refs) {
                            if (step.front() == '#')
                              cfg::grow_list(step, r);
                            else
                              cfg::grow_dict(step, r);
                          };

    return cfg::mutate(node, path, mutation);
  }

  void put(cfg::node& node, std::string const& path, cfg::spore const& value) {
    auto refs = cfg::grow(node, path);

    for (auto& r : refs) {
      r = cfg::value {
        value
      };
    }
  }

}
