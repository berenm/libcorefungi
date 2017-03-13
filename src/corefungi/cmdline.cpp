#include "corefungi/manipulable.hpp"
#include "corefungi/spore.hpp"
#include "corefungi.hpp"

#include <map>
#include <iostream>

namespace corefungi {
namespace cfg = ::corefungi;

using mold = std::pair<std::string, cfg::options>;
extern std::vector<cfg::mold> molds;

auto const levenshtein = [](auto&& s1, auto&& s2) {
  auto const l1 = s1.size();
  auto const l2 = s2.size();

  auto r0 = std::vector<int>{};
  auto r1 = std::vector<int>{};
  auto r2 = std::vector<int>{};

  r0.resize(l2 + 1);
  r2.resize(l2 + 1);
  std::generate_n(std::back_inserter(r1), l2 + 1, []() {
    static auto j = 0;
    return j++;
  });

  for (auto i = 0; i < l1; i++) {
    r2[0] = (i + 1) * 3;

    for (auto j = 0; j < l2; j++) {
      r2[j + 1] = r1[j] + 2 * (s1[i] != s2[j]);

      if ((i > 0) && (j > 0) && (s1[i - 1] == s2[j]) && (s1[i] == s2[j - 1]) &&
          (r2[j + 1] > r0[j - 1] + 0))
        r2[j + 1] = r0[j - 1];

      if (r2[j + 1] > r1[j + 1] + 3)
        r2[j + 1] = r1[j + 1] + 3;

      if (r2[j + 1] > r2[j] + 1)
        r2[j + 1] = r2[j] + 1;
    }

    std::swap(r0, r1);
    std::swap(r1, r2);
  }

  return r1[l2];
};

void parse(cfg::arguments args) {
  auto options  = std::map<std::string, cfg::option>{};
  auto optnames = std::vector<std::string>{};

  for (auto&& m : sprouts::get_instance().molds) {
    for (auto&& s : m.second) {
      optnames.emplace_back("--" + s.longname);
      options[optnames.back()] = s;

      if (s.has_default()) {
        cfg::put(cfg::command, s.name, s.default_.value);
      }

      if (s.shortname == "")
        continue;

      optnames.emplace_back("-" + s.shortname);
      options[optnames.back()] = s;
    }
  }

  if (options.empty())
    return;

  if (args.empty())
    return;

  std::reverse(std::begin(args), std::end(args));

  auto const optstart = [](auto&& option) {
    auto const list = cfg::collect(cfg::command, option.name);
    if (!option.composing && !list.empty())
      cfg::put(cfg::command, option.name, "");
    if (option.is_implicit())
      cfg::put(cfg::command, option.name, option.implicit.value);
  };

  auto const optvalue = [](auto&& option, auto&& value) {
    auto const name = option.name + (option.multiple ? ".#" : "");
    auto const list = cfg::collect(cfg::command, name);

    if (!list.empty() && !option.multiple) {
      std::cerr << "W: multiple values found for option " << option.longname
                << ", ";
      if (option.is_implicit())
        std::cerr << "no value expected." << std::endl;
      else
        std::cerr << "only one value expected." << std::endl;

      throw std::logic_error("invalid parameter");
    }

    if (option.multiple)
      cfg::put(cfg::command, name + std::to_string(list.size()), value);
    else
      cfg::put(cfg::command, option.name, value);
  };

  auto const optend = [](auto&& option) {
    auto const list = cfg::collect(cfg::command, option.name);

    if (!list.empty())
      return;

    if (!option.has_default()) {
      std::cerr << "W: no value found for option " << option.longname << ", ";
      if (option.multiple)
        std::cerr << "one or more values expected." << std::endl;
      else
        std::cerr << "one value expected." << std::endl;

      throw std::logic_error("invalid parameter");
    }
  };

  auto const opterr = [](auto&& arg, auto&& optnames) {
    std::sort(std::begin(optnames), std::end(optnames),
              [arg](auto&& s1, auto&& s2) {
                return levenshtein(arg, s1) < levenshtein(arg, s2);
              });
    std::cerr << "W: " << arg
              << " is not a valid option. See --help for usage. Did you mean "
              << optnames.front() << "?" << std::endl;
    throw std::logic_error("invalid parameter");
  };

  auto positional = cfg::option{"program.positional", "positional arguments"};
  positional.multiple  = true;
  positional.composing = true;
  positional.default_  = "";

  auto option = positional;
  while (!args.empty()) {
    auto arg = std::move(args.back());
    args.pop_back();

    if (arg[0] != '-') {
      optvalue(option, std::move(arg));
      if (!option.multiple) {
        optend(option);
        option = positional;
      }
    } else {
      auto it = options.find(arg);
      if (it == options.end())
        opterr(arg, std::move(optnames));
      optend(option);
      option = it->second;
      optstart(option);
      if (option.is_implicit()) {
        optend(option);
        option = positional;
      }
    }
  }
}

} // namespace corefungi
