cmake_minimum_required(VERSION 3.12)

set(BUILD_GMOCK TRUE CACHE INTERNAL "")
set(INSTALL_GTEST FALSE CACHE INTERNAL "")
set(gmock_build_tests FALSE CACHE INTERNAL "")
set(gtest_build_samples FALSE CACHE INTERNAL "")
set(gtest_build_tests FALSE CACHE INTERNAL "")
set(gtest_force_shared_crt TRUE CACHE INTERNAL "")

add_subdirectory(googletest)

set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
set(BENCHMARK_ENABLE_INSTALL OFF CACHE INTERNAL "")
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE INTERNAL "")

add_subdirectory(benchmark)

