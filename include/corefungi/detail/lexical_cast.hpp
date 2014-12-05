#ifndef included_corefungi_detail_lexical_cast_hpp
#define included_corefungi_detail_lexical_cast_hpp

#include <boost/lexical_cast.hpp>

namespace corefungi {
namespace cfg = ::corefungi;

namespace detail {
template <typename ToT, typename FromT>
static inline ToT lexical_cast(FromT&& from) {
  return boost::lexical_cast<ToT, FromT>(std::forward<FromT>(from));
}

template <> inline bool lexical_cast<bool, std::string>(std::string&& string) {
  if (string.empty())
    return false;
  else if (string == "true")
    return true;
  else if (string == "false")
    return false;
  else
    return boost::lexical_cast<bool, std::string>(string);
}

template <> inline std::string lexical_cast<std::string, bool>(bool&& boolean) {
  return boolean ? "true" : "false";
}
}

using detail::lexical_cast;
}

#endif // ifndef included_corefungi_detail_lexical_cast_hpp
