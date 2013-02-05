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

  typedef std::vector< std::string > arguments;
  void init(std::string const& program, corefungi::arguments const& arguments);

  corefungi::ref_list collect(std::string const& path);
  corefungi::spore    get(std::string const& path);
  corefungi::spore    get(std::string const& path, corefungi::spore const& default_value);

  corefungi::ref_list collect(corefungi::node& node, std::string const& path);
  corefungi::spore    get(corefungi::node& node, std::string const& path);
  corefungi::spore    get(corefungi::node& node, std::string const& path, corefungi::spore const& default_value);

  corefungi::ref_list grow(corefungi::node& node, std::string const& path);
  void                put(corefungi::node& node, std::string const& path, corefungi::spore const& value);

  corefungi::spore_list keys(corefungi::node& node);
  corefungi::ref_list   values(corefungi::node& node);
  corefungi::ref_dict   items(corefungi::node& node);

  extern corefungi::node_ref system_sprout;
  extern corefungi::node_ref global_sprout;
  extern corefungi::node_ref local_sprout;
  extern corefungi::node_ref command_sprout;

}

#endif // ifndef __COREFUNGI_COREFUNGI_HPP__
