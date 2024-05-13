// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Operator.h"

#include <gtest/gtest.h>

TEST(OperatorTest, ConstructorAndGetterFermion) {
  // Test creation operator with spin up and orbital 5
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 5);
  EXPECT_EQ(op1.type(), Operator::Type::CREATION);
  EXPECT_EQ(op1.spin(), Operator::Spin::UP);
  EXPECT_EQ(op1.orbital(), 5);

  // Test annihilation operator with spin down and orbital 31
  Operator op2(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
               Operator::Spin::DOWN, 31);
  EXPECT_EQ(op2.type(), Operator::Type::ANNIHILATION);
  EXPECT_EQ(op2.spin(), Operator::Spin::DOWN);
  EXPECT_EQ(op2.orbital(), 31);

  // Test raw data access
  EXPECT_EQ(op1.raw(), 0b00101010);  // Creation, Fermion, Up, Orbital 5
  EXPECT_EQ(op2.raw(), 0b11111111);  // Annihilation, Fermion, Down, Orbital 31

  // Test identifier
  EXPECT_EQ(op1.identifier(), 0b0010101);  // Creation, Up, Orbital 5
  EXPECT_EQ(op2.identifier(), 0b1111111);  // Annihilation, Down, Orbital 31
}

TEST(OperatorTest, ConstructorAndGetterBoson) {
  // Test creation operator with spin up and orbital 5
  Operator op1(Operator::Type::CREATION, Operator::Statistics::BOSON,
               Operator::Spin::UP, 5);
  EXPECT_EQ(op1.type(), Operator::Type::CREATION);
  EXPECT_EQ(op1.spin(), Operator::Spin::UP);
  EXPECT_EQ(op1.orbital(), 5);

  // Test annihilation operator with spin down and orbital 31
  Operator op2(Operator::Type::ANNIHILATION, Operator::Statistics::BOSON,
               Operator::Spin::DOWN, 31);
  EXPECT_EQ(op2.type(), Operator::Type::ANNIHILATION);
  EXPECT_EQ(op2.spin(), Operator::Spin::DOWN);
  EXPECT_EQ(op2.orbital(), 31);

  // Test raw data access
  EXPECT_EQ(op1.raw(), 0b00101000);  // Creation, Boson, Up, Orbital 5
  EXPECT_EQ(op2.raw(), 0b11111101);  // Annihilation, Boson, Down, Orbital 31

  // Test identifier
  EXPECT_EQ(op1.identifier(), 0b0010100);  // Creation, Up, Orbital 5
  EXPECT_EQ(op2.identifier(), 0b1111110);  // Annihilation, Down, Orbital 31
}

TEST(OperatorTest, CopyConstructorAndAssignment) {
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 10);

  // Test copy constructor
  Operator op2(op1);
  EXPECT_EQ(op1.type(), op2.type());
  EXPECT_EQ(op1.spin(), op2.spin());
  EXPECT_EQ(op1.orbital(), op2.orbital());

  // Test assignment operator
  Operator op3(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
               Operator::Spin::DOWN, 20);
  op3 = op1;
  EXPECT_EQ(op1.type(), op3.type());
  EXPECT_EQ(op1.spin(), op3.spin());
  EXPECT_EQ(op1.orbital(), op3.orbital());
}

TEST(OperatorTest, EqualityAndInequality) {
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 15);
  Operator op2(op1);
  Operator op3(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
               Operator::Spin::DOWN, 15);

  EXPECT_EQ(op1, op2);
  EXPECT_NE(op1, op3);
  EXPECT_NE(op2, op3);
}

TEST(OperatorTest, EqualityAndInequalityFermionBoson) {
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 15);
  Operator op2(Operator::Type::CREATION, Operator::Statistics::BOSON,
               Operator::Spin::UP, 15);
  Operator op3(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::DOWN, 15);

  EXPECT_NE(op1, op2);
  EXPECT_NE(op1, op3);
  EXPECT_NE(op2, op3);
}

