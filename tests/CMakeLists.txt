cmake_minimum_required(VERSION 3.12)

add_executable(
    libwick-test
    Operator-test.cpp
    Term-test.cpp
    Expression-test.cpp
    NormalOrder-test.cpp
    Basis-test.cpp
    SparseMatrix-test.cpp
    Model-test.cpp
)

target_include_directories(
    libwick-test
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    libwick-test
    PRIVATE
    gmock
    gmock_main
    gtest
    libwick
)

add_test(
    NAME libwick-test
    COMMAND libwick-test
)