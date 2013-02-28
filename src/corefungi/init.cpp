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
#include "corefungi/sprout.hpp"

#include <iostream>

namespace corefungi {
  namespace cfg = ::corefungi;

  namespace {
    namespace bpo = boost::program_options;
    namespace bfs = boost::filesystem;

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

    // *INDENT-OFF*/
    static cfg::node root = cfg::dict {
      { "system", cfg::node() },
      { "global", cfg::node() },
      { "local", cfg::node() },
      { "command", cfg::node() }
    };
    // *INDENT-ON*/

    static corefungi::sprout const o = {
      "General options", {
        { "program.help","produces this help message", cfg::long_name = "help", cfg::short_name = "h", cfg::bool_switch },
        { "program.config","custom configuration file", cfg::long_name = "config", cfg::short_name = "c" }
      }
    };

  }

  cfg::node_ref system  = cfg::collect(cfg::root, "system")[0];
  cfg::node_ref global  = cfg::collect(cfg::root, "global")[0];
  cfg::node_ref local   = cfg::collect(cfg::root, "local")[0];
  cfg::node_ref command = cfg::collect(cfg::root, "command")[0];

  void init(std::string const& program, cfg::arguments const& arguments) {
    namespace bpo = boost::program_options;

    auto const program_path = bfs::canonical(bfs::path(program).remove_filename());
    auto const program_name = bfs::path(program).stem().string();

    auto const config_name   = program_name + ".conf";
    auto const system_config = system_path / program_name / config_name;
    auto const global_config = global_path / program_name / config_name;

    cfg::put(cfg::command, "program.location", program_path.string());
    cfg::put(cfg::command, "program.name", program_name);
    cfg::grow(cfg::command, "program.arguments.#" + std::to_string(arguments.size() - 1));
    std::copy(arguments.begin(), arguments.end(), cfg::collect(cfg::command, "program.arguments.#").begin());

    if (bfs::exists(system_config))
      yaml::read(system_config.string(), cfg::system);

    if (bfs::exists(global_config))
      yaml::read(global_config.string(), cfg::global);

    bpo::options_description command_line_options;
    cfg::sprouts::get_instance().build(command_line_options);

    bpo::variables_map vm;
    auto const&        options = bpo::command_line_parser(arguments).options(command_line_options).allow_unregistered().run();
    bpo::store(options, vm);
    bpo::notify(vm);

    std::string const local_config = cfg::get("program.config", (program_path / config_name).string());
    if (bfs::exists(local_config))
      yaml::read(local_config, cfg::local);

    if (vm.count("help"))
      std::cout << command_line_options << std::endl;
  } // init

}
