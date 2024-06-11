// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Expression.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

using testing::IsEmpty;

using enum Operator::Type;
using enum Operator::Statistics;
using enum Operator::Spin;

TEST(ExpressionTest, ConstructorAndAccessors) {
  std::vector<Term> terms = {
      Term(
          2.5, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)}),
      Term(
          3.0, {Operator::creation<Fermion>(Down, 0),
                Operator::annihilation<Fermion>(Up, 1)})};
  Expression expression(terms);

  EXPECT_EQ(expression.terms().size(), 2);
  EXPECT_DOUBLE_EQ(
      expression.terms()
          .at(
              {Operator::creation<Fermion>(Up, 0),
               Operator::annihilation<Fermion>(Down, 1)})
          .real(),
      2.5);
  EXPECT_DOUBLE_EQ(
      expression.terms()
          .at(
              {Operator::creation<Fermion>(Down, 0),
               Operator::annihilation<Fermion>(Up, 1)})
          .real(),
      3.0);
}

TEST(ExpressionTest, EqualityOperator) {
  std::vector<Term> terms1 = {
      Term(
          2.5, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)}),
      Term(
          3.0, {Operator::creation<Fermion>(Down, 0),
                Operator::annihilation<Fermion>(Up, 1)})};
  Expression expression1(terms1);

  std::vector<Term> terms2 = {
      Term(
          2.5, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)}),
      Term(
          3.0, {Operator::creation<Fermion>(Down, 0),
                Operator::annihilation<Fermion>(Up, 1)})};
  Expression expression2(terms2);

  std::vector<Term> terms3 = {
      Term(
          3.0, {Operator::creation<Fermion>(Down, 0),
                Operator::annihilation<Fermion>(Up, 1)}),
      Term(
          2.5, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)})};
  Expression expression3(terms3);

  std::vector<Term> terms4 = {
      Term(
          2.5, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)}),
      Term(
          3.0, {Operator::creation<Fermion>(Down, 0),
                Operator::annihilation<Fermion>(Up, 1)}),
      Term(
          1.0, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)})};
  Expression expression4(terms4);

  EXPECT_EQ(expression1, expression2);
  EXPECT_EQ(expression1, expression3);
  EXPECT_NE(expression1, expression4);
}

TEST(ExpressionTest, InequalityOperator) {
  std::vector<Term> terms1 = {
      Term(
          2.5, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)}),
      Term(
          3.0, {Operator::creation<Fermion>(Down, 0),
                Operator::annihilation<Fermion>(Up, 1)})};
  Expression expression1(terms1);

  std::vector<Term> terms2 = {
      Term(
          2.5, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)}),
      Term(
          3.0, {Operator::creation<Fermion>(Down, 0),
                Operator::annihilation<Fermion>(Up, 1)})};
  Expression expression2(terms2);

  std::vector<Term> terms3 = {
      Term(
          3.0, {Operator::creation<Fermion>(Down, 0),
                Operator::annihilation<Fermion>(Up, 1)}),
      Term(
          2.5, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)})};
  Expression expression3(terms3);

  std::vector<Term> terms4 = {
      Term(
          2.5, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)}),
      Term(
          3.0, {Operator::creation<Fermion>(Down, 0),
                Operator::annihilation<Fermion>(Up, 1)}),
      Term(
          1.0, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Down, 1)})};
  Expression expression4(terms4);

  EXPECT_EQ(expression1, expression2);
  EXPECT_EQ(expression1, expression3);
  EXPECT_NE(expression1, expression4);
}

TEST(NormalOrderTest, ExpressionResultingInZero) {
  std::vector<Term> terms = {
      Term(
          1.0, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Up, 1)}),
      Term(
          -1.0, {Operator::creation<Fermion>(Up, 0),
                 Operator::annihilation<Fermion>(Up, 1)})};
  Expression expression(terms);

  std::vector<Term> normal_terms = {Term(
      0.0, {Operator::creation<Fermion>(Up, 0),
            Operator::annihilation<Fermion>(Up, 1)})};
  Expression expected(normal_terms);

  EXPECT_EQ(expression, expected);
}

TEST(NormalOrderTest, ExpressionResultingInZeroAfterClean) {
  std::vector<Term> terms = {
      Term(
          1.0, {Operator::creation<Fermion>(Up, 0),
                Operator::annihilation<Fermion>(Up, 1)}),
      Term(
          -1.0, {Operator::creation<Fermion>(Up, 0),
                 Operator::annihilation<Fermion>(Up, 1)})};
  Expression expression(terms);
  std::erase_if(expression.terms(), [](const auto &term) {
    return std::abs(term.second) < 1e-10;
  });

  EXPECT_THAT(expression.terms(), IsEmpty());
}
