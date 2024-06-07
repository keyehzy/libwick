// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Expression.h"

static constexpr auto Fermion = Operator::Statistics::Fermion;

Expression add(const Term& a, const Term& b) {
  Expression result;
  result.insert(a);
  result.insert(b);
  return result;
}

Expression Expression::Factory::hopping(
    double t, Operator::Spin spin, std::size_t i, std::size_t j) {
  return add(
      Term::Factory::one_body<Fermion>(-t, spin, i, spin, j),
      Term::Factory::one_body<Fermion>(-t, spin, i, spin, j).adjoint());
}

Expression spin_x(double coeff, std::size_t i) {
  return add(
      Term::Factory::one_body<Fermion>(
          1.0 * coeff, Operator::Spin::Up, i, Operator::Spin::Down, i),
      Term::Factory::one_body<Fermion>(
          1.0 * coeff, Operator::Spin::Down, i, Operator::Spin::Up, i));
}

Expression spin_y(double coeff, std::size_t i) {
  // FIXME: We don't have complex numbers
  return add(
      Term::Factory::one_body<Fermion>(
          1.0 /*j*/ * coeff, Operator::Spin::Up, i, Operator::Spin::Down, i),
      Term::Factory::one_body<Fermion>(
          -1.0 /*j*/ * coeff, Operator::Spin::Down, i, Operator::Spin::Up, i));
}

Expression spin_z(double coeff, std::size_t i) {
  return add(
      Term::Factory::one_body<Fermion>(
          1.0 * coeff, Operator::Spin::Up, i, Operator::Spin::Up, i),
      Term::Factory::one_body<Fermion>(
          -1.0 * coeff, Operator::Spin::Down, i, Operator::Spin::Down, i));
}
