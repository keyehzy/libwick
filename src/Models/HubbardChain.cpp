// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "HubbardChain.h"

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
    result.push_back(Term::Factory::one_body(-m_t, spin, m_size - 1, spin, 0));
    result.push_back(
        Term::Factory::one_body(-m_t, spin, m_size - 1, spin, 0).adjoint());
  }
}

void HubbardChain::interaction_term(std::vector<Term>& result) const {
  for (size_t i1 = 0; i1 < m_size; i1++) {
    result.push_back(
        Term(m_u, {Operator::creation(Operator::Spin::UP, i1),
                   Operator::annihilation(Operator::Spin::UP, i1),
                   Operator::creation(Operator::Spin::DOWN, i1),
                   Operator::annihilation(Operator::Spin::DOWN, i1)}));
  }
}