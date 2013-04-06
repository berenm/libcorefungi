/**
 * @file
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt
 */

#ifndef __COREFUNGI_PATHS_HPP__
#define __COREFUNGI_PATHS_HPP__

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

#endif // ifndef __COREFUNGI_PATHS_HPP__
