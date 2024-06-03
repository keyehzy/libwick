![Build Status](https://github.com/keyehzy/cctb/actions/workflows/cmake.yml/badge.svg)
# LibMB

This project provides a C++ library for second-quantization calculations, enabling the representation and manipulation of quantum many-body systems using creation and annihilation operators. 

## Features

- **Symbolic representation of second-quantized operators:** Define Hamiltonians and other operators using a flexible and intuitive syntax.
- **Basis generation:**  Construct custom basis sets tailored to your problem, including restrictions on particle number, spin, and other quantum numbers.
- **Matrix representation:**  Efficiently compute matrix elements of operators in the chosen basis, facilitating numerical diagonalization.
- **Integration with external libraries:** Seamlessly interface with linear algebra libraries like Armadillo for powerful numerical computations.

## Usage

```bash
# Build the project using cmake
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run the tests
./build/tests/libwick-test
```

## Examples

In the following example, we construct a Hubbard chain model and compute the ground state using the Armadillo library.

```cpp
#include <armadillo>  //  for eigensolver
#include <iostream>

#include "Model.h"

class HubbardChain : public Model {
 public:
  HubbardChain(double t, double u, size_t n) : m_t(t), m_u(u), m_size(n) {}

 private:
  void HubbardChain::hopping_term(std::vector<Term>& result) const {
    for (Operator::Spin spin : {Operator::Spin::UP, Operator::Spin::DOWN}) {
      for (std::size_t i = 0; i < m_size; i++) {
        result.push_back(Term::Factory::one_body(-m_u, spin, i, spin, i));
      }
      for (std::size_t i = 0; i < m_size - 1; i++) {
        result.push_back(Term::Factory::one_body(-m_t, spin, i, spin, i + 1));
        result.push_back(
            Term::Factory::one_body(-m_t, spin, i, spin, i + 1).adjoint());
      }
      result.push_back(
          Term::Factory::one_body(-m_t, spin, m_size - 1, spin, 0));
      result.push_back(
          Term::Factory::one_body(-m_t, spin, m_size - 1, spin, 0).adjoint());
    }
  }

  void HubbardChain::interaction_term(std::vector<Term>& result) const {
    for (size_t i = 0; i < m_size; i++) {
      result.push_back(Term::Factory::density_density(
          m_u, Operator::Spin::UP, i, Operator::Spin::DOWN, i));
    }
  }

  std::vector<Term> hamiltonian() const override {
    std::vector<Term> result;
    hopping_term(result);
    interaction_term(result);
    return result;
  }

  double m_t;
  double m_u;
  size_t m_size;
};

int main() {
  const std::size_t size = 8;
  const std::size_t particles = 8;
  const std::size_t sz = 0;

  HubbardChain model(/*t=*/1.0, /*u=*/2.0, size, particles);

  // Construct a basis with total spin equal to zero
  Basis basis(size, particles, /*allow_double_occupancy=*/true,
              [](const Basis::BasisElement& element) {
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

1. The code defines a `HubbardChain` class, inheriting from a `Model` class, to
represent the Hubbard Hamiltonian. The hopping and interaction terms are
constructed using second-quantized operators.

2. A `Basis` object is created, specifying the Hilbert space for the calculation.
This example restricts the basis to states with a fixed particle number and
total spin projection.

3. The Hamiltonian's matrix representation in the chosen basis is computed. The
Armadillo library's `eigs_sym` function efficiently finds the lowest eigenvalues
and eigenvectors.

This example highlights LibMB's core functionalities, demonstrating its
flexibility in tackling quantum many-body problems. You can easily adapt this
code to explore different models, parameter regimes, or analysis techniques by
modifying the model definition, basis construction, or post-processing steps.

## License

This project is licensed under the BSD 2-Clause License - see the [LICENSE](LICENSE) file for details.
