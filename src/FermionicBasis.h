// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Basis.h"

class FermionicBasis final : public Basis {
 public:
  FermionicBasis(std::size_t n, std::size_t m, bool allow_double_occupancy)
      : Basis(n, m), m_allow_double_occupancy{allow_double_occupancy} {
    generate_basis();
  }

  FermionicBasis(
      std::size_t n, std::size_t m, BasisFilter *filter,
      bool allow_double_occupancy)
      : Basis(n, m, filter), m_allow_double_occupancy{allow_double_occupancy} {
    generate_basis();
  }

  void generate_combinations(BasisElement &, size_t, size_t, size_t) override;

 private:
  bool m_allow_double_occupancy = true;
};
