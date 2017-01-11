#ifndef __COREFUNGI_DETAIL_LEXICAL_CAST_HPP__
#define __COREFUNGI_DETAIL_LEXICAL_CAST_HPP__

#include <boost/lexical_cast.hpp>

namespace corefungi {
namespace cfg = ::corefungi;

namespace detail {

template <typename ToT, typename FromT>
static inline ToT lexical_cast(FromT const& from) {
  return boost::lexical_cast<ToT, FromT>(from);
}

template <>
inline bool lexical_cast<bool, std::string>(std::string const& string) {
  return string == "true"
             ? true
             : string == "false"
                   ? false
                   : boost::lexical_cast<bool, std::string>(string);
}

template <>
inline std::string lexical_cast<std::string, bool>(bool const& boolean) {
  return std::string{boolean ? "true" : "false"};
}
}

using detail::lexical_cast;
}

#endif // ifndef __COREFUNGI_DETAIL_LEXICAL_CAST_HPP__
