#include "Expression.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::IsEmpty;

TEST(ExpressionTest, ConstructorAndAccessors) {
  std::vector<Term> terms = {
      Term(2.5,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)}),
      Term(3.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::DOWN, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)})};
  Expression expression(terms);

  EXPECT_EQ(expression.terms().size(), 2);
  EXPECT_DOUBLE_EQ(
      expression.terms().at(
          {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                    Operator::Spin::UP, 0),
           Operator(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
                    Operator::Spin::DOWN, 1)}),
      2.5);
  EXPECT_DOUBLE_EQ(
      expression.terms().at(
          {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                    Operator::Spin::DOWN, 0),
           Operator(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
                    Operator::Spin::UP, 1)}),
      3.0);
}

TEST(ExpressionTest, EqualityOperator) {
  std::vector<Term> terms1 = {
      Term(2.5,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)}),
      Term(3.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::DOWN, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)})};
  Expression expression1(terms1);

  std::vector<Term> terms2 = {
      Term(2.5,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)}),
      Term(3.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::DOWN, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)})};
  Expression expression2(terms2);

  std::vector<Term> terms3 = {
      Term(3.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::DOWN, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)}),
      Term(2.5,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)})};
  Expression expression3(terms3);

  std::vector<Term> terms4 = {
      Term(2.5,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)}),
      Term(3.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::DOWN, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)}),
      Term(1.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)})};
  Expression expression4(terms4);

  EXPECT_EQ(expression1, expression2);
  EXPECT_EQ(expression1, expression3);
  EXPECT_NE(expression1, expression4);
}

TEST(ExpressionTest, InequalityOperator) {
  std::vector<Term> terms1 = {
      Term(2.5,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)}),
      Term(3.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::DOWN, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)})};
  Expression expression1(terms1);

  std::vector<Term> terms2 = {
      Term(2.5,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)}),
      Term(3.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::DOWN, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)})};
  Expression expression2(terms2);

  std::vector<Term> terms3 = {
      Term(3.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::DOWN, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)}),
      Term(2.5,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)})};
  Expression expression3(terms3);

  std::vector<Term> terms4 = {
      Term(2.5,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)}),
      Term(3.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::DOWN, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)}),
      Term(1.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::DOWN, 1)})};
  Expression expression4(terms4);

  EXPECT_EQ(expression1, expression2);
  EXPECT_EQ(expression1, expression3);
  EXPECT_NE(expression1, expression4);
}

TEST(NormalOrderTest, ExpressionResultingInZero) {
  std::vector<Term> terms = {
      Term(1.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)}),
      Term(-1.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)})};
  Expression expression(terms);

  std::vector<Term> normal_terms = {Term(
      0.0, {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)})};
  Expression expected(normal_terms);

  EXPECT_EQ(expression, expected);
}

TEST(NormalOrderTest, ExpressionResultingInZeroAfterClean) {
  std::vector<Term> terms = {
      Term(1.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Statistics::FERMION, Operator::Spin::UP, 1)}),
      Term(-1.0,
           {Operator(Operator::Type::CREATION, Operator::Statistics::FERMION,
                     Operator::Spin::UP, 0),
            Operator(Operator::Type::ANNIHILATION,
                     Operator::Operator::Statistics::FERMION,
                     Operator::Spin::UP, 1)})};
  Expression expression(terms);
  expression.clean();

  EXPECT_THAT(expression.terms(), IsEmpty());
}