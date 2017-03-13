#ifndef included_corefungi_detail_lexical_cast_hpp
#define included_corefungi_detail_lexical_cast_hpp

#include <boost/lexical_cast.hpp>

namespace corefungi {
namespace cfg = ::corefungi;

namespace detail {
template <typename ToT, typename FromT>
static inline ToT lexical_cast(FromT const& from) {
  return boost::lexical_cast<ToT, FromT>(std::forward<decltype(from)>(from));
}

template <>
inline bool lexical_cast<bool, std::string>(std::string const& string) {
  if (string == "true")
    return true;
  else if (string == "false")
    return false;
  throw boost::bad_lexical_cast();
}

template <>
inline std::string lexical_cast<std::string, bool>(bool const& boolean) {
  return boolean ? "true" : "false";
}
} // namespace detail

using detail::lexical_cast;
} // namespace corefungi

#endif // ifndef included_corefungi_detail_lexical_cast_hpp
