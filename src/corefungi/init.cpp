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

  namespace {
    namespace bpo = boost::program_options;
    namespace bfs = boost::filesystem;

    template< typename T >
    struct spore_maker {
      spore_maker(corefungi::node& node, std::string const& key) :
        node(node),
        key(key)
      {}

      void operator()(T const& value) { corefungi::put(this->node, this->key, value); }

      void operator()(std::vector< T > const& values) {
        corefungi::grow(this->node, this->key + ".#" + std::to_string(values.size() - 1));
        auto nodes = corefungi::collect(this->node, this->key + ".#");

        std::copy(values.begin(), values.end(), nodes.begin());
      }

      operator bpo::typed_value< T >*() {
        return bpo::value< T >()->notifier(*this);
      }

      bpo::typed_value< T >* operator->() {
        return static_cast< bpo::typed_value< T >* >(*this);
      }

      corefungi::node_ref node;
      std::string const   key;
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

      return corefungi::expand(modified);
    }

    static bfs::path default_dir = corefungi::expand("~") / ".config";
    static bfs::path system_path = bfs::path(corefungi::install_prefix == "/usr" ? "/" : corefungi::install_prefix) / "etc";

    static std::unordered_map< std::string, bfs::path > global_paths = {
      { "linux",  std::getenv("XDG_CONFIG") ? bfs::path(std::getenv("XDG_CONFIG")) : default_dir },
      { "Win32",  std::getenv("APPDATA") ? bfs::path(std::getenv("APPDATA")) : default_dir       },
      { "Mac OS", corefungi::expand("~") / "Library" / "Preferences"                             }
    };

    static bfs::path global_path = global_paths.find(BOOST_PLATFORM) == global_paths.end() ? default_dir : global_paths[BOOST_PLATFORM];

  }

  // *INDENT-OFF*/
  static corefungi::node root = corefungi::dict {
    { "system", corefungi::node {} },
    { "global", corefungi::node {} },
    { "local", corefungi::node {} },
    { "command", corefungi::node {} }
  };
 // *INDENT-ON*/

  corefungi::node_ref system_sprout = corefungi::collect(corefungi::root, "system")[0];
  corefungi::node_ref global_sprout  = corefungi::collect(corefungi::root, "global")[0];
  corefungi::node_ref local_sprout   = corefungi::collect(corefungi::root, "local")[0];
  corefungi::node_ref command_sprout = corefungi::collect(corefungi::root, "command")[0];

  void init(std::string const& program, corefungi::arguments const& arguments) {
    auto const program_path = bfs::canonical(bfs::path(program).remove_filename());
    auto const program_name = bfs::path(program).stem().string();

    auto const config_name   = program_name + ".conf";
    auto const system_config = system_path / program_name / config_name;
    auto const global_config = global_path / program_name / config_name;

    spore_maker< std::string >(corefungi::command_sprout, "program.location") (program_path.string());
    spore_maker< std::string >(corefungi::command_sprout, "program.name") (program_name);
    spore_maker< std::string >(corefungi::command_sprout, "program.arguments") (arguments);

    if (bfs::exists(system_config))
      yaml::read(system_config.string(), corefungi::system_sprout);

    if (bfs::exists(global_config))
      yaml::read(global_config.string(), corefungi::global_sprout);

    bpo::options_description generic_options("Generic options");
    generic_options.add_options()
      ("help,h", "produce help message")
      ("config,c", spore_maker< std::string >(corefungi::command_sprout, "config.file"), "custom configuration file");

    bpo::options_description command_line_options;
    command_line_options.add(generic_options);

    bpo::variables_map vm;
    auto const&        options = bpo::command_line_parser(arguments).options(command_line_options).allow_unregistered().run();
    bpo::store(options, vm);
    bpo::notify(vm);

    std::string const local_config = corefungi::get("config.file", (program_path / config_name).string());
    if (bfs::exists(local_config))
      yaml::read(local_config, corefungi::local_sprout);

    if (vm.count("help"))
      std::cout << command_line_options << std::endl;
  }

}
