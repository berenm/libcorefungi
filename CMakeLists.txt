cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CMakeExtraBootstrap)
include(GNUInstallDirs)
include(ParseVersion)
include(AddSubmodule)
include(FindCXX11)
include(FixCCache)
include(FixStaticLink)

project(libcorefungi CXX)
parse_version(libcorefungi VERSION 0.0.1)


find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})


add_submodule(yaml-cpp git://github.com/bminor/yaml-cpp.git lib/yaml-cpp TAG release-0.5.1 INCLUDE_DIRS include)


# -------------------------------------------------------------------------
# libcorefungi
include_directories(include src ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE libcorefungi_SOURCES src/*.cpp)

add_library(corefungi ${libcorefungi_SOURCES})
set_target_properties(corefungi PROPERTIES
  VERSION   ${libcorefungi_VERSION}
  SOVERSION ${libcorefungi_SOVERSION}
  SONAME    libcorefungi${libcorefungi_SOVERSION}
)
target_link_libraries(corefungi yaml-cpp ${Boost_LIBRARIES})

# -------------------------------------------------------------------------
# installation
install(TARGETS corefungi
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/corefungi
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# -------------------------------------------------------------------------
# tests
if(BUILD_TESTS)
  enable_testing()

  add_executable(unit-test-corefungi test/test1.cpp)
  target_link_libraries(unit-test-corefungi corefungi)
  add_test(unit-test-corefungi unit-test-corefungi)

  add_executable(unit-test-corefungi-2 test/test2.cpp)
  target_link_libraries(unit-test-corefungi-2 corefungi)
  add_test(unit-test-corefungi-2 unit-test-corefungi)
endif()

if(ENABLE_GCOVERALLS)
  include(GCoveralls)
  enable_gcoveralls()
endif()
