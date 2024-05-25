// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Basis.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unordered_set>

using testing::ElementsAre;
using testing::UnorderedElementsAre;

constexpr int binomial(int n, int k) {
  if (k == 0 || k == n) {
    return 1;
  }
  return binomial(n - 1, k - 1) + binomial(n - 1, k);
}

TEST(BasisTest, ConstructorAndAttributes) {
  Basis basis(2, 2, /*allow_double_occupancy=*/true);

  EXPECT_EQ(basis.particles(), 2);
  EXPECT_EQ(basis.orbitals(), 2);
  EXPECT_EQ(basis.elements().size(), binomial(4, 2));
}

TEST(BasisTest, BasisGeneration) {
  Basis basis(2, 2, /*allow_double_occupancy=*/true);
  EXPECT_EQ(basis.elements().size(), 6);
  EXPECT_THAT(
      basis.elements(),
      UnorderedElementsAre(
          std::vector<Operator>{Operator::creation(Operator::Spin::DOWN, 0),
                                Operator::creation(Operator::Spin::DOWN, 1)},
          std::vector<Operator>{Operator::creation(Operator::Spin::UP, 1),
                                Operator::creation(Operator::Spin::DOWN, 1)},
          std::vector<Operator>{Operator::creation(Operator::Spin::DOWN, 0),
                                Operator::creation(Operator::Spin::UP, 1)},
          std::vector<Operator>{Operator::creation(Operator::Spin::UP, 0),
                                Operator::creation(Operator::Spin::DOWN, 1)},
          std::vector<Operator>{Operator::creation(Operator::Spin::UP, 0),
                                Operator::creation(Operator::Spin::DOWN, 0)},
          std::vector<Operator>{Operator::creation(Operator::Spin::UP, 0),
                                Operator::creation(Operator::Spin::UP, 1)}));
}

TEST(BasisTest, BasisGenerationDisallowingDoubleOccupation) {
  Basis basis(2, 1, /*allow_double_occupancy=*/true);
  EXPECT_EQ(basis.elements().size(), 4);
  EXPECT_THAT(
      basis.elements(),
      UnorderedElementsAre(
          std::vector<Operator>{Operator::creation(Operator::Spin::DOWN, 0)},
          std::vector<Operator>{Operator::creation(Operator::Spin::UP, 0)},
          std::vector<Operator>{Operator::creation(Operator::Spin::DOWN, 1)},
          std::vector<Operator>{Operator::creation(Operator::Spin::UP, 1)}));
}

TEST(BasisTest, IndexingUnique) {
  Basis basis(2, 2, /*allow_double_occupancy=*/true);

  std::unordered_set<std::size_t> indices;
  for (const auto& term : basis.elements()) {
    indices.insert(basis.index(term));
  }
  EXPECT_EQ(indices.size(), basis.elements().size());

  for (const auto& term : basis.elements()) {
    EXPECT_GE(basis.index(term), 0);
    EXPECT_LT(basis.index(term), basis.elements().size());
  }
}

TEST(BasisTest, IndexingInsideBasis) {
  Basis basis(2, 2, /*allow_double_occupancy=*/true);

  std::vector<Operator> term = {Operator::creation(Operator::Spin::UP, 0),
                                Operator::creation(Operator::Spin::DOWN, 1)};
  EXPECT_TRUE(basis.contains(term));
}

TEST(BasisTest, IndexingOutsideBasis) {
  Basis basis(2, 2, /*allow_double_occupancy=*/true);

  std::vector<Operator> term = {Operator::creation(Operator::Spin::UP, 0),
                                Operator::creation(Operator::Spin::UP, 2)};
  EXPECT_FALSE(basis.contains(term));
}

TEST(BasisTest, EqualityOperator) {
  Basis basis1(2, 2, /*allow_double_occupancy=*/true);
  Basis basis2(2, 2, /*allow_double_occupancy=*/true);
  Basis basis3(2, 3, /*allow_double_occupancy=*/true);
  Basis basis4(3, 2, /*allow_double_occupancy=*/true);

  EXPECT_EQ(basis1, basis2);
  EXPECT_NE(basis1, basis3);
  EXPECT_NE(basis1, basis4);
}

TEST(BasisTest, IndexingEmptyTerm) {
  Basis basis(2, 2, /*allow_double_occupancy=*/true);

  std::vector<Operator> term = {};
  EXPECT_FALSE(basis.contains(term));
}

TEST(BasisTest, IndexingSingleTermSingleBodyBasis) {
  Basis basis(1, 1, /*allow_double_occupancy=*/true);

  std::vector<Operator> term = {Operator::creation(Operator::Spin::UP, 0)};
  EXPECT_TRUE(basis.contains(term));
}

TEST(BasisTest, IndexingSingleTermManyBodyBasis) {
  Basis basis(2, 2, /*allow_double_occupancy=*/true);

  std::vector<Operator> term = {Operator::creation(Operator::Spin::UP, 0)};
  EXPECT_FALSE(basis.contains(term));
}

TEST(BasisTest, SortBasis) {
  Basis basis(2, 2, /*allow_double_occupancy=*/true);

  auto sort_fn = [](const std::vector<Operator>& a,
                    const std::vector<Operator>& b) {
    int total_spin_a = 0;
    for (const auto& op : a) {
      total_spin_a += static_cast<int>(op.spin());
    }
    int total_spin_b = 0;
    for (const auto& op : b) {
      total_spin_b += static_cast<int>(op.spin());
    }
    return total_spin_a < total_spin_b;
  };
  basis.sort(sort_fn);

  std::vector<Operator> first{Operator::creation(Operator::Spin::UP, 0),
                              Operator::creation(Operator::Spin::UP, 1)};
  std::vector<Operator> last{Operator::creation(Operator::Spin::DOWN, 0),
                             Operator::creation(Operator::Spin::DOWN, 1)};
  EXPECT_EQ(*basis.elements().begin(), first);
  EXPECT_EQ(*basis.elements().rbegin(), last);
}
