#include "NormalOrder.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <iostream>

using testing::IsEmpty;

TEST(NormalOrderTest, NormalOrderTermEqual) {
  Term term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                 Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderTermCreationCreation) {
  Term term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 1),
                  Operator(OperatorType::CREATION, Spin::UP, 0)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(-1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::CREATION, Spin::UP, 1)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderTermAnnihilationAnnihilation) {
  Term term(1.0, {Operator(OperatorType::ANNIHILATION, Spin::DOWN, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(-1.0, {Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1),
                  Operator(OperatorType::ANNIHILATION, Spin::DOWN, 0)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderTermCreationAnnihilation) {
  Term term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::DOWN, 0)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                 Operator(OperatorType::ANNIHILATION, Spin::DOWN, 0)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderTermAnnihilationCreationDifferentSpin) {
  Term term(1.0, {Operator(OperatorType::ANNIHILATION, Spin::DOWN, 0),
                  Operator(OperatorType::CREATION, Spin::UP, 0)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(-1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::DOWN, 0)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderTermAnnihilationCreationDifferentOrbital) {
  Term term(1.0, {Operator(OperatorType::ANNIHILATION, Spin::UP, 1),
                  Operator(OperatorType::CREATION, Spin::UP, 0)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(-1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 1)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderTermAnnihilationCreationSameSpinSameOrbital) {
  Term term(1.0, {Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                  Operator(OperatorType::CREATION, Spin::UP, 0)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(-1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0)}),
      Term(1.0, {})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest,
     NormalOrderTermCreationCreationAnnihilationDifferentOrbital) {
  Term term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 1),
                  Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(-1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::CREATION, Spin::UP, 1),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderTermCreationAnnihilationCreationSameOrbital) {
  Term term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                  Operator(OperatorType::CREATION, Spin::UP, 0)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(-1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0)}),
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest,
     NormalOrderTermAnnihilationCreationAnnihilationSameOrbital) {
  Term term(1.0, {Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                  Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(-1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0)}),
      Term(1.0, {Operator(OperatorType::ANNIHILATION, Spin::UP, 0)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest,
     NormalOrderTermAnnihilationAnnihilationCreationSameOrbital) {
  Term term(1.0, {Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                  Operator(OperatorType::CREATION, Spin::UP, 0)});
  Expression normal_ordered = normalOrder(term);

  std::vector<Term> terms = {
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                 Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                 Operator(OperatorType::ANNIHILATION, Spin::UP, 0)}),
      Term(0.0, {Operator(OperatorType::ANNIHILATION, Spin::UP, 0)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderExpression) {
  std::vector<Term> terms = {
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                 Operator(OperatorType::ANNIHILATION, Spin::UP, 1)}),
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 1),
                 Operator(OperatorType::ANNIHILATION, Spin::UP, 0)})};
  Expression expression(terms);
  Expression normal_ordered = normalOrder(expression);

  std::vector<Term> normal_terms = {
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                 Operator(OperatorType::ANNIHILATION, Spin::UP, 1)}),
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 1),
                 Operator(OperatorType::ANNIHILATION, Spin::UP, 0)})};
  Expression expected(normal_terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderExpressionWrongOrder) {
  std::vector<Term> terms = {
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 1),
                 Operator(OperatorType::CREATION, Spin::UP, 0)}),
      Term(1.0, {Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                 Operator(OperatorType::ANNIHILATION, Spin::UP, 1)})};
  Expression expression(terms);
  Expression normal_ordered = normalOrder(expression);

  std::vector<Term> normal_terms = {
      Term(-1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                  Operator(OperatorType::CREATION, Spin::UP, 1)}),
      Term(-1.0, {Operator(OperatorType::ANNIHILATION, Spin::UP, 1),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0)})};
  Expression expected(normal_terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderExpressionResultingInZero) {
  std::vector<Term> terms = {
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 1),
                 Operator(OperatorType::CREATION, Spin::UP, 0)}),
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                 Operator(OperatorType::CREATION, Spin::UP, 1)})};
  Expression expression(terms);
  Expression normal_ordered = normalOrder(expression);

  std::vector<Term> normal_terms = {
      Term(0.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                 Operator(OperatorType::CREATION, Spin::UP, 1)})};
  Expression expected(normal_terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest,
     NormalOrderTermAnnihilationAnnihilationCreationSameOrbitalAfterClean) {
  Term term(1.0, {Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                  Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                  Operator(OperatorType::CREATION, Spin::UP, 0)});
  Expression normal_ordered = normalOrder(term);
  normal_ordered.clean();

  std::vector<Term> terms = {
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                 Operator(OperatorType::ANNIHILATION, Spin::UP, 0),
                 Operator(OperatorType::ANNIHILATION, Spin::UP, 0)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderExpressionResultingInZeroAfterClean) {
  std::vector<Term> terms = {
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 1),
                 Operator(OperatorType::CREATION, Spin::UP, 0)}),
      Term(1.0, {Operator(OperatorType::CREATION, Spin::UP, 0),
                 Operator(OperatorType::CREATION, Spin::UP, 1)})};
  Expression expression(terms);
  Expression normal_ordered = normalOrder(expression);
  normal_ordered.clean();
  EXPECT_THAT(normal_ordered.terms(), IsEmpty());
}
