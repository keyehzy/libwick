cmake_minimum_required(VERSION 3.12)

find_package(OpenMP REQUIRED)

add_executable(
    main
    main.cpp
)

target_link_libraries(
    main
    libmb
)

add_library(
    libmb
    Basis.cpp
    FermionicBasis.cpp
    BosonicBasis.cpp
    Operator.cpp
    Term.cpp
    Expression.cpp
    NormalOrder.cpp
    Model.cpp
    SparseMatrix.cpp
    Models/LinearChain.cpp
    Models/HubbardChain.cpp
    Models/HubbardChainKSpace.cpp
    Models/HubbardKagome.cpp
    Models/HubbardSquare.cpp
)

target_include_directories(
    libmb
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    libmb
    PUBLIC
    OpenMP::OpenMP_CXX
)

