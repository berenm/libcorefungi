#ifndef included_corefungi_paths_hpp
#define included_corefungi_paths_hpp

#include <string>

namespace corefungi {
namespace cfg = ::corefungi;

std::string const global_config_dir(std::string const& program);
std::string const global_cache_dir(std::string const& program);
std::string const global_data_dir(std::string const& program);

std::string const system_config_dir(std::string const& program);
std::string const system_cache_dir(std::string const& program);
std::string const system_data_dir(std::string const& program);
}

#endif // ifndef included_corefungi_paths_hpp
