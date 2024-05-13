#pragma once

#include "Basis.h"
#include "NormalOrder.h"
#include "Term.h"

class Model {
 public:
  template <typename SpMat>
  void compute_matrix_elements(const Basis& basis, SpMat& mat) {
    const std::vector<Term>& hamilt = hamiltonian();
#pragma omp parallel for schedule(dynamic)
    for (const auto& basis_element : basis.elements()) {
      std::size_t basis_index = basis.index(basis_element);
      std::vector<Term> terms;
      terms.reserve(hamilt.size());
      for (const Term& hamilt_term : hamilt) {
        terms.push_back(hamilt_term.product(basis_element));
      }
      const Expression& e = normal_order(terms);
      for (const auto& [term, coeff] : e.terms()) {
        if (term.back().type() == Operator::Type::CREATION &&
            basis.contains(term)) {
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
  LinearChain(std::size_t n, double t, double u) : m_size{n}, m_t{t}, m_u{u} {}

 private:
  std::vector<Term> hamiltonian() const override {
    std::vector<Term> terms;
    for (Operator::Spin spin : {Operator::Spin::UP, Operator::Spin::DOWN}) {
      for (std::size_t i = 0; i < m_size; i++) {
        terms.push_back(Term::one_body(-m_u, spin, i, spin, i));
      }
      for (std::size_t i = 0; i < m_size - 1; i++) {
        terms.push_back(Term::one_body(-m_t, spin, i, spin, i + 1));
        terms.push_back(Term::one_body(-m_t, spin, i, spin, i + 1).adjoint());
      }
      terms.push_back(Term::one_body(-m_t, spin, m_size - 1, spin, 0));
      terms.push_back(
          Term::one_body(-m_t, spin, m_size - 1, spin, 0).adjoint());
    }
    return terms;
  }

  std::size_t m_size;
  double m_t;
  double m_u;
};

class HubbardChain : public Model {
 public:
  HubbardChain(double t, double u, size_t n) : m_t(t), m_u(u), m_size(n) {}

 private:
  void hopping_term(std::vector<Term>& result) const {
    for (Operator::Spin spin : {Operator::Spin::UP, Operator::Spin::DOWN}) {
      for (std::size_t i = 0; i < m_size; i++) {
        result.push_back(Term::one_body(-m_u, spin, i, spin, i));
      }
      for (std::size_t i = 0; i < m_size - 1; i++) {
        result.push_back(Term::one_body(-m_t, spin, i, spin, i + 1));
        result.push_back(Term::one_body(-m_t, spin, i, spin, i + 1).adjoint());
      }
      result.push_back(Term::one_body(-m_t, spin, m_size - 1, spin, 0));
      result.push_back(
          Term::one_body(-m_t, spin, m_size - 1, spin, 0).adjoint());
    }
  }

  void interaction_term(std::vector<Term>& result) const {
    for (size_t i1 = 0; i1 < m_size; i1++) {
      result.push_back(
          Term::term(m_u, Operator::creation(Operator::Spin::UP, i1),
                     Operator::annihilation(Operator::Spin::UP, i1),
                     Operator::creation(Operator::Spin::DOWN, i1),
                     Operator::annihilation(Operator::Spin::DOWN, i1)));
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