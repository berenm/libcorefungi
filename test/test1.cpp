#include "corefungi.hpp"
#include "ostream.hpp"

#include <iostream>

corefungi::sprout const options = {
    "Corefungi options",
    {{"another.test", "alternate config file", corefungi::bool_switch},
     {"test.size_t", "a size_t parameter"}}};

int main(int argc, char const* argv[]) {
  namespace cfg = ::corefungi;

  {
    cfg::node m;
    cfg::put(m, "#.a", "1");
    cfg::put(m, "#2.b", "1");

    std::cout << m << std::endl;
  }

  cfg::node n;

  try {
    cfg::init(argc, argv);
    std::clog << "command: " << cfg::command << std::endl;
    std::clog << "local: " << cfg::local << std::endl;
    std::clog << "global: " << cfg::global << std::endl;
    std::clog << "system: " << cfg::system << std::endl;

    bool result = cfg::get("another.test");

    n = cfg::dict{
        {cfg::spore{"value"}, cfg::value{"bla"}},
        {cfg::spore{"list-value"},
         cfg::list{cfg::value{"bar"}, cfg::value{"bar"}}},
        {cfg::spore{"dict-value"},
         cfg::dict{{cfg::spore{"bar"}, cfg::value{"bar"}}}},
        {cfg::spore{"dict-value-2"},
         cfg::dict{{cfg::spore{"bar"},
                    cfg::list{cfg::value{"bar0"}, cfg::value{"bar1"}}}}}};

    std::cout << cfg::collect(n, "*.bar.#0") << std::endl;

    cfg::node m;
    std::cout << m << std::endl;

    auto sprinklings = cfg::grow(m, "foo.#.bla");
    std::cout << sprinklings << std::endl;
    for (auto& n : sprinklings) {
      n = cfg::spore{"bla"};
    }

    std::cout << m << std::endl;

    cfg::put(m, "foo.#.*", "bar");
    std::cout << m << std::endl;

    for (auto& pair : cfg::items(n)) {
      std::cout << pair.first << ": " << pair.second << std::endl;
    }

    for (auto& key : cfg::keys(n)) {
      std::cout << key << std::endl;
    }

    for (auto& value : cfg::values(n)) {
      std::cout << value << std::endl;
    }

    std::cout << n << std::endl;
    try {
      auto const   sp = cfg::spore{"0"};
      size_t const v  = sp;

      std::cout << sp << ": " << v << std::endl;
    } catch (std::exception const& e) {
      std::clog << "Exception raised: " << e.what() << std::endl;
    }
  } catch (std::exception const& e) {
    std::clog << "Exception raised: " << e.what() << std::endl;
  }

  return 0;
} // main
