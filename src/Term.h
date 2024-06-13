// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <algorithm>
#include <complex>
#include <vector>

#include "Operator.h"

class Term {
 public:
  using CoeffType = std::complex<double>;

  Term(CoeffType coefficient, const std::vector<Operator>& operators)
      : m_coefficient{coefficient}, m_operators{operators}, m_swaps{0} {}

  Term(
      CoeffType coefficient, const std::vector<Operator>& operators,
      std::size_t phase)
      : m_coefficient{coefficient}, m_operators{operators}, m_swaps{phase} {}

  CoeffType coefficient() const { return m_coefficient; }

  std::size_t swaps() const { return m_swaps; }

  void increment(std::size_t i) { m_swaps += i; }

  const std::vector<Operator>& operators() const { return m_operators; }

  std::vector<Operator>& operators() { return m_operators; }

  bool operator==(const Term& other) const {
    return m_coefficient == other.m_coefficient &&
           m_operators == other.m_operators;
  }

  bool operator!=(const Term& other) const { return !(*this == other); }

  friend std::ostream& operator<<(std::ostream& os, const Term& term);

  Term product(const Term& other) const;

  Term product(const std::vector<Operator>& operators) const;

  Term adjoint() const;

  Term negate() const { return Term(-m_coefficient, m_operators); }

 private:
  CoeffType m_coefficient;
  std::vector<Operator> m_operators;
  std::size_t m_swaps;
};

template <Operator::Statistics S>
Term one_body(
    Term::CoeffType coefficient, Operator::Spin spin1, std::size_t orbital1,
    Operator::Spin spin2, std::size_t orbital2) {
  return Term(
      coefficient, {Operator::creation<S>(spin1, orbital1),
                    Operator::annihilation<S>(spin2, orbital2)});
}

template <Operator::Statistics S>
Term two_body(
    Term::CoeffType coefficient, Operator::Spin spin1, std::size_t orbital1,
    Operator::Spin spin2, std::size_t orbital2, Operator::Spin spin3,
    std::size_t orbital3, Operator::Spin spin4, std::size_t orbital4) {
  return Term(
      coefficient, {Operator::creation<S>(spin1, orbital1),
                    Operator::annihilation<S>(spin2, orbital2),
                    Operator::creation<S>(spin3, orbital3),
                    Operator::annihilation<S>(spin4, orbital4)});
}

template <Operator::Statistics S>
Term density_density(
    Term::CoeffType coefficient, Operator::Spin spin1, std::size_t orbital1,
    Operator::Spin spin2, std::size_t orbital2) {
  return Term(
      coefficient, {Operator::creation<S>(spin1, orbital1),
                    Operator::annihilation<S>(spin1, orbital1),
                    Operator::creation<S>(spin2, orbital2),
                    Operator::annihilation<S>(spin2, orbital2)});
}
