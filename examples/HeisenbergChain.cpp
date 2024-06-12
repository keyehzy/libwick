// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <armadillo>
#include <iomanip>

#include "Basis.h"
#include "FermionicBasis.h"
#include "Model.h"
#include "SpinOperators.h"

class HeisenbergChain : public Model {
 public:
  HeisenbergChain(std::size_t n, double J, double h)
      : m_size{n}, m_J{J}, m_h{h} {}

 private:
  std::vector<Term> hamiltonian() const override {
    // Not so efficient to use an Expression here, but constructing the
    // Hamiltonian is a small part of the program. On the other hand, we can use
    // some of the Expression methods to simplify the code, such as spin
    // operators.
    Expression e;

    // Magnetic field term
    for (std::size_t i = 0; i < m_size; i++) {
      e.insert(-m_h * SpinOperators::spin_z(i));
    }

    // Heisenberg interaction term
    for (std::size_t i = 0; i < m_size - 1; i++) {
      e.insert(m_J * SpinOperators::spin_x(i) * SpinOperators::spin_x(i + 1));
      e.insert(m_J * SpinOperators::spin_y(i) * SpinOperators::spin_y(i + 1));
      e.insert(m_J * SpinOperators::spin_z(i) * SpinOperators::spin_z(i + 1));
    }
    e.insert(
        m_J * SpinOperators::spin_x(m_size - 1) * SpinOperators::spin_x(0));
    e.insert(
        m_J * SpinOperators::spin_y(m_size - 1) * SpinOperators::spin_y(0));
    e.insert(
        m_J * SpinOperators::spin_z(m_size - 1) * SpinOperators::spin_z(0));

    std::vector<Term> terms;
    for (const auto& [operators, coefficient] : e.terms()) {
      terms.push_back(Term(coefficient, operators));
    }

    return terms;
  }

  std::size_t m_size;
  double m_J;
  double m_h;
};

template <typename Vec>
std::vector<Term> sorted_terms_from_eigvec(
    const Basis& basis, const Vec& eigvec) {
  std::vector<Term> sorted_terms;
  sorted_terms.reserve(basis.size());

  for (std::size_t i = 0; i < basis.size(); i++) {
    sorted_terms.emplace_back(eigvec(i), basis.element(i));
  }

  std::sort(
      sorted_terms.begin(), sorted_terms.end(),
      [](const auto& a, const auto& b) {
        return std::abs(a.coefficient()) > std::abs(b.coefficient());
      });

  return sorted_terms;
}

int main() {
  const int L = 4;
  HeisenbergChain model(L, -4.0, 1e-4);
  FermionicBasis basis(L, L, /*allow_double_occupancy=*/false);

  std::cout << basis.size() << " basis elements" << std::endl;

  arma::SpMat<std::complex<double>> m(basis.size(), basis.size());
  model.compute_matrix_elements(basis, m);

  arma::Col<std::complex<double>> eigval;
  arma::Mat<std::complex<double>> eigvec;

  bool ok = arma::eigs_gen(eigval, eigvec, m, 1, "sr");

  if (!ok) {
    std::cerr << "Diagonalization failed" << std::endl;
    return 1;
  }

  const arma::Col<std::complex<double>>& ground_state = eigvec.col(0);
  std::vector<Term> sorted_terms =
      sorted_terms_from_eigvec(basis, ground_state);

  std::cout << "Ground state 1, energy per site: "
            << eigval(0) / static_cast<double>(L) << std::endl;

  std::cout << std::setprecision(5);

  const std::size_t states_to_print = 10;
  for (std::size_t i = 0; i < states_to_print; i++) {
    std::cout << std::setw(12)
              << sorted_terms[i].coefficient() *
                     std::conj(sorted_terms[i].coefficient())
              << "    " << basis.state_string(sorted_terms[i].operators())
              << std::endl;
  }
  return 0;
}
