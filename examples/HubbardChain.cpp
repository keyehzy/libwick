// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <armadillo>  //  for eigensolver
#include <iostream>

#include "Assert.h"
#include "FermionicBasis.h"
#include "Model.h"

using enum Operator::Type;        // for Creation, Annihilation
using enum Operator::Statistics;  // for Fermion
using enum Operator::Spin;        // Up, Down

class HubbardChain : public Model {
 public:
  HubbardChain(double t, double u, size_t n) : m_t(t), m_u(u), m_size(n) {}

 private:
  void hopping_term(std::vector<Term>& result) const {
    for (Operator::Spin spin : {Up, Down}) {
      // chemical potential
      for (std::size_t i = 0; i < m_size; i++) {
        result.push_back(one_body<Fermion>(-m_u, spin, i, spin, i));
      }

      // hopping term
      for (std::size_t i = 0; i < m_size - 1; i++) {
        result.push_back(one_body<Fermion>(-m_t, spin, i, spin, i + 1));
        result.push_back(
            one_body<Fermion>(-m_t, spin, i, spin, i + 1).adjoint());
      }
      result.push_back(one_body<Fermion>(-m_t, spin, m_size - 1, spin, 0));
      result.push_back(
          one_body<Fermion>(-m_t, spin, m_size - 1, spin, 0).adjoint());
    }
  }

  void interaction_term(std::vector<Term>& result) const {
    // interaction term
    for (size_t i = 0; i < m_size; i++) {
      result.push_back(density_density<Fermion>(m_u, Up, i, Down, i));
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

// Construct a basis with total spin equal to zero
class ZeroTotalSpinFilter : BasisFilter {
 public:
  bool filter(const BasisElement& element) const override {
    int total_spin = 0;
    for (const auto& op : element) {
      total_spin += op.spin() == Up ? 1 : -1;
    }
    return total_spin == 0;
  }
};

int main() {
  const std::size_t size = 8;
  const std::size_t particles = 8;

  HubbardChain model(/*t=*/1.0, /*u=*/2.0, size);
  FermionicBasis basis(size, particles, new ZeroTotalSpinFilter);

  // Compute matrix elements
  arma::SpMat<arma::cx_double> m(basis.size(), basis.size());
  model.compute_matrix_elements(basis, m);
  LIBMB_ASSERT(m.is_hermitian());

  // Compute ground state using, e.g. Armadillo library
  arma::cx_vec eigval;
  arma::cx_mat eigvec;
  const std::size_t eigval_count = 4;
  arma::eigs_gen(eigval, eigvec, m, eigval_count, "sr");

  std::cout << "Eigenvalues:" << std::endl;
  for (std::size_t i = 0; i < eigval.size(); i++) {
    std::cout << std::fixed << eigval(i).real() << std::endl;
  }

  // Perform some further analysis here...
}
