/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi.hpp"

#include <cstdlib>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include "corefungi/prefix.hpp"
#include "corefungi/yaml_parser.hpp"

#include <iostream>

namespace corefungi {
  namespace cfg = ::corefungi;

  namespace {
    namespace bpo = boost::program_options;
    namespace bfs = boost::filesystem;

    template< typename T >
    struct spore_maker {
      spore_maker(cfg::node& node, std::string const& key) :
        node(node),
        key(key)
      {}

      void operator()(T const& value) { cfg::put(this->node, this->key, value); }

      void operator()(std::vector< T > const& values) {
        cfg::grow(this->node, this->key + ".#" + std::to_string(values.size() - 1));
        auto nodes = cfg::collect(this->node, this->key + ".#");

        std::copy(values.begin(), values.end(), nodes.begin());
      }

      operator bpo::typed_value< T >*() {
        return bpo::value< T >()->notifier(*this);
      }

      bpo::typed_value< T >* operator->() {
        return static_cast< bpo::typed_value< T >* >(*this);
      }

      cfg::node_ref     node;
      std::string const key;
    };

    static bfs::path expand(std::string const& path) {
      if (path.empty())
        return bfs::path();

      if (path[0] != '~')
        return bfs::path(path);

      std::string modified = path;
      if ((path.size() > 1) &&
          (path[1] != '/'))
        modified.insert(1, "/../");

      char const* const home        = std::getenv("HOME");
      char const* const userprofile = std::getenv("USERPROFILE");
      char const* const homedrive   = std::getenv("HOMEDRIVE");
      char const* const homepath    = std::getenv("HOMEPATH");

      if (home)
        modified.replace(0, 1, std::string(home));

      else if (userprofile)
        modified.replace(0, 1, std::string(userprofile));

      else if (homedrive && homepath)
        modified.replace(0, 1, std::string(homedrive) + std::string(homepath));

      return cfg::expand(modified);
    }

    static bfs::path default_dir = cfg::expand("~") / ".config";
    static bfs::path system_path = bfs::path(cfg::install_prefix == "/usr" ? "/" : cfg::install_prefix) / "etc";

    static std::unordered_map< std::string, bfs::path > global_paths = {
      { "linux",  std::getenv("XDG_CONFIG") ? bfs::path(std::getenv("XDG_CONFIG")) : default_dir },
      { "Win32",  std::getenv("APPDATA") ? bfs::path(std::getenv("APPDATA")) : default_dir       },
      { "Mac OS", cfg::expand("~") / "Library" / "Preferences"                                   }
    };

    static bfs::path global_path = global_paths.find(BOOST_PLATFORM) == global_paths.end() ? default_dir : global_paths[BOOST_PLATFORM];

  }

  // *INDENT-OFF*/
  static cfg::node root = cfg::dict {
    { "system", cfg::node {} },
    { "global", cfg::node {} },
    { "local", cfg::node {} },
    { "command", cfg::node {} }
  };
 // *INDENT-ON*/

  cfg::node_ref system_sprout = cfg::collect(cfg::root, "system")[0];
  cfg::node_ref global_sprout  = cfg::collect(cfg::root, "global")[0];
  cfg::node_ref local_sprout   = cfg::collect(cfg::root, "local")[0];
  cfg::node_ref command_sprout = cfg::collect(cfg::root, "command")[0];

  void init(std::string const& program, cfg::arguments const& arguments) {
    auto const program_path = bfs::canonical(bfs::path(program).remove_filename());
    auto const program_name = bfs::path(program).stem().string();

    auto const config_name   = program_name + ".conf";
    auto const system_config = system_path / program_name / config_name;
    auto const global_config = global_path / program_name / config_name;

    cfg::spore_maker< std::string >(cfg::command_sprout, "program.location") (program_path.string());
    cfg::spore_maker< std::string >(cfg::command_sprout, "program.name") (program_name);
    cfg::spore_maker< std::string >(cfg::command_sprout, "program.arguments") (arguments);

    if (bfs::exists(system_config))
      yaml::read(system_config.string(), cfg::system_sprout);

    if (bfs::exists(global_config))
      yaml::read(global_config.string(), cfg::global_sprout);

    bpo::options_description generic_options("Generic options");
    generic_options.add_options()
      ("help,h", "produce help message")
      ("config,c", spore_maker< std::string >(cfg::command_sprout, "config.file"), "custom configuration file");

    bpo::options_description command_line_options;
    command_line_options.add(generic_options);

    bpo::variables_map vm;
    auto const&        options = bpo::command_line_parser(arguments).options(command_line_options).allow_unregistered().run();
    bpo::store(options, vm);
    bpo::notify(vm);

    std::string const local_config = cfg::get("config.file", (program_path / config_name).string());
    if (bfs::exists(local_config))
      yaml::read(local_config, cfg::local_sprout);

    if (vm.count("help"))
      std::cout << command_line_options << std::endl;
  }

}
