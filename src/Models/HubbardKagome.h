// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Model.h"

class HubbardKagome : public Model {
 public:
  HubbardKagome(double t, double u, bool periodic)
      : m_t(t), m_u(u), m_periodic(periodic) {}

  ~HubbardKagome() override {}

  static constexpr std::size_t size = 12;

  static constexpr std::size_t nx = 1;

  static constexpr std::size_t ny = 1;

  static constexpr std::size_t index(
      std::size_t k, std::size_t i, std::size_t j) {
    return ((j % ny) * nx + (i % nx)) * size + (k % size);
  }

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
  bool m_periodic;
};
