# Copyright (c) 2024 Matheus Sousa
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.12)

add_executable(
  libmb-bench
  Basis-bench.cpp
  Model-bench.cpp
  NormalOrder-bench.cpp
)

target_include_directories(
  libmb-bench
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
  libmb-bench
  PRIVATE
  benchmark::benchmark_main
  pthread
  libmb
)
