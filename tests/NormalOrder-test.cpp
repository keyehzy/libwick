// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "NormalOrder.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using testing::IsEmpty;

using enum Operator::Type;
using enum Operator::Statistics;
using enum Operator::Spin;

TEST(NormalOrderTest, NormalOrderTermEqual) {
  {
    Term term(
        1.0, {Operator::creation<Fermion>(Up, 0),
              Operator::annihilation<Fermion>(Down, 1)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        1.0, {Operator::creation<Fermion>(Up, 0),
              Operator::annihilation<Fermion>(Down, 1)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0, {Operator::creation<Boson>(Up, 0),
              Operator::annihilation<Boson>(Down, 1)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        1.0, {Operator::creation<Boson>(Up, 0),
              Operator::annihilation<Boson>(Down, 1)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(NormalOrderTest, NormalOrderTermCreationCreation) {
  {
    Term term(
        1.0, {Operator::creation<Fermion>(Up, 1),
              Operator::creation<Fermion>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        -1.0, {Operator::creation<Fermion>(Up, 0),
               Operator::creation<Fermion>(Up, 1)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0,
        {Operator::creation<Boson>(Up, 1), Operator::creation<Boson>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        1.0,
        {Operator::creation<Boson>(Up, 0), Operator::creation<Boson>(Up, 1)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(NormalOrderTest, NormalOrderTermAnnihilationAnnihilation) {
  {
    Term term(
        1.0, {Operator::annihilation<Fermion>(Down, 0),
              Operator::annihilation<Fermion>(Down, 1)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        -1.0, {Operator::annihilation<Fermion>(Down, 1),
               Operator::annihilation<Fermion>(Down, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0, {Operator::annihilation<Boson>(Down, 0),
              Operator::annihilation<Boson>(Down, 1)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        1.0, {Operator::annihilation<Boson>(Down, 1),
              Operator::annihilation<Boson>(Down, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(NormalOrderTest, NormalOrderTermCreationAnnihilation) {
  {
    Term term(
        1.0, {Operator::creation<Fermion>(Up, 0),
              Operator::annihilation<Fermion>(Down, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        1.0, {Operator::creation<Fermion>(Up, 0),
              Operator::annihilation<Fermion>(Down, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0, {Operator::creation<Boson>(Up, 0),
              Operator::annihilation<Boson>(Down, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        1.0, {Operator::creation<Boson>(Up, 0),
              Operator::annihilation<Boson>(Down, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(NormalOrderTest, NormalOrderTermAnnihilationCreationDifferentSpin) {
  {
    Term term(
        1.0, {Operator::annihilation<Fermion>(Down, 0),
              Operator::creation<Fermion>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        -1.0, {Operator::creation<Fermion>(Up, 0),
               Operator::annihilation<Fermion>(Down, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0, {Operator::annihilation<Boson>(Down, 0),
              Operator::creation<Boson>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        1.0, {Operator::creation<Boson>(Up, 0),
              Operator::annihilation<Boson>(Down, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(NormalOrderTest, NormalOrderTermAnnihilationCreationDifferentOrbital) {
  {
    Term term(
        1.0, {Operator::annihilation<Fermion>(Up, 1),
              Operator::creation<Fermion>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        -1.0, {Operator::creation<Fermion>(Up, 0),
               Operator::annihilation<Fermion>(Up, 1)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0, {Operator::annihilation<Boson>(Up, 1),
              Operator::creation<Boson>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        1.0, {Operator::creation<Boson>(Up, 0),
              Operator::annihilation<Boson>(Up, 1)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(NormalOrderTest, NormalOrderTermAnnihilationCreationSameSpinSameOrbital) {
  {
    Term term(
        1.0, {Operator::annihilation<Fermion>(Up, 0),
              Operator::creation<Fermion>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {
        Term(
            -1.0, {Operator::creation<Fermion>(Up, 0),
                   Operator::annihilation<Fermion>(Up, 0)}),
        Term(1.0, {})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0, {Operator::annihilation<Boson>(Up, 0),
              Operator::creation<Boson>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {
        Term(
            1.0, {Operator::creation<Boson>(Up, 0),
                  Operator::annihilation<Boson>(Up, 0)}),
        Term(1.0, {})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(
    NormalOrderTest,
    NormalOrderTermCreationCreationAnnihilationDifferentOrbital) {
  {
    Term term(
        1.0,
        {Operator::creation<Fermion>(Up, 1), Operator::creation<Fermion>(Up, 0),
         Operator::annihilation<Fermion>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        -1.0,
        {Operator::creation<Fermion>(Up, 0), Operator::creation<Fermion>(Up, 1),
         Operator::annihilation<Fermion>(Up, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0,
        {Operator::creation<Boson>(Up, 1), Operator::creation<Boson>(Up, 0),
         Operator::annihilation<Boson>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {Term(
        1.0,
        {Operator::creation<Boson>(Up, 0), Operator::creation<Boson>(Up, 1),
         Operator::annihilation<Boson>(Up, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(NormalOrderTest, NormalOrderTermCreationAnnihilationCreationSameOrbital) {
  {
    Term term(
        1.0, {Operator::creation<Fermion>(Up, 0),
              Operator::annihilation<Fermion>(Up, 0),
              Operator::creation<Fermion>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {
        Term(
            -1.0, {Operator::creation<Fermion>(Up, 0),
                   Operator::creation<Fermion>(Up, 0),
                   Operator::annihilation<Fermion>(Up, 0)}),
        Term(1.0, {Operator::creation<Fermion>(Up, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0,
        {Operator::creation<Boson>(Up, 0), Operator::annihilation<Boson>(Up, 0),
         Operator::creation<Boson>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {
        Term(
            1.0,
            {Operator::creation<Boson>(Up, 0), Operator::creation<Boson>(Up, 0),
             Operator::annihilation<Boson>(Up, 0)}),
        Term(1.0, {Operator::creation<Boson>(Up, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(
    NormalOrderTest,
    NormalOrderTermAnnihilationCreationAnnihilationSameOrbital) {
  {
    Term term(
        1.0, {Operator::annihilation<Fermion>(Up, 0),
              Operator::creation<Fermion>(Up, 0),
              Operator::annihilation<Fermion>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {
        Term(
            -1.0, {Operator::creation<Fermion>(Up, 0),
                   Operator::annihilation<Fermion>(Up, 0),
                   Operator::annihilation<Fermion>(Up, 0)}),
        Term(1.0, {Operator::annihilation<Fermion>(Up, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0,
        {Operator::annihilation<Boson>(Up, 0), Operator::creation<Boson>(Up, 0),
         Operator::annihilation<Boson>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {
        Term(
            1.0, {Operator::creation<Boson>(Up, 0),
                  Operator::annihilation<Boson>(Up, 0),
                  Operator::annihilation<Boson>(Up, 0)}),
        Term(1.0, {Operator::annihilation<Boson>(Up, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(
    NormalOrderTest,
    NormalOrderTermAnnihilationAnnihilationCreationSameOrbital) {
  {
    Term term(
        1.0, {Operator::annihilation<Fermion>(Up, 0),
              Operator::annihilation<Fermion>(Up, 0),
              Operator::creation<Fermion>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {
        Term(
            1.0, {Operator::creation<Fermion>(Up, 0),
                  Operator::annihilation<Fermion>(Up, 0),
                  Operator::annihilation<Fermion>(Up, 0)}),
        Term(0.0, {Operator::annihilation<Fermion>(Up, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    Term term(
        1.0, {Operator::annihilation<Boson>(Up, 0),
              Operator::annihilation<Boson>(Up, 0),
              Operator::creation<Boson>(Up, 0)});
    Expression normal_ordered = normal_order(term);

    std::vector<Term> terms = {
        Term(
            1.0, {Operator::creation<Boson>(Up, 0),
                  Operator::annihilation<Boson>(Up, 0),
                  Operator::annihilation<Boson>(Up, 0)}),
        Term(2.0, {Operator::annihilation<Boson>(Up, 0)})};
    Expression expected(terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(NormalOrderTest, NormalOrderExpression) {
  {
    std::vector<Term> terms = {
        Term(
            1.0, {Operator::creation<Fermion>(Up, 0),
                  Operator::annihilation<Fermion>(Up, 1)}),
        Term(
            1.0, {Operator::creation<Fermion>(Up, 1),
                  Operator::annihilation<Fermion>(Up, 0)})};
    Expression normal_ordered = normal_order(terms);

    std::vector<Term> normal_terms = {
        Term(
            1.0, {Operator::creation<Fermion>(Up, 0),
                  Operator::annihilation<Fermion>(Up, 1)}),
        Term(
            1.0, {Operator::creation<Fermion>(Up, 1),
                  Operator::annihilation<Fermion>(Up, 0)})};
    Expression expected(normal_terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    std::vector<Term> terms = {
        Term(
            1.0, {Operator::creation<Boson>(Up, 0),
                  Operator::annihilation<Boson>(Up, 1)}),
        Term(
            1.0, {Operator::creation<Boson>(Up, 1),
                  Operator::annihilation<Boson>(Up, 0)})};
    Expression normal_ordered = normal_order(terms);

    std::vector<Term> normal_terms = {
        Term(
            1.0, {Operator::creation<Boson>(Up, 0),
                  Operator::annihilation<Boson>(Up, 1)}),
        Term(
            1.0, {Operator::creation<Boson>(Up, 1),
                  Operator::annihilation<Boson>(Up, 0)})};
    Expression expected(normal_terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(NormalOrderTest, NormalOrderExpressionWrongOrder) {
  {
    std::vector<Term> terms = {
        Term(
            1.0, {Operator::creation<Fermion>(Up, 1),
                  Operator::creation<Fermion>(Up, 0)}),
        Term(
            1.0, {Operator::annihilation<Fermion>(Up, 0),
                  Operator::annihilation<Fermion>(Up, 1)})};
    Expression normal_ordered = normal_order(terms);

    std::vector<Term> normal_terms = {
        Term(
            -1.0, {Operator::creation<Fermion>(Up, 0),
                   Operator::creation<Fermion>(Up, 1)}),
        Term(
            -1.0, {Operator::annihilation<Fermion>(Up, 1),
                   Operator::annihilation<Fermion>(Up, 0)})};
    Expression expected(normal_terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    std::vector<Term> terms = {
        Term(
            1.0, {Operator::creation<Boson>(Up, 1),
                  Operator::creation<Boson>(Up, 0)}),
        Term(
            1.0, {Operator::annihilation<Boson>(Up, 0),
                  Operator::annihilation<Boson>(Up, 1)})};
    Expression normal_ordered = normal_order(terms);

    std::vector<Term> normal_terms = {
        Term(
            1.0, {Operator::creation<Boson>(Up, 0),
                  Operator::creation<Boson>(Up, 1)}),
        Term(
            1.0, {Operator::annihilation<Boson>(Up, 1),
                  Operator::annihilation<Boson>(Up, 0)})};
    Expression expected(normal_terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(NormalOrderTest, NormalOrderExpressionResultingInZero) {
  {
    std::vector<Term> terms = {
        Term(
            1.0, {Operator::creation<Fermion>(Up, 1),
                  Operator::creation<Fermion>(Up, 0)}),
        Term(
            1.0, {Operator::creation<Fermion>(Up, 0),
                  Operator::creation<Fermion>(Up, 1)})};
    Expression normal_ordered = normal_order(terms);

    std::vector<Term> normal_terms = {Term(
        0.0, {Operator::creation<Fermion>(Up, 0),
              Operator::creation<Fermion>(Up, 1)})};
    Expression expected(normal_terms);

    EXPECT_EQ(normal_ordered, expected);
  }

  {
    std::vector<Term> terms = {
        Term(
            1.0, {Operator::creation<Boson>(Up, 1),
                  Operator::creation<Boson>(Up, 0)}),
        Term(
            1.0, {Operator::creation<Boson>(Up, 0),
                  Operator::creation<Boson>(Up, 1)})};
    Expression normal_ordered = normal_order(terms);

    std::vector<Term> normal_terms = {Term(
        2.0,
        {Operator::creation<Boson>(Up, 0), Operator::creation<Boson>(Up, 1)})};
    Expression expected(normal_terms);

    EXPECT_EQ(normal_ordered, expected);
  }
}

TEST(
    NormalOrderTest,
    NormalOrderTermAnnihilationAnnihilationCreationSameOrbitalAfterClean) {
  Term term(
      1.0, {Operator::annihilation<Fermion>(Up, 0),
            Operator::annihilation<Fermion>(Up, 0),
            Operator::creation<Fermion>(Up, 0)});
  Expression normal_ordered = normal_order(term);
  std::erase_if(normal_ordered.terms(), [](const auto &term_to_erase) {
    return std::abs(term_to_erase.second) < 1e-10;
  });

  std::vector<Term> terms = {Term(
      1.0, {Operator::creation<Fermion>(Up, 0),
            Operator::annihilation<Fermion>(Up, 0),
            Operator::annihilation<Fermion>(Up, 0)})};
  Expression expected(terms);

  EXPECT_EQ(normal_ordered, expected);
}

TEST(NormalOrderTest, NormalOrderExpressionResultingInZeroAfterClean) {
  std::vector<Term> terms = {
      Term(
          1.0, {Operator::creation<Fermion>(Up, 1),
                Operator::creation<Fermion>(Up, 0)}),
      Term(
          1.0, {Operator::creation<Fermion>(Up, 0),
                Operator::creation<Fermion>(Up, 1)})};
  Expression normal_ordered = normal_order(terms);
  std::erase_if(normal_ordered.terms(), [](const auto &term) {
    return std::abs(term.second) < 1e-10;
  });
  EXPECT_THAT(normal_ordered.terms(), IsEmpty());
}

TEST(NormalOrderTest, NormalOrderCommuteSameResultingInZero) {
  Term term1 = Term(1.0, {Operator::creation<Fermion>(Up, 0)});
  Expression e = commute(term1, term1);
  std::erase_if(e.terms(), [](const auto &term) {
    return std::abs(term.second) < 1e-10;
  });
  EXPECT_THAT(e.terms(), IsEmpty());
}

// The expectation value of this commutation should give just 0 and
// reproduce the commutation relation of the fermionic operators.
// However, since we are doing this calculation at the operator level
// we will have an additional term.
// In practice, when doing calcultion we define a basis where only the
// physical states are considered, so this term will not appear. Another
// solution is just to filter out the terms that, after normal ordering,
// have trailing annihilation operators.
TEST(NormalOrderTest, NormalOrderAntiCommuteSameResultingInZero) {
  {
    Term term1 = Term(1.0, {Operator::creation<Fermion>(Up, 0)});
    Expression e = anticommute(term1, term1);
    std::vector<Term> terms = {Term(
        2.0, {Operator::creation<Fermion>(Up, 0),
              Operator::creation<Fermion>(Up, 0)})};
    Expression expected(terms);
    EXPECT_EQ(e, expected);
  }

  {
    Term term1 = Term(1.0, {Operator::creation<Fermion>(Up, 0)});
    Term term2 = Term(1.0, {Operator::creation<Fermion>(Up, 1)});
    Expression e = anticommute(term1, term2);
    std::vector<Term> terms = {Term(
        2.0, {Operator::creation<Fermion>(Up, 0),
              Operator::creation<Fermion>(Up, 1)})};
    Expression expected(terms);
  }
}

TEST(NormalOrderTest, NormalOrderAntiCommuteDifferentResultingInNonZero) {
  Term term1 = Term(1.0, {Operator::creation<Fermion>(Up, 0)});
  Term term2 = Term(1.0, {Operator::annihilation<Fermion>(Up, 0)});
  Expression e = anticommute(term1, term2);
  std::erase_if(e.terms(), [](const auto &term) {
    return std::abs(term.second) < 1e-10;
  });
  std::vector<Term> terms = {Term(1.0, {})};
  Expression expected(terms);
  EXPECT_EQ(e, expected);
}
