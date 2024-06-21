// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Model.h"

class HubbardChainKSpace : public Model {
 public:
  HubbardChainKSpace(double t, double u, std::size_t n, std::size_t m)
      : m_t(t), m_u(u), m_size(n), m_particles(m) {}

  ~HubbardChainKSpace() override {}

 private:
  void hopping_term(Expression& result) const;

  void interaction_term(Expression& result) const;

  Expression hamiltonian() const override {
    Expression result;
    hopping_term(result);
    interaction_term(result);
    return result;
  }

  double m_t;
  double m_u;
  size_t m_size;
  size_t m_particles;
};
