#include "Term.h"

#include <gtest/gtest.h>

TEST(TermTest, ConstructorAndAccessors) {
  std::vector<Operator> operators = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)};
  Term term(2.5, operators);

  EXPECT_DOUBLE_EQ(term.coefficient(), 2.5);
  EXPECT_EQ(term.operators(), operators);
  EXPECT_EQ(term.operators().size(), 2);
  EXPECT_EQ(term.operators().at(0),
            Operator(OperatorType::CREATION, Spin::UP, 0));
  EXPECT_EQ(term.operators().at(1),
            Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1));
}

TEST(TermTest, EqualityOperator) {
  std::vector<Operator> operators1 = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)};
  Term term1(2.5, operators1);

  std::vector<Operator> operators2 = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)};
  Term term2(2.5, operators2);

  std::vector<Operator> operators3 = {
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1),
      Operator(OperatorType::CREATION, Spin::UP, 0)};
  Term term3(2.5, operators3);

  Term term4(3.0, operators1);

  EXPECT_TRUE(term1 == term2);
  EXPECT_FALSE(term1 == term3);
  EXPECT_FALSE(term1 == term4);
}

TEST(TermTest, InequalityOperator) {
  std::vector<Operator> operators1 = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)};
  Term term1(2.5, operators1);

  std::vector<Operator> operators2 = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)};
  Term term2(2.5, operators2);

  std::vector<Operator> operators3 = {
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1),
      Operator(OperatorType::CREATION, Spin::UP, 0)};
  Term term3(2.5, operators3);

  Term term4(3.0, operators1);

  EXPECT_FALSE(term1 != term2);
  EXPECT_TRUE(term1 != term3);
  EXPECT_TRUE(term1 != term4);
}

TEST(TermTest, ToString) {
  std::vector<Operator> operators = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)};
  Term term(2.5, operators);

  EXPECT_EQ(term.toString(),
            "Term { Coefficient: 2.500000, Operators: [Operator { Type: "
            "Creation, Spin: Up, Orbital: 0 }, Operator { Type: Annihilation, "
            "Spin: Down, Orbital: 1 }, ] }");
}

TEST(TermTest, OutputOperator) {
  std::vector<Operator> operators = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)};
  Term term(2.5, operators);

  std::ostringstream os;
  os << term;

  EXPECT_EQ(os.str(),
            "Term { Coefficient: 2.500000, Operators: [Operator { Type: "
            "Creation, Spin: Up, Orbital: 0 }, Operator { Type: Annihilation, "
            "Spin: Down, Orbital: 1 }, ] }");
}

TEST(TermTest, Product) {
  std::vector<Operator> operators1 = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)};
  Term term1(2.5, operators1);

  std::vector<Operator> operators2 = {
      Operator(OperatorType::CREATION, Spin::DOWN, 2),
      Operator(OperatorType::ANNIHILATION, Spin::UP, 3)};
  Term term2(3.0, operators2);

  Term product = term1.product(term2);

  std::vector<Operator> expected_operators = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1),
      Operator(OperatorType::CREATION, Spin::DOWN, 2),
      Operator(OperatorType::ANNIHILATION, Spin::UP, 3)};
  Term expected_product(7.5, expected_operators);

  EXPECT_EQ(product, expected_product);
}

TEST(TermTest, ProductWithVector) {
  std::vector<Operator> operators1 = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1)};
  Term term1(2.5, operators1);

  std::vector<Operator> operators2 = {
      Operator(OperatorType::CREATION, Spin::DOWN, 2),
      Operator(OperatorType::ANNIHILATION, Spin::UP, 3)};
  Term term2(3.0, operators2);

  std::vector<Term> terms = {term1, term2};

  Term product = term1.product(terms);

  std::vector<Operator> expected_operators = {
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1),
      Operator(OperatorType::CREATION, Spin::UP, 0),
      Operator(OperatorType::ANNIHILATION, Spin::DOWN, 1),
      Operator(OperatorType::CREATION, Spin::DOWN, 2),
      Operator(OperatorType::ANNIHILATION, Spin::UP, 3)};
  Term expected_product(18.75, expected_operators);

  EXPECT_EQ(product, expected_product);
}
