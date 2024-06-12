// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "BosonicBasis.h"

void BosonicBasis::generate_combinations(
    BasisElement& current, size_t first_orbital, size_t depth,
    size_t max_depth) {
  if (depth == max_depth && m_basis_filter->filter(current)) {
    m_basis_map.insert(current);
    return;
  }

  for (size_t orbital_index = first_orbital; orbital_index < m_orbitals;
       orbital_index++) {
    // TODO: bosonic operator should be integer spin
    Operator::Spin spin = Operator::Spin::Up;
    if (current.empty() || current.back().orbital() <= orbital_index) {
      current.push_back(Operator(
          Operator::Type::Creation, Operator::Statistics::Boson, spin,
          orbital_index));
      generate_combinations(current, orbital_index, depth + 1, max_depth);
      current.pop_back();
    }
  }
}
