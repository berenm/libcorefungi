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
#include <ostream>

#include "corefungi/node.hpp"
#include "corefungi/spore.hpp"

namespace corefungi {

  struct node_ref {
    public:
      node_ref() : ref(nullptr) {}
      node_ref(corefungi::node_ref const& o)            = default;
      node_ref& operator=(corefungi::node_ref const& o) = default;

      node_ref(corefungi::node& node) : ref(&node) {}
      node_ref& operator=(corefungi::node& node) { this->ref = &node; return *this; }

      template< typename T >
      node_ref& operator=(T const& t) { this->ref->operator=(t); return *this; }

      operator node&() { return *this->ref; }
      operator node const&() const { return *this->ref; }

    private:
      corefungi::node* ref;
  };

  template< typename Cr, typename Tr >
  static inline std::basic_ostream< Cr, Tr >& operator<<(std::basic_ostream< Cr, Tr >& s, corefungi::node_ref const& r) {
    s << static_cast< corefungi::node const& >(r);

    return s;
  }

  typedef std::vector< corefungi::node_ref >                          ref_list;
  typedef std::unordered_map< corefungi::spore, corefungi::node_ref > ref_dict;
  typedef corefungi::ref_dict::value_type                             ref_pair;

}

namespace boost {
  template< typename U > U const& get(corefungi::node_ref const& n) { return boost::get< U >(static_cast< corefungi::node const& >(n)); }
  template< typename U > U& get(corefungi::node_ref& n) { return boost::get< U >(static_cast< corefungi::node& >(n)); }
}

#endif // ifndef __COREFUNGI_NODE_REF_HPP__
