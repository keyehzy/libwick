// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "LinearChain.h"

std::vector<Term> LinearChain::hamiltonian() const {
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
    terms.push_back(Term::one_body(-m_t, spin, m_size - 1, spin, 0).adjoint());
  }
  return terms;
}