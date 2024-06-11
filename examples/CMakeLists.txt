cmake_minimum_required(VERSION 3.18)

find_package(OpenMP)
find_package(BLAS)
find_package(Armadillo)

function(add_example example_file)
  get_filename_component(
    example_name
    ${example_file}
    NAME_WE
  )

  add_executable(
    ${example_name}
    ${example_file}
  )

  target_include_directories(
    ${example_name}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ARMADILLO_INCLUDE_DIRS}
  )


  target_link_libraries(
    ${example_name}
    PRIVATE
    libmb
    BLAS::BLAS
    OpenMP::OpenMP_CXX
    ${ARMADILLO_LIBRARIES}
    superlu
    arpack
  )

  target_compile_definitions(
    ${example_name}
    PRIVATE
    ARMA_DONT_USE_WRAPPER
    ARMA_USE_ARPACK
    ARMA_USE_SUPERLU
    ARMA_USE_OPENMP
  )
endfunction()

file(GLOB EXAMPLE_FILES
  HubbardChain.cpp
  HubbardSquare4x4.cpp
  HeisenbergChain.cpp
)

foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
    add_example(${EXAMPLE_FILE})
endforeach()
