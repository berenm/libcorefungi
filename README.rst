Core Fungi: C++11 simple configuration library.
===============================================
.. image:: https://secure.travis-ci.org/berenm/libcorefungi.png?branch=master
    :alt: Build Status
    :target: https://travis-ci.org/berenm/libcorefungi

.. image:: https://coveralls.io/repos/berenm/libcorefungi/badge.png?branch=master
    :alt: Coverage Status
    :target: https://coveralls.io/r/berenm/libcorefungi

.. image:: http://stillmaintained.com/berenm/libcorefungi.png
    :alt: Still Maintained?
    :target: http://stillmaintained.com/berenm/libcorefungi


USAGE
-------------------------------------------------------------------------------
.. code:: cpp

  // 1. include the corefungi.hpp header.
  #include "corefungi.hpp"

  // 2. spread some sprouts in your libraries and executables.
  namespace cfg = ::corefungi;
  cfg::sprout const options = {
    "My library options", {
      { "my.switch",          "do something if this switch is used", cfg::bool_switch },
      { "my.integer.default", "an integer parameter with a default value",
                              cfg::of_type< size_t >(), cfg::default_ = 5 },
      { "my.integers",        "another integer parameter with multiple values",
                              cfg::of_type< std::vector< size_t > >(), cfg::composing = true,
                              cfg::long_name = "integer", cfg::short_name = "i" }
    }
  };

  int main(int argc, char const* argv[]) {
    // 3. fertilize the fungi from the command-line.
    cfg::init(argc, argv);

    // 4. collect the spores and cook them to your desired types.
    size_t default_integer = cfg::get("my.integer.default");
    std::cout << "my.integer.default is: " << default_integer << std::endl;

    std::vector< size_t > integers;
    auto const&           nodes = cfg::collect("my.integers.#");
    for (auto const& node : nodes) {
      size_t const integer = boost::get< cfg::spore >(node);
      integers.emplace_back(integer);
    }

    std::cout << "my.integers are: " << integers << std::endl;
  }

And at run-time:

.. code::

  $ ./my-executable -h

  My library options:
    --my-switch                   do something if this switch is used
    --my-integer-default arg (=5) an integer parameter with a default value
    -i [ --integer ] arg          another integer parameter with multiple values

  General options:
    -h [ --help ]         produces this help message
    -c [ --config ] arg   custom configuration file

  $ ./my-executable -i 3 4 5 -i 2
  my.integer.default is: 5
  my.integers are: [3, 4, 5, 2]


STATIC INITIALIZATION CAVEAT
-------------------------------------------------------------------------------
The automatic option registration, through the cfg::sprout type, relies on static
initialisation mechanism. However, the C++ standard allows a linker to drop the code
of any compiled translation unit if it can prove that no code from it is used anywhere
in the compiled program, dropping any statically initialized variable as well.

Currently, no workaround is implemented, if ever possible, and the sprout might not grow
as expected if they are spread in isolated translation units. The best practice is to
spread the sprouts around some core functionality of your program or libraries, to be
sure they will grow if ever the functionality is used.

Or one can even see that as a feature to disable sprouts from unused libraries...


LICENSE
-------------------------------------------------------------------------------

 This is free and unencumbered software released into the public domain.

 See accompanying file UNLICENSE or copy at http://unlicense.org/UNLICENSE
