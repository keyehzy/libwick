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
    Basis.cpp
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

find_package(OpenMP)

if (OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found: ${OpenMP_CXX_LIBRARIES}")
    target_link_libraries(
        libwick
        OpenMP::OpenMP_CXX
    )
endif()

find_package(BLAS)

if (BLAS_FOUND)
    message(STATUS "BLAS found: ${BLAS_LIBRARIES}")
endif()

find_package(Armadillo)

if (Armadillo_FOUND)
    message(STATUS "Armadillo found: ${ARMADILLO_LIBRARIES}")
    target_link_libraries(
        libwick
        ${ARMADILLO_LIBRARIES}
        BLAS::BLAS
        superlu
        arpack
        m
        gfortran
    )
    target_compile_definitions(
        libwick
        PRIVATE
        ARMA_DONT_USE_WRAPPER
        ARMA_USE_ARPACK
        ARMA_USE_SUPERLU
        ARMA_USE_OPENMP
        ARMA_OPENMP_THREADS=12
    )
endif()
