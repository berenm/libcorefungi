/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_DETAIL_SINGLETON_HPP__
#define __COREFUNGI_DETAIL_SINGLETON_HPP__

namespace corefungi {
  namespace detail {

    template< class T >
    struct singleton_wrapper : public T {
      static bool is_destroyed;
      ~singleton_wrapper() { singleton_wrapper::is_destroyed = true; }
    };
    template< class T > bool detail::singleton_wrapper< T >::is_destroyed = false;

    template< class T >
    struct singleton {
      private:
        static T& instance;
        static T& get_instance() {
          static detail::singleton_wrapper< T > t;

          assert(!detail::singleton_wrapper< T >::m_is_destroyed);
          singleton::use(instance);

          return static_cast< T& >(t);
        }

        static void use(T const&) {}
    };
    template< class T > T& singleton< T >::instance = singleton< T >::get_instance();

  }
}

#endif // ifndef __COREFUNGI_DETAIL_SINGLETON_HPP__
