// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "../Model.h"

class LinearChain : public Model {
 public:
  LinearChain(std::size_t n, double t, double u) : m_size{n}, m_t{t}, m_u{u} {}

  ~LinearChain() override {}

 private:
  std::vector<Term> hamiltonian() const override;

  std::size_t m_size;
  double m_t;
  double m_u;
};
