/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi/mutate.hpp"

#include <boost/algorithm/string.hpp>

namespace corefungi {

  namespace {

    static void collect(std::string const& step, corefungi::list& list, corefungi::ref_list& refs) {
      if (step.size() > 1) {
        size_t const index = boost::lexical_cast< size_t >(step.substr(1));
        refs.emplace_back(list[index]);

      } else {
        for (auto& n : list) {
          refs.emplace_back(n);
        }
      }
    }

    static void collect(std::string const& step, corefungi::dict& dict, corefungi::ref_list& refs) {
      if (step != "*") {
        if (dict.find(step) != dict.end())
          refs.emplace_back(dict.at(step));

      } else {
        for (auto& pair : dict) {
          refs.emplace_back(pair.second);
        }
      }
    }

  }

  corefungi::ref_list mutate(corefungi::node& node, std::string const& path, corefungi::mutator const mutation) {
    corefungi::ref_list        refs = { node };
    std::vector< std::string > steps;
    boost::split(steps, path, boost::is_any_of("."), boost::token_compress_on);

    for (auto const& step : steps) {
      corefungi::ref_list subrefs = {};
      for (auto& r : refs) {
        mutation(step, r, subrefs);

        try {
          if (step.front() == '#')
            corefungi::collect(step, boost::get< corefungi::list >(r), subrefs);
          else
            corefungi::collect(step, boost::get< corefungi::dict >(r), subrefs);
        } catch (boost::bad_get const&) {}
      }
      refs = std::move(subrefs);
    }

    return refs;
  }

}
