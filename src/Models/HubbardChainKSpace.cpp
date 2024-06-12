// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "HubbardChainKSpace.h"

#include <cmath>
#include <numbers>

static constexpr auto Fermion = Operator::Statistics::Fermion;

void HubbardChainKSpace::hopping_term(std::vector<Term>& result) const {
  auto index = [&](size_t i) {
    // The index will make the sequence
    // 0, 1, 2, ..., m_particles
    // look like
    // 0, 1, ..., m_particles / 2, -m_particles / 2, -m_particles / 2 - 1,
    // ...,  -1
    int lower = -m_particles / 2;
    int upper = m_particles / 2;
    return (i - lower) % (upper - lower + 1) + lower;
  };
  for (size_t i = 0; i < m_size; i++) {
    for (Operator::Spin spin : {Operator::Spin::Up, Operator::Spin::Down}) {
      double k = 2.0 * std::numbers::pi_v<double> *
                 static_cast<double>(index(i)) / static_cast<double>(m_size);
      double a = m_size == 2 ? 1 : 2;
      result.push_back(
          one_body<Fermion>(-a * m_t * std::cos(k), spin, i, spin, i));
    }
  }
}

void HubbardChainKSpace::interaction_term(std::vector<Term>& result) const {
  for (std::size_t k1 = 0; k1 < m_size; k1++) {
    for (std::size_t k2 = 0; k2 < m_size; k2++) {
      for (std::size_t k3 = 0; k3 < m_size; k3++) {
        for (std::size_t k4 = 0; k4 < m_size; k4++) {
          if (((k2 + k4) % m_size == (k1 + k3) % m_size)) {
            result.push_back(Term(
                m_u / static_cast<double>(m_size),
                {Operator::creation<Fermion>(Operator::Spin::Up, k1),
                 Operator::annihilation<Fermion>(Operator::Spin::Up, k2),
                 Operator::creation<Fermion>(Operator::Spin::Down, k3),
                 Operator::annihilation<Fermion>(Operator::Spin::Down, k4)}));
          }
        }
      }
    }
  }
}
