/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi/corefungi.hpp"

#include <boost/algorithm/string.hpp>

namespace corefungi {

  static void collect(std::string const& step, corefungi::list& list, corefungi::ref_list& nodes) {
    if (step.size() > 1) {
      size_t const index = boost::lexical_cast< size_t >(step.substr(1));
      nodes.emplace_back(list[index]);

    } else {
      for (auto& n : list) {
        nodes.emplace_back(n);
      }
    }
  }

  static void collect(std::string const& step, corefungi::dict& dict, corefungi::ref_list& nodes) {
    if (step != "*") {
      nodes.emplace_back(dict.at(step));

    } else {
      for (auto& pair : dict) {
        nodes.emplace_back(pair.second);
      }
    }
  }

  corefungi::ref_list collect(std::string const& path, corefungi::node& node) {
    corefungi::ref_list        nodes = { node };
    std::vector< std::string > steps;
    boost::split(steps, path, boost::is_any_of("."), boost::token_compress_on);

    for (auto const& step : steps) {
      corefungi::ref_list subnodes = {};
      for (auto& n : nodes) {
        try {
          if (step.front() == '#')
            corefungi::collect(step, boost::get< corefungi::list >(n), subnodes);

          else
            corefungi::collect(step, boost::get< corefungi::dict >(n), subnodes);

        } catch (boost::bad_get const&) {}
      }

      nodes = std::move(subnodes);
    }

    return nodes;
  }

  corefungi::spore get(std::string const& path, corefungi::node& node) {
    auto const& collection = corefungi::collect(path, node);

    return boost::get< corefungi::spore >(collection.empty() ? node : collection.front());
  }

}
