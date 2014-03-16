/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_OPTION_HPP__
#define __COREFUNGI_OPTION_HPP__

#include <string>
#include <vector>

#include <boost/program_options.hpp>

#include "corefungi/spore.hpp"
#include "corefungi/manipulable.hpp"

namespace corefungi {
  namespace cfg = ::corefungi;
  namespace bpo = ::boost::program_options;

  struct option_base {
    using validator = std::function< cfg::spore(std::string const& s) >;

    struct named_option : std::tuple< bool, cfg::spore, std::string > {
      using tuple_type = std::tuple< bool, cfg::spore, std::string >;

      template< typename T > named_option& operator=(T&& v) {
        tuple_type::operator=(tuple_type { true, cfg::spore { v }, cfg::spore { v } });
        return *this;
      }

      template< typename T > named_option& operator=(std::pair< T, std::string >&& v) {
        tuple_type::operator=(tuple_type { true, cfg::spore { v.first }, v.second });
        return *this;
      }

      named_option(bool const b, cfg::spore const v, std::string const s) : tuple_type{b, v, s} {}
    };

    option_base(std::string const name, std::string const description) :
      option_name(name),
      description(description)
    {}

    std::string const option_name;
    std::string const description;

    std::string short_name = "";
    std::string long_name  = option_name;

    named_option default_value  = { false, {}, "" };
    named_option implicit_value = { false, {}, "" };
    bool         composing      = false;
    bool         required       = false;
    bool         multitoken     = false;
    validator    validate       = [](std::string const& s) { return cfg::spore { s }; };
  };

  struct option : cfg::option_base, cfg::manipulable< cfg::option >, bpo::value_semantic_codecvt_helper< char > {
    using manipulable = cfg::manipulable< cfg::option >;
    using manipulable::field_setter;

    template< typename ... Ts >
    option(std::string const name, std::string const description, Ts&& ... ts) :
      cfg::option_base(name, description),
      manipulable(std::forward< Ts >(ts) ...) {
      std::replace(std::begin(this->long_name), std::end(this->long_name), '.', '-');
      std::replace(std::begin(this->long_name), std::end(this->long_name), '_', '-');
    }

    virtual ~option() {}

    std::string name() const override;
    unsigned    min_tokens() const override;
    unsigned    max_tokens() const override;
    bool        is_composing() const override;
    bool        is_required() const override;
    void        xparse(boost::any& value_store, std::vector< std::string > const& new_tokens) const override;
    bool        apply_default(boost::any& value_store) const override;
    void        notify(boost::any const& value_store) const override;
  };
  typedef std::vector< cfg::option > options;

  template< typename T >
  struct type_setter {
    cfg::option::manipulator operator()() const {
      return [ = ](cfg::option & o)->void {
               o.multitoken = false;
               o.validate   = [](std::string const& s) {
                                return cfg::spore { cfg::lexical_cast< T >(s) };
                              };
      };
    }

  };

  template< typename T >
  struct type_setter< std::vector< T > > {
    cfg::option::manipulator operator()() const {
      return [ = ](cfg::option & o)->void {
               o.multitoken = true;
               o.validate   = [](std::string const& s) {
                                return cfg::spore { cfg::lexical_cast< T >(s) };
                              };
      };
    }

  };

  static auto const default_   = cfg::option::field_setter(&cfg::option::default_value);
  static auto const implicit   = cfg::option::field_setter(&cfg::option::implicit_value);
  static auto const composing  = cfg::option::field_setter(&cfg::option::composing);
  static auto const required   = cfg::option::field_setter(&cfg::option::required);
  static auto const long_name  = cfg::option::field_setter(&cfg::option::long_name);
  static auto const short_name = cfg::option::field_setter(&cfg::option::short_name);

  template< typename T >
  static cfg::option::manipulator of_type() { return type_setter< T >()(); }
  static auto const bool_switch = [](cfg::option& o) -> void {
                                    cfg::of_type< bool >()(o);
                                    (cfg::default_ = false)(o);
                                    (cfg::implicit = true)(o);
                                    (cfg::composing = false)(o);
                                  };

}

#endif // ifndef __COREFUNGI_OPTION_HPP__
