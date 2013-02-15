/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_HPP__
#define __COREFUNGI_HPP__

#include "corefungi/node.hpp"
#include "corefungi/node_ref.hpp"
#include "corefungi/spore.hpp"

namespace corefungi {
  namespace cfg = ::corefungi;

  typedef std::vector< std::string > arguments;
  void init(std::string const& program, cfg::arguments const& arguments);

  cfg::ref_list collect(std::string const& path);
  cfg::spore    get(std::string const& path);
  cfg::spore    get(std::string const& path, cfg::spore const& default_value);

  cfg::ref_list collect(cfg::node& node, std::string const& path);
  cfg::spore    get(cfg::node& node, std::string const& path);
  cfg::spore    get(cfg::node& node, std::string const& path, cfg::spore const& default_value);

  cfg::ref_list grow(cfg::node& node, std::string const& path);
  void          put(cfg::node& node, std::string const& path, cfg::spore const& value);

  cfg::spore_list keys(cfg::node& node);
  cfg::ref_list   values(cfg::node& node);
  cfg::ref_dict   items(cfg::node& node);

  extern cfg::node_ref system;
  extern cfg::node_ref global;
  extern cfg::node_ref local;
  extern cfg::node_ref command;

}

#endif // ifndef __COREFUNGI_HPP__
