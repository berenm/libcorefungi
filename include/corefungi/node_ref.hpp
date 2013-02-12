/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_NODE_REF_HPP__
#define __COREFUNGI_NODE_REF_HPP__

#include <vector>
#include <unordered_map>

#include "corefungi/node.hpp"
#include "corefungi/spore.hpp"

#include "corefungi/detail/reference.hpp"

namespace corefungi {
  namespace cfg = ::corefungi;

  typedef detail::reference< cfg::node >                  node_ref;
  typedef std::vector< cfg::node_ref >                    ref_list;
  typedef std::unordered_map< cfg::spore, cfg::node_ref > ref_dict;
  typedef cfg::ref_dict::value_type                       ref_pair;

  typedef detail::reference< cfg::node const >             node_cref;
  typedef std::vector< cfg::node_cref >                    cref_list;
  typedef std::unordered_map< cfg::spore, cfg::node_cref > cref_dict;
  typedef cfg::cref_dict::value_type                       cref_pair;

}

namespace boost {
  template< typename U > U const& get(corefungi::node_cref const& n) { return boost::get< U >(static_cast< corefungi::node const& >(n)); }
  template< typename U > U const& get(corefungi::node_ref const& n) { return boost::get< U >(static_cast< corefungi::node const& >(n)); }
  template< typename U > U& get(corefungi::node_ref& n) { return boost::get< U >(static_cast< corefungi::node& >(n)); }
}

#endif // ifndef __COREFUNGI_NODE_REF_HPP__
