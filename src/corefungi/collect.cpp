/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi/corefungi.hpp"

#include "corefungi/mutate.hpp"

#include <boost/lexical_cast.hpp>

namespace corefungi {

  corefungi::ref_list collect(std::string const& path, corefungi::node& node) {
    auto const mutation = [](std::string const& step, corefungi::node_ref& r, corefungi::ref_list& refs) {};

    return corefungi::mutate(path, node, mutation);
  }

  corefungi::spore get(std::string const& path, corefungi::node& node) {
    auto const& collection = corefungi::collect(path, node);

    return boost::get< corefungi::spore >(collection.empty() ? node : collection.front());
  }

}
