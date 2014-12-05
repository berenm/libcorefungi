#ifndef included_corefungi_hpp
#define included_corefungi_hpp

#include "corefungi/node.hpp"
#include "corefungi/node_ref.hpp"
#include "corefungi/spore.hpp"
#include "corefungi/sprout.hpp"

namespace corefungi {
namespace cfg = ::corefungi;

typedef std::vector<std::string> arguments;
void init(std::string const& program, cfg::arguments const& arguments);
void parse(cfg::arguments args);

static inline void init(int const argc, char const* const argv[]) {
  std::string const        program = argv[0];
  std::vector<std::string> arguments;

  for (size_t i = 1, e = static_cast<size_t>(argc); i < e; ++i) {
    arguments.push_back(argv[i]);
  }

  cfg::init(program, arguments);
}

cfg::ref_list collect(std::string const& path);
cfg::spore    get(std::string const& path);
cfg::spore    get(std::string const& path, cfg::spore const& default_value);

template <typename T, typename = cfg::spore::if_sporable<T>>
static inline cfg::spore get(std::string const& path, T&& default_value) {
  return cfg::get(path, cfg::spore{std::forward<T>(default_value)});
}

cfg::ref_list collect(cfg::node& node, std::string const& path);
cfg::spore    get(cfg::node& node, std::string const& path);
cfg::spore    get(cfg::node& node, std::string const& path,
                  cfg::spore const& default_value);

cfg::ref_list grow(cfg::node& node, std::string const& path);
void put(cfg::node& node, std::string const& path, cfg::spore const& value);

template <typename T, typename = cfg::spore::if_sporable<T>>
static inline void put(cfg::node& node, std::string const& path, T&& value) {
  cfg::put(node, path, cfg::spore{std::forward<T>(value)});
}

cfg::spore_list keys(cfg::node& node);
cfg::ref_list   values(cfg::node& node);
cfg::ref_dict   items(cfg::node& node);

extern cfg::node_ref system;
extern cfg::node_ref global;
extern cfg::node_ref local;
extern cfg::node_ref command;
}

#endif // ifndef included_corefungi_hpp
