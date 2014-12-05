#ifndef included_corefungi_option_hpp
#define included_corefungi_option_hpp

#include <string>
#include <vector>

#include "corefungi/spore.hpp"
#include "corefungi/manipulable.hpp"

namespace corefungi {
namespace cfg = ::corefungi;

struct option : cfg::manipulable<cfg::option> {
  struct optional : std::tuple<bool, cfg::spore> {
    using tuple = std::tuple<bool, cfg::spore>;

    optional(bool b, cfg::spore v) : tuple{b, std::move(v)} {}

    template <typename T> optional& operator=(T v) {
      return tuple::operator=(tuple{true, cfg::spore{v}}), *this;
    }
  };

  std::string name;
  std::string description;

  std::string shortname = "";
  std::string longname  = name;

  optional default_  = {false, {}};
  optional implicit  = {false, {}};
  bool     composing = false;
  bool     required  = false;
  bool     multiple  = false;

  bool has_default() const { return std::get<0>(this->default_); }
  bool is_implicit() const { return std::get<0>(this->implicit); }

  option()              = default;
  option(option const&) = default;
  option(option&&)      = default;
  option& operator=(option const&) = default;
  option& operator=(option&&) = default;

  using manipulable = cfg::manipulable<cfg::option>;

  template <typename... Ts>
  option(std::string name, std::string description, Ts&&... ts)
      : name{std::move(name)}, description{std::move(description)} {
    manipulable::apply(*this, std::forward<Ts>(ts)...);
    std::replace(std::begin(this->longname), std::end(this->longname), '.',
                 '-');
    std::replace(std::begin(this->longname), std::end(this->longname), '_',
                 '-');
  }
};

typedef std::vector<cfg::option> options;

auto const longname  = cfg::option::setter(&cfg::option::longname);
auto const shortname = cfg::option::setter(&cfg::option::shortname);
auto const default_  = cfg::option::setter(&cfg::option::default_);
auto const implicit  = cfg::option::setter(&cfg::option::implicit);
auto const composing = cfg::option::setter(&cfg::option::composing);
auto const required  = cfg::option::setter(&cfg::option::required);
auto const multiple  = cfg::option::setter(&cfg::option::multiple);

auto const bool_switch = [](cfg::option& o) {
  (cfg::default_ = false)(o);
  (cfg::implicit = true)(o);
  (cfg::composing = false)(o);
  (cfg::multiple = false)(o);
};
}

#endif // ifndef included_corefungi_option_hpp
