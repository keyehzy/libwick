// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Assert.h"
#include "Operator.h"

using BasisElement = std::vector<Operator>;

class BasisFilter {
 public:
  virtual bool filter(const BasisElement&) const { return true; }
  virtual ~BasisFilter() = default;
};

class TotalSpinFilter : public BasisFilter {
 public:
  TotalSpinFilter(int total_spin) : m_total_spin(total_spin) {}

  bool filter(const BasisElement& element) const override {
    int s = 0;
    for (const auto& op : element) {
      s += 2 * static_cast<int>(op.spin()) - 1;
    }
    return s == m_total_spin;
  }

  ~TotalSpinFilter() override {}

 private:
  int m_total_spin;
};

class SingleSpinBasisFilter : public BasisFilter {
 public:
  SingleSpinBasisFilter(Operator::Spin spin) : m_spin(spin) {}

  bool filter(const BasisElement& e) const override {
    LIBMB_ASSERT(e.size() == 1);
    return e.back().spin() == m_spin;
  }

  ~SingleSpinBasisFilter() override {}

 private:
  Operator::Spin m_spin;
};
