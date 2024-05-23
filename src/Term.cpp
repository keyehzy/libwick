// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Term.h"

Term Term::Factory::one_body(double coefficient, Operator::Spin spin1,
                             std::uint8_t orbital1, Operator::Spin spin2,
                             std::uint8_t orbital2) {
  return Term(coefficient, {Operator::creation(spin1, orbital1),
                            Operator::annihilation(spin2, orbital2)});
}

Term Term::Factory::two_body(double coefficient, Operator::Spin spin1,
                             std::uint8_t orbital1, Operator::Spin spin2,
                             std::uint8_t orbital2, Operator::Spin spin3,
                             std::uint8_t orbital3, Operator::Spin spin4,
                             std::uint8_t orbital4) {
  return Term(coefficient, {Operator::creation(spin1, orbital1),
                            Operator::annihilation(spin2, orbital2),
                            Operator::creation(spin3, orbital3),
                            Operator::annihilation(spin4, orbital4)});
}

Term Term::Factory::density_density(double coefficient, Operator::Spin spin1,
                                    std::uint8_t orbital1, Operator::Spin spin2,
                                    std::uint8_t orbital2) {
  return Term(coefficient, {Operator::creation(spin1, orbital1),
                            Operator::annihilation(spin1, orbital1),
                            Operator::creation(spin2, orbital2),
                            Operator::annihilation(spin2, orbital2)});
}