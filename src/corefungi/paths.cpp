#include "corefungi.hpp"

#include <boost/filesystem.hpp>

namespace corefungi {
namespace cfg = ::corefungi;

namespace {
namespace bfs = boost::filesystem;

static bfs::path const expand(std::string const& path) {
  if (path.empty())
    return bfs::path();

  if (path[0] != '~')
    return bfs::path(path);

  std::string modified = path;
  if ((path.size() > 1) && (path[1] != '/'))
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

static bfs::path const getpath(std::string const& variable,
                               bfs::path const&   fallback,
                               std::string const& append = "") {
  char const* const value = std::getenv(variable.c_str());

  if (value)
    return append.empty() ? bfs::path(value) : bfs::path(value) / append;

  return fallback;
}

static bfs::path const default_global_config_dir =
    (cfg::expand("~") / ".config").string();
static bfs::path const default_global_cache_dir =
    (cfg::expand("~") / ".cache").string();
static bfs::path const default_global_data_dir =
    (cfg::expand("~") / ".local" / "share").string();

static std::string const& platform = BOOST_PLATFORM;

typedef std::unordered_map<std::string, bfs::path> string_map;

static cfg::string_map const global_config_dirs = {
    {"linux", cfg::getpath("XDG_CONFIG_HOME", default_global_config_dir)},
    {"Win32", cfg::getpath("APPDATA", default_global_config_dir)},
    {"Mac OS", cfg::expand("~") / "Library" / "Preferences"},
    {cfg::platform, default_global_config_dir}};

static cfg::string_map const global_cache_dirs = {
    {"linux", cfg::getpath("XDG_CACHE_HOME", default_global_cache_dir)},
    {"Win32", cfg::getpath("LOCALAPPDATA", default_global_cache_dir, "Cache")},
    {"Mac OS", cfg::expand("~") / "Library" / "Caches"},
    {cfg::platform, default_global_cache_dir}};

static cfg::string_map const global_data_dirs = {
    {"linux", cfg::getpath("XDG_DATA_HOME", default_global_data_dir)},
    {"Win32", cfg::getpath("LOCALAPPDATA", default_global_data_dir)},
    {"Mac OS", cfg::expand("~") / "Library" / "Application Support"},
    {cfg::platform, default_global_data_dir}};

static bfs::path const get_default_prefix(bfs::path const& program) {
  return bfs::canonical(program).root_path() / "usr";
}

static bfs::path const get_prefix(bfs::path const& program) {
  bfs::path bin_dir = bfs::canonical(program).parent_path();

  while (bin_dir.has_parent_path() && bin_dir.filename() != "bin") {
    bin_dir = bin_dir.parent_path();
  }

  return bin_dir.has_parent_path() ? bin_dir.parent_path()
                                   : cfg::get_default_prefix(program);
}
}

std::string const global_config_dir(std::string const& program) {
  return (global_config_dirs.at(cfg::platform) / bfs::canonical(program).stem())
      .string();
}
std::string const global_cache_dir(std::string const& program) {
  return (global_cache_dirs.at(cfg::platform) / bfs::canonical(program).stem())
      .string();
}
std::string const global_data_dir(std::string const& program) {
  return (global_data_dirs.at(cfg::platform) / bfs::canonical(program).stem())
      .string();
}

std::string const system_config_dir(std::string const& program) {
  auto const prefix = cfg::get_prefix(program);

  if (prefix == cfg::get_default_prefix(program))
    return (bfs::canonical(program).root_path() / "etc").string();

  return (prefix / "etc" / bfs::canonical(program).stem()).string();
}

std::string const system_cache_dir(std::string const& program) {
  return (bfs::canonical(program).root_path() / "var" / "cache" /
          bfs::canonical(program).stem())
      .string();
}
std::string const system_data_dir(std::string const& program) {
  return (cfg::get_prefix(program) / "share" / bfs::canonical(program).stem())
      .string();
}
}
