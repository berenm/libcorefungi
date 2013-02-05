/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#include "corefungi/corefungi.hpp"

#include <cstdlib>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

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

      void operator()(T const& value) { corefungi::put(this->key, value, this->node); }

      void operator()(std::vector< T > const& values) {
        corefungi::grow(this->key + ".#" + std::to_string(values.size() - 1), this->node);
        auto nodes = corefungi::collect(this->key + ".#", this->node);

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

    static bfs::path expand_user(std::string const& path) {
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

      return bfs::path(modified);
    }

  }

  corefungi::node root;

  void init(std::string const& program, corefungi::arguments const& arguments) {
    auto const application_path = bfs::canonical(bfs::path(program).remove_filename());
    auto const application_name = bfs::path(program).stem().string();

    auto const config_name   = application_name + ".conf";
    auto const system_config = bfs::path("/etc") / config_name;
    auto const global_config = expand_user("~") / config_name;
    auto const local_config  = application_path / config_name;

    spore_maker< std::string >(corefungi::root, "command.location") (application_path.string());
    spore_maker< std::string >(corefungi::root, "command.program") (bfs::path(program).stem().string());
    spore_maker< std::string >(corefungi::root, "command.arguments") (arguments);

    if (bfs::exists(system_config))
      yaml::read(system_config.string(), corefungi::root);

    if (bfs::exists(global_config))
      yaml::read(global_config.string(), corefungi::root);

    if (bfs::exists(local_config))
      yaml::read(local_config.string(), corefungi::root);

    std::clog << program << " ";
    for (auto const& argument : arguments) {
      std::clog << argument << " ";
    }
    std::clog << std::endl;

    if (arguments.empty())
      return;

    bpo::options_description generic_options("Generic options");
    generic_options.add_options()
      ("help", "produce help message")
      ("config,c", spore_maker< std::string >(corefungi::root, "config.file"), "custom configuration file");

    bpo::options_description command_line_options;
    command_line_options.add(generic_options);

    bpo::variables_map vm;
    auto const&        options = bpo::command_line_parser(arguments).options(command_line_options).allow_unregistered().run();
    bpo::store(options, vm);
    bpo::notify(vm);

    auto const& config_file = corefungi::get("config.file");
    if (bfs::extension(config_file) == ".yaml")
      yaml::read(config_file, corefungi::root);

    if (vm.count("help"))
      std::cout << command_line_options << std::endl;
  } // init

}