TEST(OperatorTest, ToString) {
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 31);
  Operator op2(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
               Operator::Spin::DOWN, 0);

  EXPECT_EQ(op1.toString(),
            "Operator { Type: Creation, Spin: Up, Orbital: 31 }");
  EXPECT_EQ(op2.toString(),
            "Operator { Type: Annihilation, Spin: Down, Orbital: 0 }");
}

// test operator<<
TEST(OperatorTest, OutputOperator) {
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 15);
  Operator op2(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
               Operator::Spin::DOWN, 15);

  std::ostringstream os1;
  os1 << op1;
  EXPECT_EQ(os1.str(), "Operator { Type: Creation, Spin: Up, Orbital: 15 }");

  std::ostringstream os2;
  os2 << op2;
  EXPECT_EQ(os2.str(),
            "Operator { Type: Annihilation, Spin: Down, Orbital: 15 }");
}

TEST(OperatorTest, Hash) {
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 15);
  Operator op2(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
               Operator::Spin::DOWN, 15);

  std::hash<Operator> hash_fn;
  EXPECT_EQ(hash_fn(op1), op1.raw());
  EXPECT_EQ(hash_fn(op2), op2.raw());
}

TEST(OperatorTest, SameHash) {
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 15);
  Operator op2(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 15);
  Operator op3(Operator::Type::CREATION, Operator::Statistics::BOSON,
               Operator::Spin::UP, 15);
  Operator op4(Operator::Type::CREATION, Operator::Statistics::BOSON,
               Operator::Spin::UP, 15);

  std::hash<Operator> hash_fn;
  EXPECT_EQ(hash_fn(op1), hash_fn(op2));
  EXPECT_EQ(hash_fn(op3), hash_fn(op4));
  EXPECT_NE(hash_fn(op1), hash_fn(op3));
  EXPECT_NE(hash_fn(op2), hash_fn(op4));
}

TEST(OperatorTest, DifferentHash) {
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 15);
  Operator op2(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
               Operator::Spin::DOWN, 15);
  Operator op3(Operator::Type::CREATION, Operator::Statistics::BOSON,
               Operator::Spin::UP, 15);

  std::hash<Operator> hash_fn;
  EXPECT_NE(hash_fn(op1), hash_fn(op2));
  EXPECT_NE(hash_fn(op1), hash_fn(op3));
}

TEST(OperatorTest, Adjoint) {
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 15);
  Operator op2(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
               Operator::Spin::DOWN, 15);

  EXPECT_EQ(op1.adjoint().type(), Operator::Type::ANNIHILATION);
  EXPECT_EQ(op1.adjoint().spin(), Operator::Spin::UP);
  EXPECT_EQ(op1.adjoint().orbital(), 15);

  EXPECT_EQ(op2.adjoint().type(), Operator::Type::CREATION);
  EXPECT_EQ(op2.adjoint().spin(), Operator::Spin::DOWN);
  EXPECT_EQ(op2.adjoint().orbital(), 15);
}

TEST(OperatorTest, AssertAdjointDoesNotChangeStatistics) {
  Operator op1(Operator::Type::CREATION, Operator::Statistics::FERMION,
               Operator::Spin::UP, 15);
  Operator op2(Operator::Type::ANNIHILATION, Operator::Statistics::FERMION,
               Operator::Spin::DOWN, 15);
  Operator op3(Operator::Type::CREATION, Operator::Statistics::BOSON,
               Operator::Spin::UP, 15);
  Operator op4(Operator::Type::ANNIHILATION, Operator::Statistics::BOSON,
               Operator::Spin::DOWN, 15);

  EXPECT_EQ(op1.adjoint().statistics(), Operator::Statistics::FERMION);
  EXPECT_EQ(op2.adjoint().statistics(), Operator::Statistics::FERMION);
  EXPECT_EQ(op3.adjoint().statistics(), Operator::Statistics::BOSON);
  EXPECT_EQ(op4.adjoint().statistics(), Operator::Statistics::BOSON);
}
