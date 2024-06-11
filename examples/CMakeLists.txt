cmake_minimum_required(VERSION 3.18)

find_package(OpenMP)
find_package(BLAS)
find_package(Armadillo)

add_executable(
  libmb-example
  main.cpp
  HubbardSquare4x4.cpp
  HeisenbergChain.cpp
)

target_include_directories(
    libmb-example
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ARMADILLO_INCLUDE_DIRS}
)

target_link_libraries(
    libmb-example
    libmb
    BLAS::BLAS
    OpenMP::OpenMP_CXX
    ${ARMADILLO_LIBRARIES}
    superlu
    arpack
)

target_compile_definitions(
    libmb-example
    PUBLIC
    ARMA_DONT_USE_WRAPPER
    ARMA_USE_ARPACK
    ARMA_USE_SUPERLU
    ARMA_USE_OPENMP
)
