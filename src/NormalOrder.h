// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Expression.h"

// We assume that all the operators in the term have the same statistics
// i.e. they are all fermionic or all bosonic. Normal order between
// fermionic and bosonic operators is not well defined.

class NormalOrderer {
 public:
  using OperatorsPhasePair = std::pair<std::vector<Operator>, std::size_t>;

  NormalOrderer(const Term& term);

  NormalOrderer(const std::vector<Term>& terms);

  NormalOrderer(const Expression& expression);

  NormalOrderer(const std::vector<Expression>& expressions);

  Expression::ExpressionMap terms() const { return m_terms_map; }

  Expression expression() const { return Expression(m_terms_map); }

 private:
  void normal_order(
      const std::vector<Operator>& operators, Term::CoeffType coefficient);

  OperatorsPhasePair sort_operators(
      std::vector<Operator> operators, std::size_t phase);

  std::vector<OperatorsPhasePair> m_stack;
  Expression::ExpressionMap m_terms_map;
  std::vector<Operator> m_elements;
};

Expression commute(const Term& term1, const Term& term2);
Expression commute(
    const Expression& expression1, const Expression& expression2);
Expression anticommute(const Term& term1, const Term& term2);
Expression anticommute(
    const Expression& expression1, const Expression& expression2);
