cmake_minimum_required(VERSION 3.18)

find_package(BLAS)
find_package(Armadillo)

add_executable(
  wick-example
  main.cpp
  HubbardSquare4x4.cpp
)

target_include_directories(
    wick-example
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    wick-example
    libwick
    BLAS::BLAS
    armadillo
    superlu
    arpack
    m
    gfortran
)

target_compile_definitions(
    wick-example
    PUBLIC
    ARMA_DONT_USE_WRAPPER
    ARMA_USE_ARPACK
    ARMA_USE_SUPERLU
    ARMA_USE_OPENMP
    #ARMA_OPENMP_THREADS=12
)
