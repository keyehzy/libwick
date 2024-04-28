#include "NormalOrder.h"

#include <gtest/gtest.h>

#include <iostream>

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