// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "Operator.h"

template <class T>
class IndexedVectorMap {
 public:
  using element_type = T;

 private:
  std::vector<T> m_elements;
  std::unordered_map<T, std::size_t> m_index_map;

 public:
  const std::vector<T>& elements() const { return m_elements; }

  const std::unordered_map<T, std::size_t>& index_map() const {
    return m_index_map;
  }

  void insert(const T& value) {
    // We don't check if the element is already in the map
    m_elements.push_back(value);
    m_index_map[value] = m_elements.size() - 1;
  }

  const T& operator[](std::size_t idx) const { return m_elements[idx]; }

  std::size_t index(const T& value) const { return m_index_map.at(value); }

  bool contains(const T& value) const {
    return m_index_map.find(value) != m_index_map.end();
  }

  std::size_t size() const { return m_elements.size(); }

  template <class CompareFunction>
  void sort(CompareFunction comp) {
    std::sort(m_elements.begin(), m_elements.end(), comp);
    for (size_t i = 0; i < m_elements.size(); i++) {
      m_index_map[m_elements[i]] = i;
    }
  }
};

class Basis {
 public:
  using BasisElement = std::vector<Operator>;
  using BasisMap = std::unordered_map<BasisElement, std::size_t>;
  using FilterFunction = std::function<bool(const BasisElement&)>;

  Basis(std::size_t n, std::size_t m, bool allow_double_occupancy)
      : m_orbitals{n},
        m_particles{m},
        m_allow_double_occupancy{allow_double_occupancy} {
    generate_basis();
  }

  Basis(std::size_t n, std::size_t m, bool allow_double_occupancy,
        FilterFunction filter)
      : m_orbitals{n},
        m_particles{m},
        m_allow_double_occupancy{allow_double_occupancy},
        m_filter{filter} {
    generate_basis();
  }

  const std::vector<BasisElement>& elements() const {
    return m_basis_map.elements();
  }

  const BasisElement& element(std::size_t i) const { return m_basis_map[i]; }

  std::size_t orbitals() const { return m_orbitals; }

  std::size_t particles() const { return m_particles; }

  bool operator==(const Basis& other) const {
    return m_orbitals == other.m_orbitals && m_particles == other.m_particles &&
           m_basis_map.elements() == other.m_basis_map.elements();
  }

  bool operator!=(const Basis& other) const { return !(*this == other); }

  bool contains(const BasisElement& term) const {
    return m_basis_map.contains(term);
  }

  std::size_t index(const BasisElement& term) const {
    return m_basis_map.index(term);
  }

  std::size_t size() const { return m_basis_map.size(); }

  template <typename CompareFunction>
  void sort(CompareFunction comp) {
    m_basis_map.sort(comp);
  }

  std::string state_string(const BasisElement& element) const;

 private:
  void generate_combinations(BasisElement&, size_t, size_t, size_t);

  void generate_basis();

  std::size_t m_orbitals;
  std::size_t m_particles;
  bool m_allow_double_occupancy = true;
  IndexedVectorMap<BasisElement> m_basis_map;
  FilterFunction m_filter;
};

void prepare_up_and_down_representation(const Basis::BasisElement& element,
                                        std::vector<int>& up,
                                        std::vector<int>& down);