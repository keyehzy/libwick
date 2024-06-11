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
    BosonicBasis.cpp
    Expression.cpp
    FermionicBasis.cpp
    Model.cpp
    Models/HubbardChain.cpp
    Models/HubbardChainKSpace.cpp
    Models/HubbardKagome.cpp
    Models/HubbardSquare.cpp
    Models/LinearChain.cpp
    NormalOrder.cpp
    Operator.cpp
    SparseMatrix.cpp
    SpinOperators.cpp
    Term.cpp
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

