#include "Operator.h"

#include <gtest/gtest.h>

TEST(OperatorTest, ConstructorAndGetter) {
  // Test creation operator with spin up and orbital 5
  Operator op1(OperatorType::CREATION, Spin::UP, 5);
  EXPECT_EQ(op1.type(), OperatorType::CREATION);
  EXPECT_EQ(op1.spin(), Spin::UP);
  EXPECT_EQ(op1.orbital(), 5);

  // Test annihilation operator with spin down and orbital 63
  Operator op2(OperatorType::ANNIHILATION, Spin::DOWN, 63);
  EXPECT_EQ(op2.type(), OperatorType::ANNIHILATION);
  EXPECT_EQ(op2.spin(), Spin::DOWN);
  EXPECT_EQ(op2.orbital(), 63);

  // Test raw data access
  EXPECT_EQ(op1.raw(), 0b00010100);  // Creation, Up, Orbital 5
  EXPECT_EQ(op2.raw(), 0b11111111);  // Annihilation, Down, Orbital 63

  // Test identifier
  EXPECT_EQ(op1.identifier(), 0b00001010);  // Creation, Up, Orbital 5
  EXPECT_EQ(op2.identifier(), 0b01111111);  // Annihilation, Down, Orbital 63
}

TEST(OperatorTest, CopyConstructorAndAssignment) {
  Operator op1(OperatorType::CREATION, Spin::UP, 10);

  // Test copy constructor
  Operator op2(op1);
  EXPECT_EQ(op1.type(), op2.type());
  EXPECT_EQ(op1.spin(), op2.spin());
  EXPECT_EQ(op1.orbital(), op2.orbital());

  // Test assignment operator
  Operator op3(OperatorType::ANNIHILATION, Spin::DOWN, 20);
  op3 = op1;
  EXPECT_EQ(op1.type(), op3.type());
  EXPECT_EQ(op1.spin(), op3.spin());
  EXPECT_EQ(op1.orbital(), op3.orbital());
}

TEST(OperatorTest, EqualityAndInequality) {
  Operator op1(OperatorType::CREATION, Spin::UP, 15);
  Operator op2(op1);
  Operator op3(OperatorType::ANNIHILATION, Spin::DOWN, 15);

  EXPECT_TRUE(op1 == op2);
  EXPECT_FALSE(op1 == op3);
  EXPECT_TRUE(op1 != op3);
  EXPECT_FALSE(op1 != op2);
}

TEST(OperatorTest, ToString) {
  Operator op1(OperatorType::CREATION, Spin::UP, 31);
  Operator op2(OperatorType::ANNIHILATION, Spin::DOWN, 0);

  EXPECT_EQ(op1.toString(),
            "Operator { Type: Creation, Spin: Up, Orbital: 31 }");
  EXPECT_EQ(op2.toString(),
            "Operator { Type: Annihilation, Spin: Down, Orbital: 0 }");
}

// test operator<<
TEST(OperatorTest, OutputOperator) {
  Operator op1(OperatorType::CREATION, Spin::UP, 15);
  Operator op2(OperatorType::ANNIHILATION, Spin::DOWN, 15);

  std::ostringstream os1;
  os1 << op1;
  EXPECT_EQ(os1.str(), "Operator { Type: Creation, Spin: Up, Orbital: 15 }");

  std::ostringstream os2;
  os2 << op2;
  EXPECT_EQ(os2.str(),
            "Operator { Type: Annihilation, Spin: Down, Orbital: 15 }");
}

TEST(OperatorTest, Swap) {
  Operator op1(OperatorType::CREATION, Spin::UP, 15);
  Operator op2(OperatorType::ANNIHILATION, Spin::DOWN, 15);

  op1.swap(op2);
  EXPECT_EQ(op1.type(), OperatorType::ANNIHILATION);
  EXPECT_EQ(op1.spin(), Spin::DOWN);
  EXPECT_EQ(op1.orbital(), 15);

  EXPECT_EQ(op2.type(), OperatorType::CREATION);
  EXPECT_EQ(op2.spin(), Spin::UP);
  EXPECT_EQ(op2.orbital(), 15);
}

TEST(OperatorTest, Hash) {
  Operator op1(OperatorType::CREATION, Spin::UP, 15);
  Operator op2(OperatorType::ANNIHILATION, Spin::DOWN, 15);

  std::hash<Operator> hash_fn;
  EXPECT_EQ(hash_fn(op1), op1.raw());
  EXPECT_EQ(hash_fn(op2), op2.raw());
}