cmake_minimum_required(VERSION 3.12)

add_executable(
    libmb-test
    Operator-test.cpp
    Term-test.cpp
    Expression-test.cpp
    NormalOrder-test.cpp
    Basis-test.cpp
    SparseMatrix-test.cpp
    Model-test.cpp
)

target_compile_options(
    libmb-test
    PRIVATE
    "${LIBMB_CXX_COMPILER_OPTIONS}"
)

target_include_directories(
    libmb-test
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    libmb-test
    PRIVATE
    gmock
    gmock_main
    gtest
    libmb
)

add_test(
    NAME libmb-test
    COMMAND libmb-test
)
