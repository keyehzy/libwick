#pragma once

#include <unordered_map>

#include "Term.h"

class Basis {
 public:
  using BasisMap = std::unordered_map<std::vector<Operator>, std::size_t>;

  Basis(std::size_t n, std::size_t m) : m_orbitals{m}, m_particles{n} {
    generate_basis();
  }

  const std::vector<std::vector<Operator>>& elements() const { return m_basis; }

  std::size_t orbitals() const { return m_orbitals; }

  std::size_t particles() const { return m_particles; }

  bool operator==(const Basis& other) const {
    return m_orbitals == other.m_orbitals && m_particles == other.m_particles &&
           m_basis == other.m_basis;
  }

  bool operator!=(const Basis& other) const { return !(*this == other); }

  std::size_t index(const std::vector<Operator>& term) const {
    BasisMap::const_iterator iter = m_basis_map.find(term);
    if (iter != m_basis_map.end()) {
      return iter->second;
    }
    return m_basis_map.size();
  }

 private:
  void generate_combinations(std::vector<Operator> current,
                             size_t first_orbital, size_t depth,
                             size_t max_depth) {
    if (depth == max_depth) {
      m_basis_map[current] = m_basis.size();
      m_basis.push_back(current);
      return;
    }

    for (size_t i = first_orbital; i < m_orbitals; i++) {
      for (Spin spin : {Spin::UP, Spin::DOWN}) {
        if (current.empty() || current.back().orbital() < i ||
            (current.back().orbital() == i && spin > current.back().spin())) {
          current.emplace_back(OperatorType::CREATION, spin, i);
          generate_combinations(current, i, depth + 1, max_depth);
          current.pop_back();
        }
      }
    }
  }

  void generate_basis() { generate_combinations({}, 0, 0, m_particles); }

  std::size_t m_orbitals;
  std::size_t m_particles;
  BasisMap m_basis_map;
  std::vector<std::vector<Operator>> m_basis;
};
