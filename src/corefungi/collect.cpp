#include "corefungi.hpp"
#include "corefungi/mutate.hpp"

namespace corefungi {
namespace cfg = ::corefungi;

cfg::ref_list collect(cfg::node& node, std::string const& path) {
  auto const mutation = [](cfg::node&, std::string const&) {};

  return cfg::mutate(node, path, mutation);
}

cfg::spore get(cfg::node& node, std::string const& path) {
  cfg::node     n;
  cfg::ref_list collection = cfg::collect(node, path);

  collection.emplace_back(n);

  return boost::get<cfg::spore>(collection.front());
}

cfg::spore get(cfg::node& node, std::string const& path,
               cfg::spore const& default_value) {
  cfg::node     n          = default_value;
  cfg::ref_list collection = cfg::collect(node, path);

  collection.emplace_back(n);

  return boost::get<cfg::spore>(collection.front());
}

cfg::ref_list collect(std::string const& path) {
  cfg::ref_list refs;

  for (auto& n :
       cfg::ref_list{cfg::command, cfg::local, cfg::global, cfg::system}) {
    try {
      auto const collection = cfg::collect(n, path);
      std::copy(collection.begin(), collection.end(), std::back_inserter(refs));
    } catch (boost::bad_get const&) {
    }
  }

  return refs;
}

cfg::spore get(std::string const& path) {
  cfg::node     n;
  cfg::ref_list collection = cfg::collect(path);

  collection.emplace_back(n);

  return boost::get<cfg::spore>(collection.front());
}

cfg::spore get(std::string const& path, cfg::spore const& default_value) {
  cfg::node     n          = default_value;
  cfg::ref_list collection = cfg::collect(path);

  collection.emplace_back(n);

  return boost::get<cfg::spore>(collection.front());
}
}
