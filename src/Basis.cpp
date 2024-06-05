// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Basis.h"

#include <cassert>
#include <sstream>

void prepare_up_and_down_representation(const Basis::BasisElement& element,
                                        std::vector<int>& up,
                                        std::vector<int>& down) {
  for (const auto& o : element) {
    (o.type() == Operator::Type::ANNIHILATION
         ? (o.spin() == Operator::Spin::UP ? up[o.orbital()]--
                                           : down[o.orbital()]--)
         : (o.spin() == Operator::Spin::UP ? up[o.orbital()]++
                                           : down[o.orbital()]++));
  }
}

void Basis::generate_basis() {
  BasisElement current;
  current.reserve(m_particles);
  generate_combinations(current, 0, 0, m_particles);
}

static constexpr std::string unicode_empty_cell = "  ";
static constexpr std::string unicode_up_arrow_cell = "\u2191 ";
static constexpr std::string unicode_down_arrow_cell = " \u2193";
static constexpr std::string unicode_double_occ_cell = "\u2191\u2193";

std::string Basis::state_string(const Basis::BasisElement& element) const {
  assert(contains(element));
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
        assert(false);
        break;
    }
    if (i < m_orbitals - 1) {
      out << ",";
    }
  }
  out << ">";
  return out.str();
}

void FermionicBasis::generate_combinations(BasisElement& current,
                                           size_t first_orbital, size_t depth,
                                           size_t max_depth) {
  if (depth == max_depth && (!m_filter || m_filter(current))) {
    m_basis_map.insert(current);
    return;
  }

  for (size_t i = first_orbital; i < m_orbitals; i++) {
    for (int spin_index = 0; spin_index < 2; ++spin_index) {
      Operator::Spin spin = static_cast<Operator::Spin>(spin_index);
      if (current.empty() || current.back().orbital() < i ||
          (m_allow_double_occupancy &&
           (current.back().orbital() == i && spin > current.back().spin()))) {
        current.push_back(Operator::creation(spin, i));
        generate_combinations(current, i, depth + 1, max_depth);
        current.pop_back();
      }
    }
  }
}
