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

  corefungi::ref_list collect(corefungi::node& node, std::string const& path) {
    auto const mutation = [](std::string const& step, corefungi::node_ref& r, corefungi::ref_list& refs) {};

    return corefungi::mutate(node, path, mutation);
  }

  corefungi::spore get(corefungi::node& node, std::string const& path) {
    corefungi::node     n;
    corefungi::ref_list collection = corefungi::collect(node, path);

    collection.emplace_back(n);

    return boost::get< corefungi::spore >(collection.front());
  }

  corefungi::spore get(corefungi::node& node, std::string const& path, corefungi::spore const& default_value) {
    corefungi::node     n          = default_value;
    corefungi::ref_list collection = corefungi::collect(node, path);

    collection.emplace_back(n);

    return boost::get< corefungi::spore >(collection.front());
  }

  corefungi::ref_list collect(std::string const& path) {
    corefungi::ref_list refs;

    for (auto& n : corefungi::ref_list { corefungi::command_sprout, corefungi::local_sprout, corefungi::global_sprout, corefungi::system_sprout }) {
      try {
        auto const collection = corefungi::collect(n, path);
        std::copy(collection.begin(), collection.end(), std::back_inserter(refs));
      } catch (boost::bad_get const&) {}
    }

    return refs;
  }

  corefungi::spore get(std::string const& path) {
    corefungi::node     n;
    corefungi::ref_list collection = corefungi::collect(path);

    collection.emplace_back(n);

    return boost::get< corefungi::spore >(collection.front());
  }

  corefungi::spore get(std::string const& path, corefungi::spore const& default_value) {
    corefungi::node     n          = default_value;
    corefungi::ref_list collection = corefungi::collect(path);

    collection.emplace_back(n);

    return boost::get< corefungi::spore >(collection.front());
  }

}
