# Copyright (c) 2024 Matheus Sousa
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.12)
project(LibMB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(LIBMB_CXX_COMPILER_OPTIONS "" CACHE STRING "")
mark_as_advanced(LIBMB_CXX_COMPILER_OPTIONS)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(vendor)

if (BUILD_TESTING)
  include(CTest)
  add_subdirectory(tests)
endif()
