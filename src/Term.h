#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

#include "Operator.h"

class Term {
 public:
  Term(double coefficient, const std::vector<Operator>& operators)
      : m_coefficient{coefficient}, m_operators{operators}, m_swaps{0} {}

  Term(double coefficient, const std::vector<Operator>& operators,
       std::size_t phase)
      : m_coefficient{coefficient}, m_operators{operators}, m_swaps{phase} {}

  double coefficient() const { return m_coefficient; }

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
        "Term { Coefficient: " + std::to_string(m_coefficient) +
        ", Operators: [";
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
    new_operators.insert(new_operators.end(), other.m_operators.begin(),
                         other.m_operators.end());
    return Term(m_coefficient * other.m_coefficient, new_operators);
  }

  Term product(const std::vector<Operator>& operators) const {
    std::vector<Operator> new_operators = m_operators;
    new_operators.insert(new_operators.end(), operators.begin(),
                         operators.end());
    return Term(m_coefficient, new_operators);
  }

  template <typename... Args>
  Term product(const Term& term, Args... args) const {
    return product(term).product(args...);
  }

  Term adjoint() const {
    std::vector<Operator> adj_operators;
    for (const auto& op : m_operators) {
      adj_operators.push_back(op.adjoint());
    }
    std::reverse(adj_operators.begin(), adj_operators.end());
    // TODO(m): coefficient should be a complex number
    // and we should take the complex conjugate.
    return Term(m_coefficient, adj_operators);
  }

  static Term one_body(double coefficient, Operator::Spin spin1,
                       std::uint8_t orbital1, Operator::Spin spin2,
                       std::uint8_t orbital2) {
    return Term(coefficient,
                std::vector<Operator>{Operator::creation(spin1, orbital1),
                                      Operator::annihilation(spin2, orbital2)});
  }

  template <typename... Args>
  static Term term(double coefficient, Args... args) {
    std::vector<Operator> operators{args...};
    return Term(coefficient, operators);
  }

 private:
  double m_coefficient;
  std::vector<Operator> m_operators;
  std::size_t m_swaps;
};
