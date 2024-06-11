// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "SpinOperators.h"

namespace SpinOperators {
static constexpr auto Fermion = Operator::Statistics::Fermion;
static constexpr auto ju = std::complex<double>{0, 1};

Expression spin_x(std::size_t i) {
  return add(
      Term::Factory::one_body<Fermion>(
          1.0, Operator::Spin::Up, i, Operator::Spin::Down, i),
      Term::Factory::one_body<Fermion>(
          1.0, Operator::Spin::Down, i, Operator::Spin::Up, i));
}

Expression spin_y(std::size_t i) {
  return add(
      Term::Factory::one_body<Fermion>(
          1.0 * ju, Operator::Spin::Up, i, Operator::Spin::Down, i),
      Term::Factory::one_body<Fermion>(
          -1.0 * ju, Operator::Spin::Down, i, Operator::Spin::Up, i));
}

Expression spin_z(std::size_t i) {
  return add(
      Term::Factory::one_body<Fermion>(
          1.0, Operator::Spin::Up, i, Operator::Spin::Up, i),
      Term::Factory::one_body<Fermion>(
          -1.0, Operator::Spin::Down, i, Operator::Spin::Down, i));
}
}  // namespace SpinOperators
