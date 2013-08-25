/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi/option.hpp"
#include "corefungi.hpp"

namespace corefungi {

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
    if (!this->multitoken) {
      if (!new_tokens.empty())
        value_store = this->validate(new_tokens[0]);
      else if (std::get< 0 >(this->implicit_value))
        value_store = std::get< 1 >(this->implicit_value);

    } else {
      if (value_store.empty())
        value_store = boost::any(cfg::spore_list {});

      cfg::spore_list& list = *boost::any_cast< cfg::spore_list >(&value_store);

      if (new_tokens.empty() && std::get< 0 >(this->implicit_value))
        list.emplace_back(std::get< 1 >(this->implicit_value));

      for (auto const& token : new_tokens) {
        list.emplace_back(this->validate(token));
      }
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

}
