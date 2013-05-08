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

namespace corefungi {
  namespace cfg = ::corefungi;
  namespace bpo = ::boost::program_options;

  struct option : bpo::value_semantic_codecvt_helper< char > {
    typedef std::function< void (cfg::option&) >              manipulator;
    typedef std::vector< cfg::option::manipulator >           manipulators;
    typedef std::function< cfg::spore(std::string const& s) > validator;
    typedef std::tuple< bool, cfg::spore, std::string >       value_tuple;

    template< typename ... Ts >
    option(std::string const name, std::string const description, Ts ... ts) : option(name, description,
                                                                                      cfg::option::manipulators { ts ... }
                                                                                      )
    {}
    option(std::string const name, std::string const description, cfg::option::manipulators const& manipulators);
    virtual ~option() {}

    std::string name() const;
    unsigned    min_tokens() const;
    unsigned    max_tokens() const;
    bool        is_composing() const;
    bool        is_required() const;
    void        xparse(boost::any& value_store, std::vector< std::string > const& new_tokens) const;
    bool        apply_default(boost::any& value_store) const;
    void        notify(boost::any const& value_store) const;

    static cfg::spore no_validation(std::string const& s);

    std::string const option_name;
    std::string const option_description;

    std::string short_name;
    std::string long_name;

    value_tuple default_value;
    value_tuple implicit_value;
    bool        composing;
    bool        required;
    bool        multitoken;
    validator   validate;
  };
  typedef std::vector< cfg::option > options;

  template< typename T, T cfg::option::* Field >
  struct field_setter {
    cfg::option::manipulator operator=(T const v) const {
      return [ = ](cfg::option & o)->void { o.*Field = v; };
    }

    field_setter() {}
  };

  template< cfg::option::value_tuple cfg::option::* Field >
  struct field_setter< cfg::option::value_tuple, Field > {
    cfg::option::manipulator operator=(cfg::spore const& v) const {
      return [ = ](cfg::option & o)->void {
               std::get< 0 >(o.*Field) = true;
               std::get< 1 >(o.*Field) = v;
               std::get< 2 >(o.*Field) = v;
      };
    }

    cfg::option::manipulator operator=(std::pair< cfg::spore, std::string > const& v) const {
      return [ = ](cfg::option & o)->void {
               std::get< 0 >(o.*Field) = true;
               std::get< 1 >(o.*Field) = v.first;
               std::get< 2 >(o.*Field) = v.second;
      };
    }

    field_setter() {}
  };

  template< typename T >
  struct type_setter {
    cfg::option::manipulator operator()() const {
      return [ = ](cfg::option & o)->void {
               o.multitoken = false;
               o.validate   = [](std::string const& s) -> cfg::spore { return cfg::spore { boost::lexical_cast< T >(s) }; };
      };
    }

  };

  template< typename T >
  struct type_setter< std::vector< T > > {
    cfg::option::manipulator operator()() const {
      return [ = ](cfg::option & o)->void {
               o.multitoken = true;
               o.validate   = [](std::string const& s) -> cfg::spore { return cfg::spore { boost::lexical_cast< T >(s) }; };
      };
    }

  };

  static field_setter< cfg::option::value_tuple, & cfg::option::default_value > const  default_;
  static field_setter< cfg::option::value_tuple, & cfg::option::implicit_value > const implicit;
  static field_setter< bool, & cfg::option::composing > const                          composing;
  static field_setter< bool, & cfg::option::required > const                           required;
  static field_setter< std::string, & cfg::option::long_name > const                   long_name;
  static field_setter< std::string, & cfg::option::short_name > const                  short_name;

  template< typename T >
  static cfg::option::manipulator of_type() { return type_setter< T >() (); }
  static cfg::option::manipulator const bool_switch = [](cfg::option& o) -> void {
                                                        cfg::of_type< bool >() (o);
                                                        (cfg::default_ = false)(o);
                                                        (cfg::implicit = true)(o);
                                                        (cfg::composing = false)(o);
                                                      };

}

#endif // ifndef __COREFUNGI_OPTION_HPP__
