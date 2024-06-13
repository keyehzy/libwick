// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <algorithm>
#include <cstddef>
#include <unordered_map>
#include <vector>

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
    for (std::size_t i = 0; i < m_elements.size(); i++) {
      m_index_map[m_elements[i]] = i;
    }
  }
};
