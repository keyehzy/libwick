#include "SparseMatrix.h"

#include "gtest/gtest.h"

TEST(SparseMatrixTest, DefaultConstructor) {
  SparseMatrix<int> matrix;
  EXPECT_EQ(matrix.size(), 0);
}

TEST(SparseMatrixTest, OperatorParentheses) {
    SparseMatrix<int> matrix;
    matrix(0, 0) = 1;
    matrix(1, 1) = 2;
    matrix(2, 2) = 3;
    matrix(3, 3) = 4;
    matrix(4, 4) = 5;
    
    EXPECT_EQ(matrix(0, 0), 1);
    EXPECT_EQ(matrix(1, 1), 2);
    EXPECT_EQ(matrix(2, 2), 3);
    EXPECT_EQ(matrix(3, 3), 4);
    EXPECT_EQ(matrix(4, 4), 5);
}

TEST(SparseMatrixTest, OperatorParenthesesConst) {
    SparseMatrix<int> matrix;
    matrix(0, 0) = 1;
    matrix(1, 1) = 2;
    matrix(2, 2) = 3;
    matrix(3, 3) = 4;
        
    EXPECT_EQ(matrix(0, 0), 1);
    EXPECT_EQ(matrix(1, 1), 2);
    EXPECT_EQ(matrix(2, 2), 3);
    EXPECT_EQ(matrix(3, 3), 4);
    EXPECT_EQ(matrix(4, 4), 0); // Default value
}

TEST(SparseMatrixTest, Size) {
    SparseMatrix<int> matrix;
    matrix(0, 0) = 1;
    matrix(1, 1) = 2;
    matrix(2, 2) = 3;
    matrix(3, 3) = 4;
    matrix(4, 4) = 5;
    
    EXPECT_EQ(matrix.size(), 5);
}

TEST(SparseMatrixTest, EqualityOperator) {
    SparseMatrix<int> matrix1;
    matrix1(0, 0) = 1;
    matrix1(1, 1) = 2;
    matrix1(2, 2) = 3;
    matrix1(3, 3) = 4;
    matrix1(4, 4) = 5;
    
    SparseMatrix<int> matrix2;
    matrix2(0, 0) = 1;
    matrix2(1, 1) = 2;
    matrix2(2, 2) = 3;
    matrix2(3, 3) = 4;
    matrix2(4, 4) = 5;
    
    SparseMatrix<int> matrix3;
    matrix3(0, 0) = 1;
    matrix3(1, 1) = 2;
    matrix3(2, 2) = 3;
    matrix3(3, 3) = 4;
    matrix3(4, 4) = 6;
    
    EXPECT_EQ(matrix1, matrix2);
    EXPECT_NE(matrix1, matrix3);
}
