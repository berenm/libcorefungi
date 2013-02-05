/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_SPORE_HPP__
#define __COREFUNGI_SPORE_HPP__

#include <string>
#include <vector>
#include <ostream>

#include <boost/lexical_cast.hpp>

namespace corefungi {

  struct spore : std::string {
    spore()             = default;
    spore(spore const&) = default;

    spore(char const* s) :        std::string(s) {}
    spore(std::string const& s) : std::string(s) {}

    template< typename T > explicit spore(T const& t) : std::string(boost::lexical_cast< std::string >(t)) {}
    template< typename T > operator T() const { return boost::lexical_cast< T >(*this); }
  };

  template< typename Cr, typename Tr >
  static inline std::basic_ostream< Cr, Tr >& operator<<(std::basic_ostream< Cr, Tr >& s, corefungi::spore const& sp) {
    s << static_cast< std::string >(sp);

    return s;
  }

  typedef std::vector< corefungi::spore > spore_list;

}

namespace std {
  template< > struct hash< corefungi::spore > : hash< string > {};
}

#endif // ifndef __COREFUNGI_SPORE_HPP__
