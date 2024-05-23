// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Expression.h"

Expression add(const Term& a, const Term& b) {
  Expression result;
  result.insert(a);
  result.insert(b);
  return result;
}

Expression hopping(double t, Operator::Spin spin, std::size_t i,
                   std::size_t j) {
  return add(Term::Factory::one_body(-t, spin, i, spin, j),
             Term::Factory::one_body(-t, spin, i, spin, j).adjoint());
}

Expression spin_x(double coeff, std::size_t i) {
  return add(Term::Factory::one_body(1.0 * coeff, Operator::Spin::UP, i,
                                     Operator::Spin::DOWN, i),
             Term::Factory::one_body(1.0 * coeff, Operator::Spin::DOWN, i,
                                     Operator::Spin::UP, i));
}

Expression spin_y(double coeff, std::size_t i) {
  // FIXME: We don't have complex numbers
  return add(Term::Factory::one_body(1.0 /*j*/ * coeff, Operator::Spin::UP, i,
                                     Operator::Spin::DOWN, i),
             Term::Factory::one_body(-1.0 /*j*/ * coeff, Operator::Spin::DOWN,
                                     i, Operator::Spin::UP, i));
}

Expression spin_z(double coeff, std::size_t i) {
  return add(Term::Factory::one_body(1.0 * coeff, Operator::Spin::UP, i,
                                     Operator::Spin::UP, i),
             Term::Factory::one_body(-1.0 * coeff, Operator::Spin::DOWN, i,
                                     Operator::Spin::DOWN, i));
}
