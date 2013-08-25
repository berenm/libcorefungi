/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi.hpp"

#include <type_traits>

namespace corefungi {
  namespace cfg = ::corefungi;

  namespace {

    template< typename OutT >
    struct transform_result {
      typedef std::vector< OutT > type;
    };

    template< typename KeyT, typename ValueT >
    struct transform_result< std::pair< KeyT, ValueT > > {
      typedef std::unordered_map< typename std::remove_cv< KeyT >::type, ValueT > type;
    };

    /**
     * Transforms each element from the input container, using the given lambda and return a container of results.
     * The returned container is an unordered_map if the lambda returns a pair, or a vector otherwise.
     */
    template< typename InT, typename OpT, typename ValT = typename InT::value_type >
    static auto transform(InT & i, OpT && lambda)->typename transform_result< decltype(lambda(* std::begin(i))) >::type {
      typename transform_result< decltype(lambda(* std::begin(i))) >::type o;

      std::for_each(std::begin(i), std::end(i),
                    [&](ValT& v) { o.insert(o.end(), lambda(v)); });

      return o;
    }

  }

  cfg::ref_dict items(cfg::node& node) {
    return cfg::transform(boost::get< cfg::dict >(node),
                          [](cfg::pair& v) {
                            return cfg::ref_pair { v.first, cfg::node_ref { v.second } };
                          });
  }

  cfg::spore_list keys(cfg::node& node) {
    return cfg::transform(boost::get< cfg::dict >(node),
                          [](cfg::pair& v) {
                            return cfg::spore { v.first };
                          });
  }

  cfg::ref_list values(cfg::node& node) {
    return cfg::transform(boost::get< cfg::dict >(node),
                          [](cfg::pair& v) {
                            return cfg::node_ref { v.second };
                          });
  }

}
