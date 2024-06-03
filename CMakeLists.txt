cmake_minimum_required(VERSION 3.12)
project(LibMB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(vendor)

if (BUILD_TESTING)
  include(CTest)
  add_subdirectory(tests)
endif()
