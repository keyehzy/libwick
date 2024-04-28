#include <gmock/gmock-matchers.h>
#include <gmock/gmock-more-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Operator.h"

// Test the Operator class
TEST(OperatorTest, TypeReturnsCorrectValue) {
  {
    Operator op(OperatorType::CREATION, Spin::UP, 3);
    EXPECT_EQ(OperatorType::CREATION, op.type());
  }
  {
    Operator op(OperatorType::ANNIHILATION, Spin::DOWN, 3);
    EXPECT_EQ(OperatorType::ANNIHILATION, op.type());
  }
}
 