cmake_minimum_required(VERSION 3.4)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
include(CMakeBuildPackage)

build_package(REQUIRES system
  "yaml-cpp>=0.5.2"
  "boost::program_options>=1.62.0"
  "boost::filesystem>=1.62.0"
)

add_executable(test-test1 test/test1.cpp)
target_link_libraries(test-test1 PUBLIC libcorefungi)
add_executable(test-test2 test/test2.cpp)
target_link_libraries(test-test2 PUBLIC libcorefungi)
