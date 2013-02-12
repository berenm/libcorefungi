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

  namespace {

    void grow_list(std::string const& step, corefungi::node_ref& r) {
      if (!corefungi::is_a< corefungi::list >(r))
        r = corefungi::list {};

      auto&        l    = boost::get< corefungi::list >(r);
      size_t const size = step.size() > 1 ? boost::lexical_cast< size_t >(step.substr(1)) + 1 : 1;

      if (l.size() < size)
        l.resize(size);
    }

    void grow_dict(std::string const& step, corefungi::node_ref& r) {
      if (!corefungi::is_a< corefungi::dict >(r))
        r = corefungi::dict {};

      if (step == "*")
        return;

      auto& d = boost::get< corefungi::dict >(r);
      if (d.find(step) == d.end())
        d.emplace(step, corefungi::node {});
    }

  }

  corefungi::ref_list grow(corefungi::node& node, std::string const& path) {
    auto const mutation = [](std::string const& step, corefungi::node_ref& r, corefungi::ref_list& refs) {
                            if (step.front() == '#')
                              corefungi::grow_list(step, r);
                            else
                              corefungi::grow_dict(step, r);
                          };

    return corefungi::mutate(node, path, mutation);
  }

  void put(corefungi::node& node, std::string const& path, corefungi::spore const& value) {
    auto refs = corefungi::grow(node, path);

    for (auto& r : refs) {
      r = corefungi::value {
        value
      };
    }
  }

}
