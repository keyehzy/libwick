// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "../Model.h"

class HubbardKagome : public Model {
 public:
  HubbardKagome(double t, double u) : m_t(t), m_u(u) {}

  static constexpr size_t size() { return 12; }

  static constexpr size_t nx() { return 1; }

  static constexpr size_t ny() { return 1; }

  static constexpr size_t index(int k, int i, int j) {
    return ((j % ny()) * nx() + (i % nx())) * size() + (k % size());
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
};
