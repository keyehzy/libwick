// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "NormalOrder.h"

#include <vector>

// We assume that all the operators in the term have the same statistics
// i.e. they are all fermionic or all bosonic. Normal order between
// fermionic and bosonic operators is not well defined.

void sort_term(Term& term, std::vector<Term>& stack,
               std::vector<Operator>& elms) {
  if (term.operators().size() < 2) {
    return;
  }

  std::vector<Operator>& operators = term.operators();

  auto push_new_term = [&](size_t j) {
    elms.clear();
    elms.insert(elms.end(), operators.begin(), operators.end());
    elms.erase(elms.begin() + j - 1, elms.begin() + j + 1);
    stack.emplace_back(term.coefficient(), std::move(elms), term.swaps());
  };

  for (std::size_t i = 1; i < operators.size(); ++i) {
    for (std::size_t j = i; j > 0; --j) {
      Operator& op1 = operators[j - 1];
      Operator& op2 = operators[j];
      if (op1.type() == Operator::Type::CREATION &&
          op2.type() == Operator::Type::CREATION &&
          op1.identifier() > op2.identifier()) {
        std::swap(op1, op2);
        term.increment(op1.is_fermion() && op2.is_fermion());
      } else if (op1.type() == Operator::Type::ANNIHILATION &&
                 op2.type() == Operator::Type::ANNIHILATION &&
                 op1.identifier() < op2.identifier()) {
        std::swap(op1, op2);
        term.increment(op1.is_fermion() && op2.is_fermion());
      } else if (op1.type() == Operator::Type::ANNIHILATION &&
                 op2.type() == Operator::Type::CREATION) {
        if (op1.identifier() == op2.identifier()) {
          push_new_term(j);
        }
        std::swap(op1, op2);
        term.increment(op1.is_fermion() && op2.is_fermion());
      }
    }
  }
}

Expression normal_order(const Term& term) {
  std::vector<Term> stack = {term};
  Expression::ExpressionMap terms;
  std::vector<Operator> elms;
  elms.reserve(term.operators().size());
  while (!stack.empty()) {
    Term cur = stack.back();
    stack.pop_back();
    if (cur.operators().empty()) {
      terms[{}] += cur.coefficient();
      continue;
    }
    sort_term(cur, stack, elms);
    terms[cur.operators()] +=
        cur.swaps() % 2 == 0 ? cur.coefficient() : -cur.coefficient();
  }
  return Expression(terms);
}

Expression normal_order(const std::vector<Term>& terms) {
  Expression::ExpressionMap result;
  for (const Term& term : terms) {
    const Expression& e = normal_order(term);
    for (const auto& [term, coeff] : e.terms()) {
      result[term] += coeff;
    }
  }
  return Expression(std::move(result));
}

Expression normal_order(const Expression& expression) {
  Expression::ExpressionMap result;
  for (const auto& [term, coeff] : expression.terms()) {
    const Expression& e = normal_order(Term(coeff, term));
    for (const auto& [term, coeff] : e.terms()) {
      result[term] += coeff;
    }
  }
  return Expression(std::move(result));
}

Expression commute(const Term& term1, const Term& term2) {
  Term t1 = term1.product(term2);
  Term t2 = term2.product(term1);
  // t2.coefficient() *= -1;
  return Expression(std::vector<Term>{t1, t2});
}
