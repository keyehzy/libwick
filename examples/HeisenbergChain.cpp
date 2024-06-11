// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <armadillo>
#include <iomanip>

#include "Basis.h"
#include "FermionicBasis.h"
#include "Model.h"

class HeisenbergChain : public Model {
 public:
  HeisenbergChain(std::size_t n, double J, double h)
      : m_size{n}, m_J{J}, m_h{h} {}

 private:
  static constexpr auto spin_x = Expression::Factory::spin_x;
  static constexpr auto spin_z = Expression::Factory::spin_x;
  static constexpr auto spin_y = Expression::Factory::spin_x;

  std::vector<Term> hamiltonian() const override {
    // Not so efficient to use an Expression here, but constructing the
    // Hamiltonian is a small part of the program. On the other hand, we can use
    // some of the Expression methods to simplify the code, such as spin
    // operators.
    // TODO: Make compute_matrix_elements use Expression directly
    Expression e;

    // Magnetic field term
    for (std::size_t i = 0; i < m_size; i++) {
      e.insert(-m_h * spin_z(i));
    }

    // Heisenberg interaction term
    for (std::size_t i = 0; i < m_size - 1; i++) {
      e.insert(m_J * spin_x(i) * spin_x(i + 1));
      e.insert(m_J * spin_y(i) * spin_y(i + 1));
      e.insert(m_J * spin_z(i) * spin_z(i + 1));
    }
    e.insert(m_J * spin_x(m_size - 1) * spin_x(0));
    e.insert(m_J * spin_y(m_size - 1) * spin_y(0));
    e.insert(m_J * spin_z(m_size - 1) * spin_z(0));

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

int HeisenbergChainExample() {
  const int L = 8;
  const double eps = 1e-9;  // to break the FM degeneracy
  HeisenbergChain model(L, 1.0, eps);
  FermionicBasis basis(L, L, /*allow_double_occupancy=*/false);

  std::cout << basis.size() << " basis elements" << std::endl;

  arma::SpMat<std::complex<double>> m(basis.size(), basis.size());
  model.compute_matrix_elements(basis, m);

  // std::cout << arma::mat(m) << std::endl;

  arma::Col<std::complex<double>> eigval;
  arma::Mat<std::complex<double>> eigvec;

  arma::eigs_opts opts;
  opts.tol = 1e-9;
  opts.maxiter = 10000;

  bool ok = arma::eigs_gen(eigval, eigvec, m, 2, "sr", opts);

  if (!ok) {
    std::cerr << "Diagonalization failed" << std::endl;
    return 1;
  }

  const arma::Col<std::complex<double>>& ground_state = eigvec.col(0);
  std::vector<Term> sorted_terms =
      sorted_terms_from_eigvec(basis, ground_state);

  std::cout << std::setprecision(10);

  std::cout << "Ground state 1, energy per site: "
            << eigval(0) / static_cast<double>(L) << std::endl;

  std::cout << std::setprecision(5);

  const std::size_t states_to_print = 10;
  for (std::size_t i = 0; i < states_to_print; i++) {
    std::cout << std::setw(12)
              << sorted_terms[i].coefficient() * sorted_terms[i].coefficient()
              << "    " << basis.state_string(sorted_terms[i].operators())
              << std::endl;
  }
  return 0;
}
