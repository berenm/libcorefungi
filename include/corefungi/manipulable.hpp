#ifndef included_corefungi_detail_manipulable_hpp
#define included_corefungi_detail_manipulable_hpp

#include <functional>

namespace corefungi {
namespace manipulate {

template <typename Class, typename Type> struct field {
  using manipulator_type = typename Class::manipulator;
  using field_type       = Type;
  using field_ptr_type   = field_type Class::*;

  template <typename U> manipulator_type operator=(U&& v) const {
    return [=](Class& o) -> void { o.*field_ptr = std::move(v); };
  }

  field(field_ptr_type const field_ptr) : field_ptr(field_ptr) {}
  field_ptr_type const field_ptr;
};
}

template <typename T> struct manipulable {
  using manipulator  = std::function<void(T&)>;
  using manipulators = std::vector<manipulator>;

  template <typename F>
  static inline auto field_setter(F field_ptr, T& t = *static_cast<T*>(nullptr))
      -> manipulate::field<T,
                           typename std::decay<decltype(t.*field_ptr)>::type> {
    using field_manip =
        manipulate::field<T, typename std::decay<decltype(t.*field_ptr)>::type>;
    return field_manip{field_ptr};
  }

  template <typename... Ts>
  manipulable(Ts&&... ts)
      : manipulable(manipulators{std::forward<Ts>(ts)...}) {}
  manipulable(manipulators&& manipulators) {
    for (auto const& manipulate : manipulators) {
      manipulate(*static_cast<T*>(this));
    }
  }
};
}

#endif // ifndef included_corefungi_detail_manipulable_hpp
