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

  struct Factory;

 private:
  ExpressionMap m_terms;
};

struct Expression::Factory {
  Factory() = delete;

  static constexpr auto Fermion = Operator::Statistics::Fermion;

  static Expression add(const Term&, const Term&);

  template <Operator::Statistics S>
  static Expression hopping(
      double t, Operator::Spin spin, std::size_t i, std::size_t j) {
    return add(
        Term::Factory::one_body<S>(-t, spin, i, spin, j),
        Term::Factory::one_body<S>(-t, spin, i, spin, j).adjoint());
  }

  // NOTE: These are only spin-1/2 operators. We implement them here in terms of
  // spin flippings of Fermions. However, for spin-1 particles we would have to
  // implement them in terms of Bosons.

  static Expression spin_x(double coeff, std::size_t i) {
    return add(
        Term::Factory::one_body<Fermion>(
            1.0 * coeff, Operator::Spin::Up, i, Operator::Spin::Down, i),
        Term::Factory::one_body<Fermion>(
            1.0 * coeff, Operator::Spin::Down, i, Operator::Spin::Up, i));
  }

  static Expression spin_y(double coeff, std::size_t i) {
    // FIXME: We don't have complex numbers
    return add(
        Term::Factory::one_body<Fermion>(
            1.0 /*j*/ * coeff, Operator::Spin::Up, i, Operator::Spin::Down, i),
        Term::Factory::one_body<Fermion>(
            -1.0 /*j*/ * coeff, Operator::Spin::Down, i, Operator::Spin::Up,
            i));
  }

  static Expression spin_z(double coeff, std::size_t i) {
    return add(
        Term::Factory::one_body<Fermion>(
            1.0 * coeff, Operator::Spin::Up, i, Operator::Spin::Up, i),
        Term::Factory::one_body<Fermion>(
            -1.0 * coeff, Operator::Spin::Down, i, Operator::Spin::Down, i));
  }
};
