// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <gtest/gtest.h>

#include "Models/LinearChain.h"
#include "SparseMatrix.h"

TEST(ModelTest, LinearChain) {
  auto model = LinearChain(3, 1.0, 2.0);
  SparseMatrix<double> m;
  Basis basis(3, 1);
  model.compute_matrix_elements(basis, m);
  std::size_t diag = 3;
  std::size_t off_diag = 6;
  EXPECT_EQ(m.size(), 2 * diag + 2 * off_diag);

  // Check diagonal elements
  for (std::size_t i = 0; i < basis.size(); i++) {
    EXPECT_EQ(m(i, i), -2.0);
  }

  // Check off-diagonal elements
  for (std::size_t i = 0; i < basis.size(); i++) {
    EXPECT_EQ(m(i, (i + 2) % basis.size()), -1.0);
    EXPECT_EQ(m((i + 2) % basis.size(), i), -1.0);
  }
}
