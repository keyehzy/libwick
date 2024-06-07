// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "HubbardKagome.h"

static constexpr auto Fermion = Operator::Statistics::Fermion;

// clang-format off
void HubbardKagome::hopping_term(std::vector<Term>& result) const {
    size_t hex_size = size() / 2;
    for (Operator::Spin spin : {Operator::Spin::Up, Operator::Spin::Down}) {
    for (std::size_t i = 0; i < nx(); i++) {
        for (std::size_t j = 0; j < ny(); j++) {
        // Unitcell
        for (std::size_t k = 0; k < hex_size; k++) {
            std::vector<Term> terms{
                // Inner ring
                Term::Factory::one_body<Fermion>(-m_t, spin, index(k, i, j), spin, index((k + 1) % hex_size, i, j)),
                Term::Factory::one_body<Fermion>(-m_t, spin, index(k, i, j), spin, index((k + 1) % hex_size, i, j)).adjoint(),
                
                // Outer ring
                Term::Factory::one_body<Fermion>(-m_t, spin, index(hex_size + k, i, j), spin, index(k, i, j)),
                Term::Factory::one_body<Fermion>(-m_t, spin, index(hex_size + k, i, j), spin, index(k, i, j)).adjoint(),

                Term::Factory::one_body<Fermion>(-m_t, spin, index(hex_size + k, i, j), spin, index((k + 1) % hex_size, i, j)),
                Term::Factory::one_body<Fermion>(-m_t, spin, index(hex_size + k, i, j), spin, index((k + 1) % hex_size, i, j)).adjoint(),
            };

            result.insert(result.end(), terms.begin(), terms.end());
        }

        // PBC
        if (m_periodic) {
            std::vector<Term> terms{
                Term::Factory::one_body<Fermion>(-m_t, spin, index(6, i, j), spin, index(8, i - 1, j - 1)),
                Term::Factory::one_body<Fermion>(-m_t, spin, index(6, i, j), spin, index(8, i - 1, j - 1)).adjoint(),
    
                Term::Factory::one_body<Fermion>(-m_t, spin, index(6, i, j), spin, index(10, i, j - 1)),
                Term::Factory::one_body<Fermion>(-m_t, spin, index(6, i, j), spin, index(10, i, j - 1)).adjoint(),
    
                Term::Factory::one_body<Fermion>(-m_t, spin, index(7, i, j), spin, index(9, i, j - 1)),
                Term::Factory::one_body<Fermion>(-m_t, spin, index(7, i, j), spin, index(9, i, j - 1)).adjoint(),
    
                Term::Factory::one_body<Fermion>(-m_t, spin, index(7, i, j), spin, index(11, i + 1, j)),
                Term::Factory::one_body<Fermion>(-m_t, spin, index(7, i, j), spin, index(11, i + 1, j)).adjoint(),
    
                Term::Factory::one_body<Fermion>(-m_t, spin, index(8, i, j), spin, index(10, i + 1, j)),
                Term::Factory::one_body<Fermion>(-m_t, spin, index(8, i, j), spin, index(10, i + 1, j)).adjoint(),
    
                Term::Factory::one_body<Fermion>(-m_t, spin, index(9, i, j), spin, index(11, i, j + 1)),
                Term::Factory::one_body<Fermion>(-m_t, spin, index(9, i, j), spin, index(11, i, j + 1)).adjoint(),
                };
            result.insert(result.end(), terms.begin(), terms.end());
        }
        }
    }
    }
}
// clang-format on

void HubbardKagome::interaction_term(std::vector<Term>& result) const {
  for (size_t i1 = 0; i1 < size(); i1++) {
    result.push_back(Term::Factory::density_density<Fermion>(
        m_u, Operator::Spin::Up, i1, Operator::Spin::Down, i1));
  }
}
