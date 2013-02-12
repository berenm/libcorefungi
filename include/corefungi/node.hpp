/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_NODE_HPP__
#define __COREFUNGI_NODE_HPP__

#include <vector>
#include <unordered_map>

#include <boost/variant.hpp>
#include <boost/mpl/at.hpp>

#include "corefungi/spore.hpp"

namespace corefungi {
  namespace cfg = ::corefungi;

  typedef boost::make_recursive_variant< cfg::spore,
                                         std::vector< boost::recursive_variant_ >,
                                         std::unordered_map< cfg::spore, boost::recursive_variant_ >
                                         >::type node;
  typedef boost::mpl::at_c< node::types, 0 >::type value;
  typedef boost::mpl::at_c< node::types, 1 >::type list;
  typedef boost::mpl::at_c< node::types, 2 >::type dict;
  typedef cfg::dict::value_type                    pair;

  /** forbid conversion from spore to node, as a node is already constructible from a spore */
  template< > struct is_spore_convertible< cfg::node > : std::false_type {};

  template< typename T > static inline bool is_a(cfg::node const& n) { return n.type() == typeid(T); }

}

#endif // ifndef __COREFUNGI_NODE_HPP__
