// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Basis.h"

class BosonicBasis final : public Basis {
 public:
  BosonicBasis(std::size_t n, std::size_t m) : Basis(n, m) { generate_basis(); }

  BosonicBasis(std::size_t n, std::size_t m, BasisFilter *filter)
      : Basis(n, m, filter) {
    generate_basis();
  }

  void generate_combinations(BasisElement &, size_t, size_t, size_t) override;
};
