// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "../Model.h"

class HubbardSquare : public Model {
 public:
  HubbardSquare(double t, double u, std::size_t nx, std::size_t ny)
      : m_t(t), m_u(u), m_nx(nx), m_ny(ny) {}

  ~HubbardSquare() override {}

  std::size_t size() const { return m_nx * m_ny; }

  std::size_t nx() const { return m_nx; }

  std::size_t ny() const { return m_ny; }

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
  std::size_t m_nx;
  std::size_t m_ny;
};
