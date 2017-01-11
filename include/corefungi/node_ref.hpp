#ifndef __COREFUNGI_NODE_REF_HPP__
#define __COREFUNGI_NODE_REF_HPP__

#include <unordered_map>
#include <vector>

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

  template <> struct is_spore_convertible< cfg::node_ref > : std::false_type {};
  template <> struct is_spore_convertible< cfg::ref_list > : std::false_type {};
  template <> struct is_spore_convertible< cfg::ref_dict > : std::false_type {};
  template <> struct is_spore_convertible< cfg::ref_pair > : std::false_type {};

  template <>
  struct is_spore_convertible< cfg::node_cref > : std::false_type {};
  template <>
  struct is_spore_convertible< cfg::cref_list > : std::false_type {};
  template <>
  struct is_spore_convertible< cfg::cref_dict > : std::false_type {};
  template <>
  struct is_spore_convertible< cfg::cref_pair > : std::false_type {};
}

namespace boost {
  template < typename U > U const& get(corefungi::node_cref const& n) {
    return boost::get< U >(static_cast< corefungi::node const& >(n));
  }
  template < typename U > U const& get(corefungi::node_ref const& n) {
    return boost::get< U >(static_cast< corefungi::node const& >(n));
  }
  template < typename U > U& get(corefungi::node_ref& n) {
    return boost::get< U >(static_cast< corefungi::node& >(n));
  }
}

#endif // ifndef __COREFUNGI_NODE_REF_HPP__
