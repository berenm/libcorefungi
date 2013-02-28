/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi/option.hpp"
#include "corefungi.hpp"

namespace corefungi {

  option::option(std::string const name, std::string const description, cfg::option::manipulators const& manipulators) :
    option_name(name),
    option_description(description),
    short_name(""),
    long_name(name),
    default_value(false, {}, ""),
    implicit_value(false, {}, ""),
    composing(false),
    required(false),
    multitoken(false),
    validate(option::no_validation) {
    std::replace(this->long_name.begin(), this->long_name.end(), '.', '-');

    for (auto const& manipulate : manipulators) {
      manipulate(*this);
    }
  }

  std::string option::name() const {
    if (std::get< 0 >(this->implicit_value) && (std::get< 2 >(this->implicit_value) != "")) {
      std::string msg = "[=arg(=" + std::get< 2 >(this->implicit_value) + ")]";
      if (std::get< 0 >(this->default_value) && (std::get< 2 >(this->default_value) != ""))
        msg += " (=" + std::get< 2 >(this->default_value) + ")";

      return msg;
    } else if (std::get< 0 >(this->default_value) && (std::get< 2 >(this->default_value) != "")) {
      return bpo::arg + " (=" + std::get< 2 >(this->default_value) + ")";
    } else {
      return bpo::arg;
    }
  }

  unsigned option::min_tokens() const {
    return std::get< 0 >(this->implicit_value) ? 0 : 1;
  }

  unsigned option::max_tokens() const {
    return std::get< 0 >(this->implicit_value) ? 0 : (this->multitoken ? -1 : 1);
  }

  bool option::is_composing() const {
    return this->composing;
  }

  bool option::is_required() const {
    return this->required;
  }

  void option::xparse(boost::any& value_store, std::vector< std::string > const& new_tokens) const {
    if (new_tokens.empty() && std::get< 0 >(this->implicit_value)) {
      if (!this->multitoken) {
        value_store = std::get< 1 >(this->implicit_value);
      } else {
        value_store = cfg::spore_list {
          std::get< 1 >(this->implicit_value)
        };
      }

      return;
    }

    if (!this->multitoken) {
      value_store = this->validate(new_tokens[0]);
    } else {
      auto l = cfg::spore_list {};

      for (auto const& s : new_tokens) {
        l.emplace_back(this->validate(s));
      }

      value_store = l;
    }
  }

  bool option::apply_default(boost::any& value_store) const {
    if (!std::get< 0 >(this->default_value))
      return false;

    if (!this->multitoken) {
      value_store = std::get< 1 >(this->default_value);
    } else {
      value_store = cfg::spore_list {
        std::get< 1 >(this->default_value)
      };
    }

    return true;
  }

  void option::notify(boost::any const& value_store) const {
    if (!this->multitoken) {
      cfg::put(cfg::command, this->option_name, boost::any_cast< cfg::spore >(value_store));
    } else {
      auto const& values = boost::any_cast< cfg::spore_list >(value_store);
      cfg::grow(cfg::command, this->option_name + ".#" + std::to_string(values.size() - 1));
      auto nodes = cfg::collect(cfg::command, this->option_name + ".#");
      std::copy(values.begin(), values.end(), nodes.begin());
    }
  }

  cfg::spore option::no_validation(std::string const& s) {
    return s;
  }

}
