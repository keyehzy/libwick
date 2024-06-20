// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "HubbardKagome.h"

using enum Operator::Statistics;

void HubbardKagome::hopping_term(std::vector<Term>& result) const {
  Expression e;
  for (Operator::Spin spin : {Operator::Spin::Up, Operator::Spin::Down}) {
    // Inner ring
    e += hopping<Fermion>(-m_t, spin, 0, 1);
    e += hopping<Fermion>(-m_t, spin, 1, 2);
    e += hopping<Fermion>(-m_t, spin, 2, 3);
    e += hopping<Fermion>(-m_t, spin, 3, 4);
    e += hopping<Fermion>(-m_t, spin, 4, 5);
    e += hopping<Fermion>(-m_t, spin, 5, 0);

    e += hopping<Fermion>(-m_t, spin, 0, 6);
    e += hopping<Fermion>(-m_t, spin, 1, 7);
    e += hopping<Fermion>(-m_t, spin, 2, 8);
    e += hopping<Fermion>(-m_t, spin, 3, 9);
    e += hopping<Fermion>(-m_t, spin, 4, 10);
    e += hopping<Fermion>(-m_t, spin, 5, 11);

    e += hopping<Fermion>(-m_t, spin, 1, 6);
    e += hopping<Fermion>(-m_t, spin, 2, 7);
    e += hopping<Fermion>(-m_t, spin, 3, 8);
    e += hopping<Fermion>(-m_t, spin, 4, 9);
    e += hopping<Fermion>(-m_t, spin, 5, 10);
    e += hopping<Fermion>(-m_t, spin, 0, 11);

    // PBC
    if (m_periodic) {
      e += hopping<Fermion>(-m_t, spin, 6, 10);
      e += hopping<Fermion>(-m_t, spin, 7, 11);
      e += hopping<Fermion>(-m_t, spin, 8, 6);
      e += hopping<Fermion>(-m_t, spin, 9, 7);
      e += hopping<Fermion>(-m_t, spin, 10, 8);
      e += hopping<Fermion>(-m_t, spin, 11, 9);
    }
  }
  std::vector<Term> terms;
  for (const auto& [operators, coefficient] : e.terms()) {
    terms.emplace_back(coefficient, operators);
  }
  result.insert(result.end(), terms.begin(), terms.end());
}

void HubbardKagome::interaction_term(std::vector<Term>& result) const {
  for (size_t i1 = 0; i1 < size; i1++) {
    result.push_back(density_density<Fermion>(
        m_u, Operator::Spin::Up, i1, Operator::Spin::Down, i1));
  }
}
