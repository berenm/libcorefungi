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


add_submodule(yaml-cpp git://github.com/bminor/yaml-cpp.git lib/yaml-cpp TAG release-0.5.0 INCLUDE_DIRS include)


include_directories(include src)

file(GLOB_RECURSE COREFUNGI_SOURCES src/corefungi/*.cpp)
add_executable(unit-test-corefungi ${COREFUNGI_SOURCES})
target_link_libraries(unit-test-corefungi yaml-cpp ${Boost_LIBRARIES})
