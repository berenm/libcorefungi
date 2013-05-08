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
    template< typename InT, typename OpT, typename ValueRef = typename InT::value_type& >
    static auto transform(InT & i, OpT && lambda)->typename transform_result< decltype(lambda(* i.begin())) >::type {
      typename transform_result< decltype(lambda(* i.begin())) >::type o;

      std::for_each(i.begin(), i.end(),
                    [&](typename InT::value_type& v) {
                      o.insert(o.end(), lambda(v));
                    });

      return o;
    }

  }

  cfg::ref_dict items(cfg::node& node) {
    return cfg::transform(boost::get< cfg::dict >(node),
                          [](cfg::pair& v) -> cfg::ref_pair {
                            return std::make_pair(v.first, cfg::node_ref(v.second));
                          });
  }

  cfg::spore_list keys(cfg::node& node) {
    return cfg::transform(boost::get< cfg::dict >(node),
                          [](cfg::pair& v) -> cfg::spore {
                            return v.first;
                          });
  }

  cfg::ref_list values(cfg::node& node) {
    return cfg::transform(boost::get< cfg::dict >(node),
                          [](cfg::pair& v) -> cfg::node_ref {
                            return v.second;
                          });
  }

}
