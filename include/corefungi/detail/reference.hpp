#ifndef __COREFUNGI_DETAIL_REFERENCE_HPP__
#define __COREFUNGI_DETAIL_REFERENCE_HPP__

#include <iosfwd>
#include <utility>

namespace corefungi {
  namespace cfg = ::corefungi;

  namespace detail {

    template< typename T >
    struct reference {
      public:
        /** default constructors and assignment operators for reference */
        reference()                              = default;
        reference(reference const& o)            = default;
        reference(reference&& o)                 = default;
        reference& operator=(reference const& o) = default;
        reference& operator=(reference&& o)      = default;

        template< typename U > using if_derive_T = typename std::enable_if< std::is_base_of< T, U >::value, void >::type;

        /** constructors and assignment operators for T object */
        template< typename U, typename = if_derive_T< U > > explicit reference(U const& u) : pointer(&u) {}
        template< typename U, typename = if_derive_T< U > > explicit reference(U& u) : pointer(&u) {}

        /** dereference the T object */
        template< typename U = T, typename = if_derive_T< U > > operator U const&() const { return *static_cast< U const* >(this->pointer); }
        template< typename U = T, typename = if_derive_T< U > > operator U&() { return *static_cast< U* >(this->pointer); }

        /** retrieve the address of the referenced T object */
        template< typename U = T, typename = if_derive_T< U > > U const* operator&() const { return static_cast< U const* >(this->pointer); }
        template< typename U = T, typename = if_derive_T< U > > U* operator&() { return static_cast< U* >(this->pointer); }

        template< typename U = T, typename = if_derive_T< U > > U const& operator*() const { return *static_cast< U const* >(this->pointer); }
        template< typename U = T, typename = if_derive_T< U > > U& operator*() { return *static_cast< U* >(this->pointer); }

        /** transitive assignment operator for convenience */
        template< typename U > reference& operator=(U&& u) { this->pointer->operator=(std::forward< U >(u)); return *this; }

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
