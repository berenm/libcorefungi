/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi.hpp"

#include <type_traits>

namespace corefungi {

  namespace {

    template< typename OutT, typename InT >
    static std::vector< OutT > transform(InT& i, std::function< OutT(typename InT::value_type&) > const op) {
      std::vector< OutT > o;
      std::for_each(i.begin(), i.end(),
                    [&](typename InT::value_type& v) {
                      o.emplace_back(op(v));
                    });

      return o;
    }

    template< typename OutK, typename OutV, typename InT >
    static std::unordered_map< typename std::remove_cv< OutK >::type, OutV > transform(InT& i, std::function< std::pair< OutK, OutV >(typename InT::value_type&) > const op) {
      std::unordered_map< typename std::remove_cv< OutK >::type, OutV > o;
      std::for_each(i.begin(), i.end(),
                    [&](typename InT::value_type& v) {
                      o.emplace(op(v));
                    });

      return o;
    }

  }

  corefungi::ref_dict items(corefungi::node& node) {
    return corefungi::transform(boost::get< corefungi::dict >(node),
                                std::function< corefungi::ref_pair(corefungi::dict::value_type&) > {
                                  [](corefungi::dict::value_type& v) -> corefungi::ref_pair {
                                    return std::make_pair(v.first, corefungi::node_ref(v.second));
                                  }
                                });
  }

  corefungi::spore_list keys(corefungi::node& node) {
    return corefungi::transform(boost::get< corefungi::dict >(node),
                                std::function< corefungi::spore(corefungi::dict::value_type&) > {
                                  [](corefungi::dict::value_type& v) -> corefungi::spore {
                                    return v.first;
                                  }
                                });
  }

  corefungi::ref_list values(corefungi::node& node) {
    return corefungi::transform(boost::get< corefungi::dict >(node),
                                std::function< corefungi::node_ref(corefungi::dict::value_type&) > {
                                  [](corefungi::dict::value_type& v) -> corefungi::node_ref {
                                    return v.second;
                                  }
                                });
  }

}
