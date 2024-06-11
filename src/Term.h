// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <algorithm>
#include <cassert>
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

  std::string toString() const {
    std::string result =
        "Term { Coefficient: " + std::to_string(m_coefficient.real());
    if (m_coefficient.imag() != 0.0) {
      result += "+ i" + std::to_string(m_coefficient.imag());
    };
    result += ", Operators: [";
    for (const auto& op : m_operators) {
      result += op.toString() + ", ";
    }
    result += "] }";
    return result;
  }

  friend std::ostream& operator<<(std::ostream& os, const Term& term) {
    return os << term.toString();
  }

  Term product(const Term& other) const {
    std::vector<Operator> new_operators = m_operators;
    new_operators.insert(
        new_operators.end(), other.m_operators.begin(),
        other.m_operators.end());
    return Term(m_coefficient * other.m_coefficient, new_operators);
  }

  Term product(const std::vector<Operator>& operators) const {
    std::vector<Operator> new_operators = m_operators;
    new_operators.insert(
        new_operators.end(), operators.begin(), operators.end());
    return Term(m_coefficient, new_operators);
  }

  Term adjoint() const {
    std::vector<Operator> adj_operators;
    for (const auto& op : m_operators) {
      adj_operators.push_back(op.adjoint());
    }
    std::reverse(adj_operators.begin(), adj_operators.end());
    return Term(std::conj(m_coefficient), adj_operators);
  }

  Term negate() const { return Term(-m_coefficient, m_operators); }

  struct Factory;

 private:
  CoeffType m_coefficient;
  std::vector<Operator> m_operators;
  std::size_t m_swaps;
};

struct Term::Factory {
  Factory() = delete;

  template <Operator::Statistics S>
  static Term one_body(
      CoeffType coefficient, Operator::Spin spin1, std::uint8_t orbital1,
      Operator::Spin spin2, std::uint8_t orbital2) {
    return Term(
        coefficient, {Operator::creation<S>(spin1, orbital1),
                      Operator::annihilation<S>(spin2, orbital2)});
  }

  template <Operator::Statistics S>
  static Term two_body(
      CoeffType coefficient, Operator::Spin spin1, std::uint8_t orbital1,
      Operator::Spin spin2, std::uint8_t orbital2, Operator::Spin spin3,
      std::uint8_t orbital3, Operator::Spin spin4, std::uint8_t orbital4) {
    return Term(
        coefficient, {Operator::creation<S>(spin1, orbital1),
                      Operator::annihilation<S>(spin2, orbital2),
                      Operator::creation<S>(spin3, orbital3),
                      Operator::annihilation<S>(spin4, orbital4)});
  }

  template <Operator::Statistics S>
  static Term density_density(
      CoeffType coefficient, Operator::Spin spin1, std::uint8_t orbital1,
      Operator::Spin spin2, std::uint8_t orbital2) {
    return Term(
        coefficient, {Operator::creation<S>(spin1, orbital1),
                      Operator::annihilation<S>(spin1, orbital1),
                      Operator::creation<S>(spin2, orbital2),
                      Operator::annihilation<S>(spin2, orbital2)});
  }
};
