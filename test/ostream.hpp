#ifndef __COREFUNGI_OSTREAM_HPP__
#define __COREFUNGI_OSTREAM_HPP__

#include <cstdint>
#include <ostream>
#include <vector>
#include <list>
#include <forward_list>
#include <queue>
#include <stack>
#include <valarray>
#include <array>
#include <bitset>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <complex>
#include <functional>

namespace std {

  namespace detail {

    struct sequence_container;
    struct associative_container;
    struct collection_container;

    template< typename T > struct container_type;

    template< typename T, typename C = typename container_type< T >::type >
    struct delimiters;

    template< typename  T >
    struct delimiters< T, sequence_container > {
      static constexpr char begin     = '[';
      static constexpr char end       = ']';
      static constexpr char separator = ',';
    };

    template< typename T >
    struct delimiters< T, associative_container > {
      static constexpr char begin     = '{';
      static constexpr char end       = '}';
      static constexpr char separator = ',';
    };

    template< typename T >
    struct delimiters< T, collection_container > {
      static constexpr char begin     = '(';
      static constexpr char end       = ')';
      static constexpr char separator = ',';
    };

#define STL_OSTREAM_DECLARE_CONTAINER_TYPE(container_m, type_m)                      \
  template< typename ... Types > struct container_type< container_m< Types ... > > { \
    typedef  type_m type;                                                            \
  }

    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::deque, sequence_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::list, sequence_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::forward_list, sequence_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::vector, sequence_container);

    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::stack, sequence_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::queue, sequence_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::priority_queue, sequence_container);

    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::valarray, sequence_container);

    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::set, associative_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::multiset, associative_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::map, associative_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::multimap, associative_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::unordered_set, associative_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::unordered_multiset, associative_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::unordered_map, associative_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::unordered_multimap, associative_container);

    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::tuple, collection_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::pair, collection_container);
    STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::complex, collection_container);

    template< class T, std::size_t N > struct container_type< std::array< T, N > > {
      typedef  sequence_container type;
    };

    // STL_OSTREAM_DECLARE_CONTAINER_TYPE(std::bitset, sequence_container);

#undef STL_OSTREAM_DECLARE_CONTAINER_TYPE

    template< std::size_t const N, typename ... Types >
    struct tuple_printer {
      template< typename S >
      static inline void print(S& s, std::tuple< Types ... > const& t) {
        detail::tuple_printer< N - 1, Types ... >::print(s, t);

        s << detail::delimiters< std::tuple< Types ... > >::separator << ' ' << std::get< N >(t);
      }

    };

    template< typename ... Types >
    struct tuple_printer< 0, Types ... > {
      template< typename S >
      static inline void print(S& s, std::tuple< Types ... > const& t) {
        s << std::get< 0 >(t);
      }

    };
  }

  template< typename C, typename T = typename C::value_type, typename Type = typename detail::container_type< C >::type, typename Cr, typename Tr >
  static inline std::basic_ostream< Cr, Tr >& operator<<(std::basic_ostream< Cr, Tr >& s, C const& v) {
    if (std::begin(v) == std::end(v)) {
      s << detail::delimiters< C >::begin << detail::delimiters< C >::end;

      return s;
    }

    s << detail::delimiters< C >::begin;
    T const& f = *std::begin(v);
    for (T const& t : v) {
      if (&t != &f)
        s << detail::delimiters< C >::separator << ' ';
      s << t;
    }
    s << detail::delimiters< C >::end;

    return s;
  }

  template< typename K, typename V, typename Cr, typename Tr >
  static inline std::basic_ostream< Cr, Tr >& operator<<(std::basic_ostream< Cr, Tr >& s, std::pair< K, V > const& p) {
    s << p.first << ": " << p.second;

    return s;
  }

  template< typename ... Types, typename Cr, typename Tr >
  static inline std::basic_ostream< Cr, Tr >& operator<<(std::basic_ostream< Cr, Tr >& s, std::tuple< Types ... > const& t) {
    s << detail::delimiters< std::tuple< Types ... > >::begin;
    detail::tuple_printer< sizeof ... (Types) -1, Types ... >::print(s, t);
    s << detail::delimiters< std::tuple< Types ... > >::end;

    return s;
  }

  template< size_t S, typename Cr, typename Tr >
  static inline std::basic_ostream< Cr, Tr >& operator<<(std::basic_ostream< Cr, Tr >& s, std::bitset< S > const& b) {
    s << b.to_string();

    return s;
  }

  template< class R, class ... Args, typename Cr, typename Tr >
  static inline std::basic_ostream< Cr, Tr >& operator<<(std::basic_ostream< Cr, Tr >& s, std::function< R(Args ...) > const& f) {
    s << reinterpret_cast< void const* >(*f.template target< R (*)(Args ...) >());

    return s;
  }

}

#endif // ifndef __COREFUNGI_OSTREAM_HPP__
