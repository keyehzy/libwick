// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "NormalOrderer.h"

#include <vector>

constexpr Term::CoeffType evaluate_parity(
    Term::CoeffType coefficient, std::size_t phase) {
  return phase % 2 == 0 ? coefficient : -coefficient;
}

NormalOrderer::NormalOrderer(const Term& term) {
  normal_order(term.operators(), term.coefficient());
}

NormalOrderer::NormalOrderer(const std::vector<Term>& terms) {
  for (const Term& term : terms) {
    normal_order(term.operators(), term.coefficient());
  }
}

NormalOrderer::NormalOrderer(const Expression& expression) {
  for (const auto& [operators, coeff] : expression.terms()) {
    normal_order(operators, coeff);
  }
}

NormalOrderer::NormalOrderer(const std::vector<Expression>& expressions) {
  for (const Expression& expression : expressions) {
    for (const auto& [operators, coeff] : expression.terms()) {
      normal_order(operators, coeff);
    }
  }
}

void NormalOrderer::normal_order(
    const std::vector<Operator>& operators, Term::CoeffType coefficient) {
  m_stack.emplace_back(operators, 0);
  m_elements.reserve(operators.size());

  while (!m_stack.empty()) {
    auto [prev_operators, prev_phase] = std::move(m_stack.back());
    m_stack.pop_back();

    if (prev_operators.size() < 2) {
      m_terms_map[prev_operators] += evaluate_parity(coefficient, prev_phase);
      continue;
    }

    auto [new_operators, new_phase] = merge_sort(prev_operators, prev_phase);
    m_terms_map[new_operators] += evaluate_parity(coefficient, new_phase);
  }
}

NormalOrderer::OperatorsPhasePair NormalOrderer::sort_operators(
    std::vector<Operator> operators, std::size_t phase) {
  for (std::size_t i = 1; i < operators.size(); ++i) {
    for (std::size_t j = i; j > 0; --j) {
      Operator& op1 = operators[j - 1];
      Operator& op2 = operators[j];
      if (op1.type() == Operator::Type::Creation &&
          op2.type() == Operator::Type::Creation &&
          op1.identifier() > op2.identifier()) {
        std::swap(op1, op2);
        phase += op1.is_fermion() && op2.is_fermion();
      } else if (
          op1.type() == Operator::Type::Annihilation &&
          op2.type() == Operator::Type::Annihilation &&
          op1.identifier() < op2.identifier()) {
        std::swap(op1, op2);
        phase += op1.is_fermion() && op2.is_fermion();
      } else if (
          op1.type() == Operator::Type::Annihilation &&
          op2.type() == Operator::Type::Creation) {
        if (op1.identifier() == op2.identifier()) {
          m_elements.clear();
          m_elements.insert(
              m_elements.end(), operators.begin(), operators.end());
          m_elements.erase(
              m_elements.begin() + j - 1, m_elements.begin() + j + 1);
          m_stack.emplace_back(m_elements, phase);
        }
        std::swap(op1, op2);
        phase += op1.is_fermion() && op2.is_fermion();
      }
    }
  }
  return OperatorsPhasePair{operators, phase};
}

NormalOrderer::OperatorsPhasePair NormalOrderer::merge(
    std::vector<Operator>& left, std::vector<Operator>& right,
    std::size_t phase) {
  std::vector<Operator> result;
  std::size_t i = 0, j = 0;

  while (i < left.size() && j < right.size()) {
    Operator op1 = left[i];
    Operator op2 = right[j];

    if (op1.type() == Operator::Type::Creation &&
        op2.type() == Operator::Type::Creation &&
        op1.identifier() > op2.identifier()) {
      result.push_back(op2);
      phase += op1.is_fermion() && op2.is_fermion();
      j++;
    } else if (
        op1.type() == Operator::Type::Annihilation &&
        op2.type() == Operator::Type::Annihilation &&
        op1.identifier() < op2.identifier()) {
      result.push_back(op2);
      phase += op1.is_fermion() && op2.is_fermion();
      j++;
    } else if (
        op1.type() == Operator::Type::Annihilation &&
        op2.type() == Operator::Type::Creation) {
      result.push_back(op2);
      phase += op1.is_fermion() && op2.is_fermion();
      j++;
    } else {
      result.push_back(op1);
      i++;
    }
  }

  while (i < left.size()) {
    result.push_back(left[i++]);
  }

  while (j < right.size()) {
    result.push_back(right[j++]);
  }

  return OperatorsPhasePair{result, phase};
}

NormalOrderer::OperatorsPhasePair NormalOrderer::merge_sort(
    std::vector<Operator>& operators, std::size_t phase) {
  if (operators.size() <= 1) {
    return OperatorsPhasePair{operators, phase};
  }

  std::size_t mid = operators.size() / 2;
  std::vector<Operator> left(operators.begin(), operators.begin() + mid);
  std::vector<Operator> right(operators.begin() + mid, operators.end());

  OperatorsPhasePair left_result = merge_sort(left, phase);
  OperatorsPhasePair right_result = merge_sort(right, left_result.second);

  return merge(left_result.first, right_result.first, right_result.second);
}

Expression commute(const Term& term1, const Term& term2) {
  return NormalOrderer({term1.product(term2), term2.product(term1).negate()})
      .expression();
}

Expression commute(
    const Expression& expression1, const Expression& expression2) {
  return NormalOrderer({expression1.product(expression2),
                        expression2.product(expression1).negate()})
      .expression();
}

Expression anticommute(const Term& term1, const Term& term2) {
  return NormalOrderer({term1.product(term2), term2.product(term1)})
      .expression();
}

Expression anticommute(
    const Expression& expression1, const Expression& expression2) {
  return NormalOrderer({expression1.product(expression2),
                        expression2.product(expression1)})
      .expression();
  ;
}
