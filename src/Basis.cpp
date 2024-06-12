// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Basis.h"

#include <sstream>

#include "Assert.h"

void prepare_up_and_down_representation(
    const BasisElement& element, std::vector<int>& up, std::vector<int>& down) {
  for (const auto& o : element) {
    (o.type() == Operator::Type::Annihilation
         ? (o.spin() == Operator::Spin::Up ? up[o.orbital()]--
                                           : down[o.orbital()]--)
         : (o.spin() == Operator::Spin::Up ? up[o.orbital()]++
                                           : down[o.orbital()]++));
  }
}

void Basis::generate_basis() {
  BasisElement current;
  current.reserve(m_particles);
  generate_combinations(current, 0, 0, m_particles);
}

static constexpr std::string_view unicode_empty_cell = "  ";
static constexpr std::string_view unicode_up_arrow_cell = "\u2191 ";
static constexpr std::string_view unicode_down_arrow_cell = " \u2193";
static constexpr std::string_view unicode_double_occ_cell = "\u2191\u2193";

std::string Basis::state_string(const BasisElement& element) const {
  LIBMB_ASSERT(contains(element));
  std::vector<int> up(m_orbitals, 0);
  std::vector<int> down(m_orbitals, 0);
  prepare_up_and_down_representation(element, up, down);

  std::stringstream out;
  out << "|";

  for (std::size_t i = 0; i < m_orbitals; i++) {
    switch (up[i] * 2 + down[i]) {
      case 0:
        out << unicode_empty_cell;
        break;
      case 1:
        out << unicode_down_arrow_cell;
        break;
      case 2:
        out << unicode_up_arrow_cell;
        break;
      case 3:
        out << unicode_double_occ_cell;
        break;
      default:
        LIBMB_UNREACHABLE();
        break;
    }
    if (i < m_orbitals - 1) {
      out << ",";
    }
  }
  out << ">";
  return out.str();
}
