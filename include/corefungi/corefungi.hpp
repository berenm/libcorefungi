/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_COREFUNGI_HPP__
#define __COREFUNGI_COREFUNGI_HPP__

#include "corefungi/node.hpp"
#include "corefungi/node_ref.hpp"
#include "corefungi/spore.hpp"

namespace corefungi {
  namespace cfg = ::corefungi;

  extern corefungi::node root;

  corefungi::spore    get(std::string const& path, corefungi::node& node=root);
  corefungi::ref_list collect(std::string const& path, corefungi::node& node=root);

  corefungi::spore_list keys(corefungi::node& node=root);
  corefungi::ref_list   values(corefungi::node& node=root);
  corefungi::ref_dict   items(corefungi::node& node=root);

}

#endif // ifndef __COREFUNGI_COREFUNGI_HPP__
