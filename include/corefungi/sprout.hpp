#ifndef included_corefungi_sprout_hpp
#define included_corefungi_sprout_hpp

#include <string>
#include <vector>

#include "corefungi/node.hpp"
#include "corefungi/option.hpp"
#include "corefungi/detail/singleton.hpp"

namespace corefungi {
namespace cfg = ::corefungi;

using mold = std::pair<std::string, cfg::options>;
struct sprouts : cfg::detail::singleton<cfg::sprouts> {
  std::vector<cfg::mold> molds;
};

struct sprout {
  sprout() = default;
  sprout(std::string const& s, cfg::options const& o) {
    sprouts::get_instance().molds.emplace_back(cfg::mold{s, o});
  }
};
}

#endif // ifndef included_corefungi_sprout_hpp
