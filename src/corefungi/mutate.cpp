#include "corefungi/mutate.hpp"

#include <boost/algorithm/string.hpp>

namespace corefungi {
  namespace cfg = ::corefungi;

  namespace {

    static void collect(cfg::list& list, std::string const& step,
                        cfg::ref_list& refs) {
      if (step.size() > 1) {
        size_t const index = boost::lexical_cast< size_t >(step.substr(1));
        refs.emplace_back(list[index]);

      } else {
        for (auto& n : list) {
          refs.emplace_back(n);
        }
      }
    }

    static void collect(cfg::dict& dict, std::string const& step,
                        cfg::ref_list& refs) {
      if (step != "*") {
        auto const spore = cfg::spore{step};
        if (dict.find(spore) != dict.end())
          refs.emplace_back(dict.at(std::move(spore)));

      } else {
        for (auto& pair : dict) {
          refs.emplace_back(pair.second);
        }
      }
    }

    typedef std::vector< std::string > step_list;
  }

  cfg::ref_list mutate(cfg::node& node, std::string const& path,
                       cfg::mutator const mutation) {
    cfg::ref_list  refs = {cfg::node_ref{node}};
    cfg::step_list steps;
    boost::split(steps, path, boost::is_any_of("."), boost::token_compress_on);

    for (auto const& step : steps) {
      if (step.empty()) continue;

      cfg::ref_list subrefs = {};

      for (auto& r : refs) {
        mutation(r, step);

        try {
          if (step.front() == '#')
            cfg::collect(boost::get< cfg::list >(r), step, subrefs);
          else
            cfg::collect(boost::get< cfg::dict >(r), step, subrefs);
        } catch (boost::bad_get const&) {}
      }

      refs = std::move(subrefs);
    }

    return refs;
  }
}
