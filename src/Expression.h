// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <unordered_map>
#include <vector>

#include "Operator.h"
#include "Term.h"

class Expression {
 public:
  Expression() = default;

  using ExpressionMap = std::unordered_map<std::vector<Operator>, double>;

  Expression(const ExpressionMap& terms) : m_terms(terms) {}

  Expression(ExpressionMap&& terms) : m_terms(std::move(terms)) {}

  Expression(const std::vector<Term>& terms) {
    for (const auto& term : terms) {
      m_terms[term.operators()] += term.coefficient();
    }
  }

  void insert(const Term& term) {
    m_terms[term.operators()] += term.coefficient();
  }

  void insert(const Expression& other) {
    for (const auto& [operators, coefficient] : other.terms()) {
      m_terms[operators] += coefficient;
    }
  }

  const ExpressionMap& terms() const { return m_terms; }

  ExpressionMap& terms() { return m_terms; }

  bool operator==(const Expression& other) const {
    return m_terms == other.m_terms;
  }

  bool operator!=(const Expression& other) const { return !(*this == other); }

  Expression add(const Expression& other) const {
    Expression result(*this);
    for (const auto& [operators, coefficient] : other.terms()) {
      result.insert(Term(coefficient, operators));
    }
    return result;
  }

  Expression product(const Expression& other) const {
    Expression result;
    for (const auto& [operators_a, coefficient_a] : terms()) {
      for (const auto& [operators_b, coefficient_b] : other.terms()) {
        result.insert(Term(coefficient_a, operators_a)
                          .product(Term(coefficient_b, operators_b)));
      }
    }
    return result;
  }

  Expression adjoint() const {
    Expression result;
    for (const auto& [operators, coefficient] : terms()) {
      result.insert(Term(coefficient, operators).adjoint());
    }
    return result;
  }

  Expression negate() const {
    Expression result;
    for (const auto& [operators, coefficient] : terms()) {
      result.insert(Term(-coefficient, operators));
    }
    return result;
  }

  struct Factory {
    Factory() = delete;

    static Expression add(const Term&, const Term&);

    static Expression hopping(double, Operator::Spin, std::size_t, std::size_t);

    static Expression spin_x(double, std::size_t);

    static Expression spin_y(double, std::size_t);

    static Expression spin_z(double, std::size_t);
  };

 private:
  ExpressionMap m_terms;
};
