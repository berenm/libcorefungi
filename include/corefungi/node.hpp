#ifndef included_corefungi_node_hpp
#define included_corefungi_node_hpp

#include <vector>
#include <unordered_map>

#include <boost/variant.hpp>
#include <boost/mpl/at.hpp>

#include "corefungi/spore.hpp"

namespace corefungi {
namespace cfg = ::corefungi;

struct nothing {};

typedef typename boost::make_recursive_variant<
    cfg::spore, std::vector<boost::recursive_variant_>,
    std::unordered_map<cfg::spore, boost::recursive_variant_>,
    cfg::nothing>::type                                 node;
typedef typename boost::mpl::at_c<node::types, 0>::type value;
typedef typename boost::mpl::at_c<node::types, 1>::type list;
typedef typename boost::mpl::at_c<node::types, 2>::type dict;
typedef typename cfg::dict::value_type                  pair;

/** forbid conversion from spore to node, as a node is already constructible
 * from a spore */
template <> struct is_spore_convertible<cfg::node> : std::false_type {};
template <> struct is_spore_convertible<cfg::list> : std::false_type {};
template <> struct is_spore_convertible<cfg::dict> : std::false_type {};

template <typename T> static inline bool is_a(cfg::node const& n) {
  return n.type() == typeid(T);
}

template <typename Cr, typename Tr>
static inline std::basic_ostream<Cr, Tr>&
operator<<(std::basic_ostream<Cr, Tr>& s, cfg::nothing const& sp) {
  s << "null";
  return s;
}
} // namespace corefungi

#endif // ifndef included_corefungi_node_hpp
