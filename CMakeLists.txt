cmake_minimum_required(VERSION 3.11)
project(AOC2020)

set(CMAKE_CXX_FLAGS "-std=c++17 -fno-omit-frame-pointer -ggdb3")

# find_package(PkgConfig)
# pkg_check_modules(PC_RapidJSON QUIET RapidJSON)

# set(BOOST_ROOT "/home/ov699/opt/")
find_package(Boost 1.67.0 COMPONENTS unit_test_framework REQUIRED)

include_directories(src)

add_executable(level1 src/level1.cc)
target_compile_features(level1 PRIVATE cxx_std_17)

add_executable(level2 src/level2.cc)
target_compile_features(level2 PRIVATE cxx_std_17)

add_executable(level3 src/level3.cc)
target_compile_features(level3 PRIVATE cxx_std_17)

add_executable(level4 src/level4.cc)
target_compile_features(level4 PRIVATE cxx_std_17)
