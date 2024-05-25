cmake_minimum_required(VERSION 3.12)
project(LibMB)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(vendor)
add_subdirectory(tests)
add_subdirectory(examples)
