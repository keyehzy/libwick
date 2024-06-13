// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Term.h"

std::ostream& operator<<(std::ostream& os, const Term& term) {
  os << "Term { Coefficient: " << term.coefficient();
  os << ", Operators: [";
  for (size_t i = 0; i < term.operators().size() - 1; ++i) {
    os << term.operators()[i].toString() << ", ";
  }
  os << term.operators()[term.operators().size() - 1].toString();
  os << "]}";
  return os;
}

Term Term::product(const Term& other) const {
  std::vector<Operator> new_operators = m_operators;
  new_operators.insert(
      new_operators.end(), other.m_operators.begin(), other.m_operators.end());
  return Term(m_coefficient * other.m_coefficient, new_operators);
}

Term Term::product(const std::vector<Operator>& operators) const {
  std::vector<Operator> new_operators = m_operators;
  new_operators.insert(new_operators.end(), operators.begin(), operators.end());
  return Term(m_coefficient, new_operators);
}

Term Term::adjoint() const {
  std::vector<Operator> adj_operators;
  for (const auto& op : m_operators) {
    adj_operators.push_back(op.adjoint());
  }
  std::reverse(adj_operators.begin(), adj_operators.end());
  return Term(std::conj(m_coefficient), adj_operators);
}
