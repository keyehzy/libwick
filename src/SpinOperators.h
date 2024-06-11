// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Expression.h"

namespace SpinOperators {
template <Operator::Statistics S>
Expression hopping(
    double t, Operator::Spin spin, std::size_t i, std::size_t j) {
  return add(
      Term::Factory::one_body<S>(-t, spin, i, spin, j),
      Term::Factory::one_body<S>(-t, spin, i, spin, j).adjoint());
}

// NOTE: These are only spin-1/2 operators. We implement them here in terms of
// spin flippings of Fermions. However, for spin-1 particles we would have to
// implement them in terms of Bosons.

Expression spin_x(std::size_t i);
Expression spin_y(std::size_t i);
Expression spin_z(std::size_t i);
}  // namespace SpinOperators
