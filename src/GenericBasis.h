// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Basis.h"

class GenericBasis : public Basis {
 public:
  GenericBasis(std::size_t n, std::size_t m) : Basis(n, m) { generate_basis(); }

  GenericBasis(std::size_t n, std::size_t m, BasisFilter *filter)
      : Basis(n, m, filter) {
    generate_basis();
  }

  void generate_combinations(BasisElement &, size_t, size_t, size_t) override;
};
