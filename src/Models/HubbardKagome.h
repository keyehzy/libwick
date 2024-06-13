// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "../Model.h"

class HubbardKagome : public Model {
 public:
  HubbardKagome(double t, double u, bool periodic)
      : m_t(t), m_u(u), m_periodic(periodic) {}

  ~HubbardKagome() override {}

  static constexpr std::size_t size = 12;

  static constexpr std::size_t nx = 1;

  static constexpr std::size_t ny = 1;

  static constexpr std::size_t index(int k, int i, int j) {
    return ((j % ny) * nx + (i % nx)) * size + (k % size);
  }

 private:
  void hopping_term(std::vector<Term>& result) const;

  void interaction_term(std::vector<Term>& result) const;

  std::vector<Term> hamiltonian() const override {
    std::vector<Term> result;
    hopping_term(result);
    interaction_term(result);
    return result;
  }

  double m_t;
  double m_u;
  bool m_periodic;
};
