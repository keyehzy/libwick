cmake_minimum_required(VERSION 3.12)

add_executable(
    main
    main.cpp
)

target_link_libraries(
    main
    libwick
)

add_library(
    libwick
    wick.cpp
    Operator.cpp
    Term.cpp
    Expression.cpp
    NormalOrder.cpp
    Model.cpp
    SparseMatrix.cpp
)

target_include_directories(
    libwick
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
