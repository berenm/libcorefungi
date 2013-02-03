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
