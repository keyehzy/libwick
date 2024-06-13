// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Expression.h"

Expression add(const Term& a, const Term& b) {
  Expression result;
  result.insert(a);
  result.insert(b);
  return result;
}

using enum Operator::Statistics;
static constexpr auto ju = std::complex<double>{0, 1};

Expression spin_x(std::size_t i) {
  return add(
      one_body<Fermion>(1.0, Operator::Spin::Up, i, Operator::Spin::Down, i),
      one_body<Fermion>(1.0, Operator::Spin::Down, i, Operator::Spin::Up, i));
}

Expression spin_y(std::size_t i) {
  return add(
      one_body<Fermion>(
          1.0 * ju, Operator::Spin::Up, i, Operator::Spin::Down, i),
      one_body<Fermion>(
          -1.0 * ju, Operator::Spin::Down, i, Operator::Spin::Up, i));
}

Expression spin_z(std::size_t i) {
  return add(
      one_body<Fermion>(1.0, Operator::Spin::Up, i, Operator::Spin::Up, i),
      one_body<Fermion>(
          -1.0, Operator::Spin::Down, i, Operator::Spin::Down, i));
}
