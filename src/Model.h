#pragma once

#include "Basis.h"
#include "NormalOrder.h"
#include "Term.h"

class Model {
 public:
  template <typename SpMat>
  void compute_matrix_elements(const Basis& basis, SpMat& mat) {
    std::vector<Term> hamilt = hamiltonian();
#pragma omp parallel for
    for (const auto& basis_element : basis.elements()) {
      std::size_t basis_index = basis.index(basis_element);
      std::vector<Term> terms;
      for (const Term& hamilt_term : hamilt) {
        terms.push_back(hamilt_term.product(basis_element));
      }
      Expression e = normal_order(terms);
      for (const auto& [term, coeff] : e.terms()) {
        if (basis.contains(term)) {
          std::size_t term_index = basis.index(term);
#pragma omp critical
          mat(basis_index, term_index) = coeff;
        }
      }
    }
  }

 private:
  virtual std::vector<Term> hamiltonian() const = 0;
};

class LinearChain : public Model {
 public:
  LinearChain(std::size_t n, double t, double u) : m_n{n}, m_t{t}, m_u{u} {}

 private:
  std::vector<Term> hamiltonian() const override {
    std::vector<Term> terms;
    for (Spin spin : {Spin::UP, Spin::DOWN}) {
      for (std::size_t i = 0; i < m_n; i++) {
        terms.push_back(Term::one_body(-m_u, spin, i, spin, i));
      }
      for (std::size_t i = 0; i < m_n - 1; i++) {
        terms.push_back(Term::one_body(-m_t, spin, i, spin, i + 1));
        terms.push_back(Term::one_body(-m_t, spin, i, spin, i + 1).adjoint());
      }
      terms.push_back(Term::one_body(-m_t, spin, m_n - 1, spin, 0));
      terms.push_back(Term::one_body(-m_t, spin, m_n - 1, spin, 0).adjoint());
    }
    return terms;
  }

  std::size_t m_n;
  double m_t;
  double m_u;
};