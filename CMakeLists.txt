cmake_minimum_required(VERSION 3.11)
project(AOC2020)

set(CMAKE_CXX_FLAGS "-Wall -std=c++17 -fno-omit-frame-pointer -ggdb3 -O3")

# find_package(PkgConfig)
# pkg_check_modules(PC_RapidJSON QUIET RapidJSON)

# set(BOOST_ROOT "/home/ov699/opt/")
find_package(Boost 1.67.0 COMPONENTS unit_test_framework REQUIRED)

include_directories(src ${Boost_INCLUDE_DIR})

add_executable(level1 src/level1.cc)
target_compile_features(level1 PRIVATE cxx_std_17)

add_executable(level2 src/level2.cc)
target_compile_features(level2 PRIVATE cxx_std_17)

add_executable(level3 src/level3.cc)
target_compile_features(level3 PRIVATE cxx_std_17)

add_executable(level4 src/level4.cc)
target_compile_features(level4 PRIVATE cxx_std_17)

add_executable(level5 src/level5.cc)
target_compile_features(level5 PRIVATE cxx_std_17)

add_executable(level6 src/level6.cc)
target_compile_features(level6 PRIVATE cxx_std_17)

add_executable(level7 src/level7.cc)
target_compile_features(level7 PRIVATE cxx_std_17)

add_executable(level8 src/level8.cc)
target_compile_features(level8 PRIVATE cxx_std_17)

add_executable(level9 src/level9.cc)
target_compile_features(level9 PRIVATE cxx_std_17)

add_executable(level10 src/level10.cc)
target_compile_features(level10 PRIVATE cxx_std_17)

add_executable(level11 src/level11.cc)
target_compile_features(level11 PRIVATE cxx_std_17)

add_executable(level12 src/level12.cc)
target_compile_features(level12 PRIVATE cxx_std_17)

add_executable(level13 src/level13.cc)
target_link_libraries(level13 pthread)
target_compile_features(level13 PRIVATE cxx_std_17)

add_executable(level14 src/level14.cc)
target_compile_features(level14 PRIVATE cxx_std_17)

add_executable(level15 src/level15.cc)
target_compile_features(level15 PRIVATE cxx_std_17)

add_executable(level16_1 src/level16_1.cc)
target_compile_features(level16_1 PRIVATE cxx_std_17)

add_executable(level16_2 src/level16_2.cc)
target_compile_features(level16_2 PRIVATE cxx_std_17)

add_executable(level17 src/level17.cc)
target_compile_features(level17 PRIVATE cxx_std_17)

add_executable(level18 src/level18.cc)
target_compile_features(level18 PRIVATE cxx_std_17)

add_executable(level19 src/level19.cc)
target_compile_features(level19 PRIVATE cxx_std_17)

add_executable(level20 src/level20.cc)
target_compile_features(level20 PRIVATE cxx_std_17)

add_executable(level21 src/level21.cc)
target_compile_features(level21 PRIVATE cxx_std_17)

add_executable(level22 src/level22.cc)
target_compile_features(level22 PRIVATE cxx_std_17)

add_executable(level23 src/level23.cc)
target_compile_features(level23 PRIVATE cxx_std_17)

add_executable(level24 src/level24.cc)
target_compile_features(level24 PRIVATE cxx_std_17)

add_executable(level25 src/level25.cc)
target_compile_features(level25 PRIVATE cxx_std_17)
