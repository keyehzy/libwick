#pragma once

#include <cassert>
#include <functional>
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

  Term product(const Term& other) const {
    std::vector<Operator> new_operators = m_operators;
    new_operators.insert(new_operators.end(), other.m_operators.begin(),
                         other.m_operators.end());
    return Term(m_coefficient * other.m_coefficient, new_operators);
  }

  Term product(const std::vector<Term>& terms) const {
    Term result = *this;
    for (const auto& term : terms) {
      result = result.product(term);
    }
    return result;
  }

 private:
  double m_coefficient;
  std::vector<Operator> m_operators;
};

template <>
struct std::hash<Term> {
  size_t operator()(const Term& term) const {
    size_t hash = 0;
    hash_combine(hash, term.coefficient());
    for (const auto& op : term.operators()) {
      hash_combine(hash, std::hash<Operator>{}(op));
    }
    return hash;
  }

 private:
  template <typename T>
  void hash_combine(size_t& seed, const T& value) const {
    seed ^= std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
};