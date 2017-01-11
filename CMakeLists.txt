cmake_minimum_required(VERSION 3.4)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
include(CMakeBuildPackage)

build_package(REQUIRES system
  "yaml-cpp>=0.5.2"
  "boost::program_options>=1.62.0"
  "boost::filesystem>=1.62.0"
)
