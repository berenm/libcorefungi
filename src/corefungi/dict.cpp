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

  cfg::ref_dict items(cfg::node& node) {
    return cfg::transform(boost::get< cfg::dict >(node),
                          std::function< cfg::ref_pair(cfg::dict::value_type&) > {
                            [](cfg::dict::value_type& v) -> cfg::ref_pair {
                              return std::make_pair(v.first, cfg::node_ref(v.second));
                            }
                          });
  }

  cfg::spore_list keys(cfg::node& node) {
    return cfg::transform(boost::get< cfg::dict >(node),
                          std::function< cfg::spore(cfg::dict::value_type&) > {
                            [](cfg::dict::value_type& v) -> cfg::spore {
                              return v.first;
                            }
                          });
  }

  cfg::ref_list values(cfg::node& node) {
    return cfg::transform(boost::get< cfg::dict >(node),
                          std::function< cfg::node_ref(cfg::dict::value_type&) > {
                            [](cfg::dict::value_type& v) -> cfg::node_ref {
                              return v.second;
                            }
                          });
  }

}
