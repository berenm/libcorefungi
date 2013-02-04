/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_COREFUNGI_HPP__
#define __COREFUNGI_COREFUNGI_HPP__

#include <string>
#include <vector>
#include <unordered_map>

#include <boost/variant.hpp>
#include <boost/mpl/at.hpp>

namespace corefungi
{

  typedef boost::make_recursive_variant< std::string,
                                         std::vector< boost::recursive_variant_ >,
                                         std::unordered_map< std::string, boost::recursive_variant_ >
                                         >::type node;
  typedef boost::mpl::at_c< node::types, 0 >::type value;
  typedef boost::mpl::at_c< node::types, 1 >::type list;
  typedef boost::mpl::at_c< node::types, 2 >::type dict;

}

#endif // ifndef __COREFUNGI_COREFUNGI_HPP__
