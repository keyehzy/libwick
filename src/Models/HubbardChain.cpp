// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "HubbardChain.h"

#include "Operator.h"

static constexpr auto Fermion = Operator::Statistics::Fermion;

void HubbardChain::hopping_term(std::vector<Term>& result) const {
  for (Operator::Spin spin : {Operator::Spin::Up, Operator::Spin::Down}) {
    for (std::size_t i = 0; i < m_size; i++) {
      result.push_back(
          Term::Factory::one_body<Fermion>(-m_u, spin, i, spin, i));
    }
    for (std::size_t i = 0; i < m_size - 1; i++) {
      result.push_back(
          Term::Factory::one_body<Fermion>(-m_t, spin, i, spin, i + 1));
      result.push_back(
          Term::Factory::one_body<Fermion>(-m_t, spin, i, spin, i + 1)
              .adjoint());
    }
    result.push_back(
        Term::Factory::one_body<Fermion>(-m_t, spin, m_size - 1, spin, 0));
    result.push_back(
        Term::Factory::one_body<Fermion>(-m_t, spin, m_size - 1, spin, 0)
            .adjoint());
  }
}

void HubbardChain::interaction_term(std::vector<Term>& result) const {
  for (size_t i1 = 0; i1 < m_size; i1++) {
    result.push_back(Term::Factory::density_density<Fermion>(
        m_u, Operator::Spin::Up, i1, Operator::Spin::Down, i1));
  }
}
