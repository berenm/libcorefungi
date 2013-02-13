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

    static void extend(cfg::node& node, size_t const size) {
      if (!cfg::is_a< cfg::list >(node))
        node = cfg::list {};

      auto& l = boost::get< cfg::list >(node);
      if (l.size() < size + 1)
        l.resize(size + 1);
    }

    static void extend(cfg::node& node, std::string const& step) {
      if (!cfg::is_a< cfg::dict >(node))
        node = cfg::dict {};

      if (step == "*")
        return;

      auto& d = boost::get< cfg::dict >(node);
      if (d.find(step) == d.end())
        d.emplace(step, cfg::node {});
    }

  }

  cfg::ref_list grow(cfg::node& node, std::string const& path) {
    auto const mutation = [](cfg::node& node, std::string const& step) {
                            if (step.front() == '#')
                              cfg::extend(node, boost::lexical_cast< size_t >(step.size() == 1 ? "0" : step.substr(1)));
                            else
                              cfg::extend(node, step);
                          };

    return cfg::mutate(node, path, mutation);
  }

  void put(cfg::node& node, std::string const& path, cfg::spore const& value) {
    auto refs = cfg::grow(node, path);

    for (auto& r : refs) {
      r = cfg::value(value);
    }
  }

}
