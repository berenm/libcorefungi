#ifndef included_corefungi_spore_hpp
#define included_corefungi_spore_hpp

#include <string>
#include <vector>
#include <iosfwd>

#include "corefungi/detail/lexical_cast.hpp"

namespace corefungi {
namespace cfg = ::corefungi;

struct spore;

template <typename T> struct is_spore_convertible : std::true_type {};

/** no need to lexically convert from a spore to string, a spore is already a
 * std::string */
template <> struct is_spore_convertible<std::string> : std::false_type {};

/** no need to convert a spore from another spore, copy constructors are there
 */
template <> struct is_spore_convertible<cfg::spore> : std::false_type {};

/**
 * @brief opt-out string convertible structure.
 * - represent any serializable type in its string form, using
 * boost::lexical_cast, or ostream operators.
 * - parse back any deserializable type from its string form, using using
 * boost::lexical_cast, or istream operators.
 */
struct spore : std::string {
  spore()             = default;
  spore(spore const&) = default;

  explicit spore(std::string const& s) : std::string(s) {}

  template <typename T>
  using if_sporable =
      typename std::enable_if<cfg::is_spore_convertible<T>::value, void>::type;

  template <typename T, typename = if_sporable<T>>
  explicit spore(T const& t) : std::string(cfg::lexical_cast<std::string>(t)) {}
  template <typename T, typename = if_sporable<T>> operator T() const {
    return cfg::lexical_cast<T, std::string>(*this);
  }
};

template <typename Cr, typename Tr>
static inline std::basic_ostream<Cr, Tr>&
operator<<(std::basic_ostream<Cr, Tr>& s, cfg::spore const& sp) {
  s << static_cast<std::string>(sp);

  return s;
}

typedef std::vector<cfg::spore> spore_list;
}

namespace std {
template <> struct hash<corefungi::spore> : hash<string> {};
}

#endif // ifndef included_corefungi_spore_hpp
