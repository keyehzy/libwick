// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Model.h"

class HeisenbergChain : public Model {
 public:
  HeisenbergChain(std::size_t n, double J, double h)
      : m_size{n}, m_J{J}, m_h{h} {}

  ~HeisenbergChain() override {}

  std::size_t size() const { return m_size; }

 private:
  Expression hamiltonian() const override {
    Expression result;

    for (std::size_t i = 0; i < m_size; i++) {
      result += -m_h * spin_z(i);
    }

    for (std::size_t i = 0; i < m_size; i++) {
      result += m_J * spin_x(i) * spin_x((i + 1) % m_size);
      result += m_J * spin_y(i) * spin_y((i + 1) % m_size);
      result += m_J * spin_z(i) * spin_z((i + 1) % m_size);
    }
    return result;
  }

  std::size_t m_size;
  double m_J;
  double m_h;
};
