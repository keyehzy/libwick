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
  FermionicBasis basis(2, 2, /*allow_double_occupancy=*/true);

  EXPECT_EQ(basis.particles(), 2);
  EXPECT_EQ(basis.orbitals(), 2);
  EXPECT_EQ(basis.elements().size(), binomial(4, 2));
}

TEST(BasisTest, BasisGeneration) {
  FermionicBasis basis(2, 2, /*allow_double_occupancy=*/true);
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
  FermionicBasis basis(2, 1, /*allow_double_occupancy=*/true);
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
  FermionicBasis basis(2, 2, /*allow_double_occupancy=*/true);

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
  FermionicBasis basis(2, 2, /*allow_double_occupancy=*/true);

  std::vector<Operator> term = {Operator::creation(Operator::Spin::UP, 0),
                                Operator::creation(Operator::Spin::DOWN, 1)};
  EXPECT_TRUE(basis.contains(term));
}

TEST(BasisTest, IndexingOutsideBasis) {
  FermionicBasis basis(2, 2, /*allow_double_occupancy=*/true);

  std::vector<Operator> term = {Operator::creation(Operator::Spin::UP, 0),
                                Operator::creation(Operator::Spin::UP, 2)};
  EXPECT_FALSE(basis.contains(term));
}

TEST(BasisTest, EqualityOperator) {
  FermionicBasis basis1(2, 2, /*allow_double_occupancy=*/true);
  FermionicBasis basis2(2, 2, /*allow_double_occupancy=*/true);
  FermionicBasis basis3(2, 3, /*allow_double_occupancy=*/true);
  FermionicBasis basis4(3, 2, /*allow_double_occupancy=*/true);

  EXPECT_EQ(basis1, basis2);
  EXPECT_NE(basis1, basis3);
  EXPECT_NE(basis1, basis4);
}

TEST(BasisTest, IndexingEmptyTerm) {
  FermionicBasis basis(2, 2, /*allow_double_occupancy=*/true);

  std::vector<Operator> term = {};
  EXPECT_FALSE(basis.contains(term));
}

TEST(BasisTest, IndexingSingleTermSingleBodyBasis) {
  FermionicBasis basis(1, 1, /*allow_double_occupancy=*/true);

  std::vector<Operator> term = {Operator::creation(Operator::Spin::UP, 0)};
  EXPECT_TRUE(basis.contains(term));
}

TEST(BasisTest, IndexingSingleTermManyBodyBasis) {
  FermionicBasis basis(2, 2, /*allow_double_occupancy=*/true);

  std::vector<Operator> term = {Operator::creation(Operator::Spin::UP, 0)};
  EXPECT_FALSE(basis.contains(term));
}

TEST(BasisTest, SortBasis) {
  FermionicBasis basis(2, 2, /*allow_double_occupancy=*/true);

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

TEST(PrepareUpAndDownRepresentationTest, EmptyElement) {
  Basis::BasisElement element;
  std::vector<int> up(5, 0);
  std::vector<int> down(5, 0);
  prepare_up_and_down_representation(element, up, down);
  EXPECT_EQ(up, std::vector<int>(5, 0));
  EXPECT_EQ(down, std::vector<int>(5, 0));
}

TEST(PrepareUpAndDownRepresentationTest, SingleAnnihilationUp) {
  Basis::BasisElement element{Operator::annihilation(Operator::Spin::UP, 2)};
  std::vector<int> up(5, 0);
  std::vector<int> down(5, 0);
  prepare_up_and_down_representation(element, up, down);
  EXPECT_EQ(up, std::vector<int>({0, 0, -1, 0, 0}));
  EXPECT_EQ(down, std::vector<int>(5, 0));
}

TEST(PrepareUpAndDownRepresentationTest, MultipleOperators) {
  Basis::BasisElement element{Operator::creation(Operator::Spin::DOWN, 1),
                              Operator::annihilation(Operator::Spin::UP, 3),
                              Operator::creation(Operator::Spin::UP, 0)};
  std::vector<int> up(5, 0);
  std::vector<int> down(5, 0);
  prepare_up_and_down_representation(element, up, down);
  EXPECT_EQ(up, std::vector<int>({1, 0, 0, -1, 0}));
  EXPECT_EQ(down, std::vector<int>({0, 1, 0, 0, 0}));
}

TEST(StateStringTest, EmptyElement) {
  Basis::BasisElement element;
  FermionicBasis basis(5, 0, /*allow_double_occupancy=*/true);
  std::string expected_state = "|  ,  ,  ,  ,  >";
  std::string actual_state = basis.state_string(element);
  EXPECT_EQ(actual_state, expected_state);
}

TEST(StateStringTest, SingleUp) {
  Basis::BasisElement element;
  element.push_back(Operator::creation(Operator::Spin::UP, 1));
  FermionicBasis basis(5, 1, /*allow_double_occupancy=*/true);
  std::string expected_state = "|  ,\u2191 ,  ,  ,  >";
  std::string actual_state = basis.state_string(element);
  EXPECT_EQ(actual_state, expected_state);
}

TEST(StateStringTest, MultipleOperators) {
  Basis::BasisElement element{Operator::creation(Operator::Spin::UP, 0),
                              Operator::creation(Operator::Spin::DOWN, 1),
                              Operator::creation(Operator::Spin::UP, 3)};
  FermionicBasis basis(5, 3, /*allow_double_occupancy=*/true);
  std::string expected_state = "|\u2191 , \u2193,  ,\u2191 ,  >";
  std::string actual_state = basis.state_string(element);
  EXPECT_EQ(actual_state, expected_state);
}

TEST(StateStringTest, AllUpAndDown) {
  Basis::BasisElement element{Operator::creation(Operator::Spin::UP, 0),
                              Operator::creation(Operator::Spin::DOWN, 0),
                              Operator::creation(Operator::Spin::UP, 1),
                              Operator::creation(Operator::Spin::DOWN, 1)};
  FermionicBasis basis(5, 4, /*allow_double_occupancy=*/true);
  std::string expected_state = "|\u2191\u2193,\u2191\u2193,  ,  ,  >";
  std::string actual_state = basis.state_string(element);
  EXPECT_EQ(actual_state, expected_state);
}
