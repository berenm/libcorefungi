#ifndef included_corefungi_mutate_hpp
#define included_corefungi_mutate_hpp

#include "corefungi/node.hpp"
#include "corefungi/node_ref.hpp"

namespace corefungi {
namespace cfg = ::corefungi;

typedef std::function<void(cfg::node& node, std::string const& step)> mutator;

cfg::ref_list mutate(cfg::node& node, std::string const& path,
                     cfg::mutator const mutation);
}

#endif // ifndef included_corefungi_mutate_hpp
