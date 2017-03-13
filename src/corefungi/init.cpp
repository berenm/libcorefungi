#include "corefungi.hpp"

#include <cstdlib>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

#include "corefungi/yaml_parser.hpp"
#include "corefungi/sprout.hpp"
#include "corefungi/paths.hpp"

#include <iostream>

namespace corefungi {
namespace cfg = ::corefungi;

namespace {
namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;

// *INDENT-OFF*/
static cfg::node root = cfg::dict{{cfg::spore{"system"}, cfg::node{}},
                                  {cfg::spore{"global"}, cfg::node{}},
                                  {cfg::spore{"local"}, cfg::node{}},
                                  {cfg::spore{"command"}, cfg::node{}}};
// *INDENT-ON*/

auto const o = cfg::sprout{
    "General options",
    {{"program.help", "produces this help message", cfg::longname = "help",
      cfg::shortname = "h", cfg::bool_switch},
     {"program.config", "custom configuration file", cfg::longname = "config",
      cfg::shortname = "c"}}};
} // namespace

cfg::node_ref system  = cfg::collect(cfg::root, "system")[0];
cfg::node_ref global  = cfg::collect(cfg::root, "global")[0];
cfg::node_ref local   = cfg::collect(cfg::root, "local")[0];
cfg::node_ref command = cfg::collect(cfg::root, "command")[0];

void init(std::string const& program, cfg::arguments const& arguments) {
  namespace bpo = boost::program_options;

  auto const program_path = bfs::canonical(bfs::path(program).parent_path());
  auto const program_name = bfs::path(program).stem().string();

  cfg::put(cfg::command, "program.location", program_path.string());
  cfg::put(cfg::command, "program.name", program_name);
  cfg::grow(cfg::command,
            "program.arguments.#" + std::to_string(arguments.size() - 1));
  std::transform(arguments.begin(), arguments.end(),
                 cfg::collect(cfg::command, "program.arguments.#").begin(),
                 [](std::string const& s) { return cfg::spore{s}; });

  cfg::put(cfg::command, "program.configdir.system",
           cfg::system_config_dir(program));
  cfg::put(cfg::command, "program.cachedir.system",
           cfg::system_cache_dir(program));
  cfg::put(cfg::command, "program.datadir.system",
           cfg::system_data_dir(program));

  cfg::put(cfg::command, "program.configdir.global",
           cfg::global_config_dir(program));
  cfg::put(cfg::command, "program.cachedir.global",
           cfg::global_cache_dir(program));
  cfg::put(cfg::command, "program.datadir.global",
           cfg::global_data_dir(program));

  auto const config_name   = program_name + ".conf";
  auto const system_config = bfs::path(cfg::get("program.system.configdir")) /
                             program_name / config_name;
  auto const global_config = bfs::path(cfg::get("program.global.configdir")) /
                             program_name / config_name;

  if (bfs::exists(system_config))
    yaml::read(system_config.string(), cfg::system);

  if (bfs::exists(global_config))
    yaml::read(global_config.string(), cfg::global);

  parse(arguments);

  std::string const local_config =
      cfg::get("program.config", (program_path / config_name).string());
  if (bfs::exists(local_config))
    yaml::read(local_config, cfg::local);

  if (static_cast<bool>(cfg::get("program.help"))) {
    size_t column = 0;
    for (auto&& m : sprouts::get_instance().molds) {
      for (auto&& s : m.second) {
        auto col = 4 + s.longname.size();
        if (!s.shortname.empty())
          col += 3 + s.shortname.size();

        if (!s.implicit.has_value) {
          if (s.multiple)
            col += 10;
          else
            col += 6;

          if (s.default_.has_value)
            col += 1 + s.default_.value.size();
        }

        column = std::max(column, col);
      }
    }

    for (auto&& m : sprouts::get_instance().molds) {
      std::cerr << m.first << ":\n";

      // Corefungi options:
      //   --another-test         alternate config file
      //   --test-size-t arg      a size_t parameter

      // General options:
      //   -h [ --help ]          produces this help message
      //   -c [ --config ] arg    custom configuration file

      for (auto&& s : m.second) {
        if (!s.shortname.empty())
          std::cerr << "  -" << s.shortname << ", --" << s.longname;
        else
          std::cerr << "  --" << s.longname;

        if (!s.implicit.has_value) {
          if (s.multiple)
            std::cerr << " [args...";
          else
            std::cerr << " [arg";

          if (s.default_.has_value)
            std::cerr << "=" << s.default_.value;

          std::cerr << "]";
        }

        auto col = 4 + s.longname.size();
        if (!s.shortname.empty())
          col += 3 + s.shortname.size();

        if (!s.implicit.has_value) {
          if (s.multiple)
            col += 10;
          else
            col += 6;

          if (s.default_.has_value)
            col += 1 + s.default_.value.size();
        }

        std::cerr << std::string(column - col, ' ') << " " << s.description
                  << "\n";
      }

      std::cerr << "\n";
    }

    std::exit(0);
  }
} // init
} // namespace corefungi
