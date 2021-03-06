/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_DETAIL_MANIPULABLE_HPP__
#define __COREFUNGI_DETAIL_MANIPULABLE_HPP__

#include <functional>

namespace corefungi {
  namespace manipulate {

    template< typename Class, typename Type >
    struct field {
      using manipulator_type = typename Class::manipulator;
      using field_type       = Type;
      using field_ptr_type   = field_type Class::*;

      template< typename U >
      manipulator_type operator=(U&& v) const {
        return [=](Class& o)->void { o.*field_ptr = std::move(v); };
      }

      field(field_ptr_type const field_ptr) : field_ptr(field_ptr) {}
      field_ptr_type const field_ptr;
    };

  }

  template< typename T >
  struct manipulable {
    using manipulator  = std::function< void(T&) >;
    using manipulators = std::vector< manipulator >;

    template< typename F >
    static inline auto field_setter(F field_ptr, T& t = *static_cast< T* >(nullptr))->manipulate::field< T, typename std::decay< decltype(t.*field_ptr) >::type > {
      using field_manip = manipulate::field< T, typename std::decay< decltype(t.*field_ptr) >::type >;
      return field_manip { field_ptr };
    }

    template< typename ... Ts >
    manipulable(Ts&& ... ts) : manipulable(manipulators { std::forward< Ts >(ts) ... }) {}
    manipulable(manipulators&& manipulators) { for (auto const& manipulate : manipulators) { manipulate(*static_cast< T* >(this)); } }
  };

}

#endif // ifndef __COREFUNGI_DETAIL_MANIPULABLE_HPP__
