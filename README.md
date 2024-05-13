# libwick

This project implements a second-quantization library in C++. It provides tools for representing and manipulating quantum many-body systems using creation and annihilation operators.

# Usage

```bash
# Build the project using cmake
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_COMPILER=clang++
cmake --build build

# Run the tests
./build/tests/libwick-test
```

# Examples

In the following example, we construct a Hubbard chain model and compute the ground state using the Armadillo library.

```cpp
#include <armadillo> //  for eigensolver
#include <iostream>

#include "Model.h"

int main() {
  const std::size_t size = 8;
  const std::size_t particles = 8;
  const std::size_t sz = 0;

  HubbardChain model(/*t=*/1.0, /*u=*/2.0, size, particles);

  // Construct a basis with total spin equal to zero
  Basis basis(size, particles, [](const Basis::BasisElement& element) {
    int total_spin = 0;
    for (const auto& op : element) {
      total_spin += op.spin() == Operator::Spin::UP ? 1 : -1;
    }
    return total_spin == sz;
  });

  // Compute matrix elements
  arma::sp_mat m(basis.size(), basis.size());
  model.compute_matrix_elements(basis, m);  

  // Compute ground state using, e.g. Armadillo library
  arma::vec eigval;
  arma::mat eigvec;
  const std::size_t eigval_count = 4;
  arma::eigs_sym(eigval, eigvec, m, eigval_count, "sa");

  std::cout << "Eigenvalues:" << std::endl;
  for (std::size_t i = 0; i < eigval.size(); i++) {
    std::cout << std::setprecision(10) << eigval(i) << std::endl;
  }

  for (std::size_t i = 0; i < eigval.size(); i++) {
    std::cout << "Ground state " << i << ":" << std::endl;
    ground_state_analysis(basis, eigvec.col(i));
  }
}
```

# License

This project is licensed under the BSD 2-Clause License - see the [LICENSE](LICENSE) file for details.
