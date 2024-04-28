#pragma once

// We encode a single term as list of operators and a coefficient.

#include <vector>

#include "Operator.h"

class Term {
 public:
  Term(double coefficient, const std::vector<Operator>& operators)
      : m_coefficient{coefficient}, m_operators{operators} {}

  double coefficient() const { return m_coefficient; }

  const std::vector<Operator>& operators() const { return m_operators; }

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

 private:
  double m_coefficient;
  std::vector<Operator> m_operators;
};