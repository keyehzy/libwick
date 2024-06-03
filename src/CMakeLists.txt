cmake_minimum_required(VERSION 3.12)

find_package(OpenMP REQUIRED)
find_package(BLAS)
find_package(Armadillo)

add_executable(
    main
    main.cpp
)

target_link_libraries(
    main
    libmb
    BLAS::BLAS
    armadillo
    superlu
    arpack
    m
    gfortran
)

target_compile_definitions(
    main
    PUBLIC
    ARMA_DONT_USE_WRAPPER
    ARMA_USE_ARPACK
    ARMA_USE_SUPERLU
    ARMA_USE_OPENMP
    #ARMA_OPENMP_THREADS=12
)

add_library(
    libmb
    Basis.cpp
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

