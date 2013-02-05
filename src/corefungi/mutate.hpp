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

  typedef std::function< void (std::string const& step, corefungi::node_ref& r, corefungi::ref_list& refs) > mutator;

  corefungi::ref_list mutate(corefungi::node& node, std::string const& path, corefungi::mutator const mutation);

}

#endif // ifndef __COREFUNGI_MUTATE_HPP__
