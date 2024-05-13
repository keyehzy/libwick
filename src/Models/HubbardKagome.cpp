// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "HubbardKagome.h"

// clang-format off
void HubbardKagome::hopping_term(std::vector<Term>& result) const {
    size_t hex_size = size() / 2;
    for (Operator::Spin spin : {Operator::Spin::UP, Operator::Spin::DOWN}) {
    for (std::size_t i = 0; i < nx(); i++) {
        for (std::size_t j = 0; j < ny(); j++) {
        // Unitcell
        for (std::size_t k = 0; k < hex_size; k++) {
            std::vector<Term> terms{
                // Inner ring
                Term::one_body(-m_t, spin, index(k, i, j), spin, index((k + 1) % hex_size, i, j)),
                Term::one_body(-m_t, spin, index((k + 1) % hex_size, i, j), spin, index(k, i, j)),

                // Outer ring
                Term::one_body(-m_t, spin, index(hex_size + k, i, j), spin, index(k, i, j)),
                Term::one_body(-m_t, spin, index(k, i, j), spin, index(hex_size + k, i, j)),

                Term::one_body(-m_t, spin, index(hex_size + k, i, j), spin, index((k + 1) % hex_size, i, j)),
                Term::one_body(-m_t, spin, index((k + 1) % hex_size, i, j), spin, index(hex_size + k, i, j))};

            result.insert(result.end(), terms.begin(), terms.end());
        }

        // PBC
        std::vector<Term> terms{
            Term::one_body(-m_t, spin, index(6, i, j), spin, index(8, i - 1, j - 1)),
            Term::one_body(-m_t, spin, index(8, i - 1, j - 1), spin, index(6, i, j)),

            Term::one_body(-m_t, spin, index(6, i, j), spin, index(10, i, j - 1)),
            Term::one_body(-m_t, spin, index(10, i, j - 1), spin, index(6, i, j)),

            Term::one_body(-m_t, spin, index(7, i, j), spin, index(9, i, j - 1)),
            Term::one_body(-m_t, spin, index(9, i, j - 1), spin, index(7, i, j)),

            Term::one_body(-m_t, spin, index(7, i, j), spin, index(11, i + 1, j)),
            Term::one_body(-m_t, spin, index(11, i + 1, j), spin, index(7, i, j)),

            Term::one_body(-m_t, spin, index(8, i, j), spin, index(10, i + 1, j)),
            Term::one_body(-m_t, spin, index(10, i + 1, j), spin, index(8, i, j)),

            Term::one_body(-m_t, spin, index(9, i, j), spin, index(11, i, j + 1)),
            Term::one_body(-m_t, spin, index(11, i, j + 1), spin, index(9, i, j))};
        result.insert(result.end(), terms.begin(), terms.end());
        }
    }
    }
}
// clang-format on

void HubbardKagome::interaction_term(std::vector<Term>& result) const {
  for (size_t i1 = 0; i1 < size(); i1++) {
    result.push_back(Term::density_density(
        m_u, Operator::Spin::UP, i1, Operator::Spin::UP, i1,
        Operator::Spin::DOWN, i1, Operator::Spin::DOWN, i1));
  }
}