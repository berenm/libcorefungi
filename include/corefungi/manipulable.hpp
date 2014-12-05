#ifndef included_corefungi_detail_manipulable_hpp
#define included_corefungi_detail_manipulable_hpp

#include <utility>
#include <functional>

namespace corefungi {
namespace cfg = ::corefungi;

template <typename T> struct manipulable {
  template <typename F> struct helper {
    constexpr auto operator=(auto&& v) const {
      return [=](T& o) { o.*p = std::move(v); };
    }

    F const p;
  };

  constexpr static auto setter(auto&& p) {
    return helper<std::decay_t<decltype(p)>>{std::forward<decltype(p)>(p)};
  }

  constexpr static void apply(T& t, auto&&... ms) {
    (int[]){call(t, std::forward<decltype(ms)>(ms))...};
  }
  constexpr static void apply(T& t) {}

private:
  constexpr static int call(T& t, auto&& m) {
    std::forward<decltype(m)>(m)(t);
    return 0;
  }
};
}

#endif // ifndef included_corefungi_detail_manipulable_hpp
