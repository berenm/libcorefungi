/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_MUTATE_HPP__
#define __COREFUNGI_MUTATE_HPP__

#include "corefungi/node.hpp"
#include "corefungi/node_ref.hpp"

#include <string>

namespace corefungi {
  namespace cfg = ::corefungi;

  typedef std::function< void (std::string const& step, cfg::node_ref& r, cfg::ref_list& refs) > mutator;

  cfg::ref_list mutate(cfg::node& node, std::string const& path, cfg::mutator const mutation);

}

#endif // ifndef __COREFUNGI_MUTATE_HPP__
