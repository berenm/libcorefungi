#ifndef __COREFUNGI_SPROUT_HPP__
#define __COREFUNGI_SPROUT_HPP__

#include <string>
#include <vector>

#include <boost/program_options.hpp>

#include "corefungi/detail/singleton.hpp"
#include "corefungi/node.hpp"
#include "corefungi/option.hpp"

namespace corefungi {
  namespace cfg = ::corefungi;
  namespace bpo = ::boost::program_options;

  typedef std::pair< std::string, cfg::options > mold;

  struct sprouts : cfg::detail::singleton< cfg::sprouts > {
    std::vector< std::function< bpo::options_description() > > builders;

    static void add(cfg::mold&& m);
    static void build(bpo::options_description& global);
  };

  struct sprout : cfg::detail::singleton< cfg::sprout > {
    sprout() = default;
    sprout(std::string const& s, cfg::options const& o) {
      sprout::get_instance() = cfg::mold{s, o};
    }
    void operator=(cfg::mold&& m) const {
      sprouts::add(std::forward< cfg::mold >(m));
    }
  };
}

#endif // ifndef __COREFUNGI_SPROUT_HPP__
