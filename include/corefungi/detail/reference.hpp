/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_DETAIL_REFERENCE_HPP__
#define __COREFUNGI_DETAIL_REFERENCE_HPP__

#include <ostream>

namespace corefungi {
  namespace cfg = ::corefungi;

  namespace detail {

    template< typename T >
    struct reference {
      public:
        /** default constructors and assignment operators for reference */
        reference()                              = default;
        reference(reference const& o)            = default;
        reference& operator=(reference const& o) = default;

        /** constructors and assignment operators for T object */
        reference(T& t) : pointer(&t) {}
        reference& operator=(T& t) { this->pointer = &t; return *this; }

        /** dereference the T object */
        operator T&() { return *this->pointer; }
        operator T const&() const { return *this->pointer; }

        /** retrieve the address of the referenced T object */
        T* operator&() { return this->pointer; }
        T const* operator&() const { return this->pointer; }

        /** transitive assignment operator for convenience */
        template< typename U >
        reference& operator=(U const& u) { this->pointer->operator=(u); return *this; }

      private:
        T* pointer = nullptr;
    };

    template< typename Cr, typename Tr, typename T >
    static inline std::basic_ostream< Cr, Tr >& operator<<(std::basic_ostream< Cr, Tr >& s, detail::reference< T > const& r) {
      s << static_cast< T const& >(r);

      return s;
    }

  }
}

#endif // ifndef __COREFUNGI_DETAIL_REFERENCE_HPP__
