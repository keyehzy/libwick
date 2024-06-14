// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Operator.h"

using BasisElement = std::vector<Operator>;

class BasisFilter {
 public:
  virtual bool filter(const BasisElement&) const { return true; }
  virtual ~BasisFilter() = default;
};
