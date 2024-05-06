#pragma once

#include <cstddef>
#include <unordered_map>

template <typename T>
class SparseMatrix {
 private:
  struct Index {
    std::size_t i;
    std::size_t j;

    bool operator==(const Index& other) const noexcept {
      return i == other.i && j == other.j;
    }
  };

  struct IndexHasher {
    static constexpr std::size_t prime = 0x9e3779b9;
    size_t operator()(const Index& index) const noexcept {
      std::size_t ih = std::hash<std::size_t>{}(index.i);
      std::size_t jh = std::hash<std::size_t>{}(index.j);
      return (prime + ih) * prime + jh;
    }
  };

 public:
  SparseMatrix() = default;

  T& operator()(std::size_t i, std::size_t j) noexcept { return m_data[{i, j}]; }

  const T& operator()(std::size_t i, std::size_t j) const noexcept {
    auto it = m_data.find({i, j});
    return it == m_data.end() ? T{} : it->second;
  }

  std::size_t size() const noexcept { return m_data.size(); }

  bool operator==(const SparseMatrix& other) const noexcept {
    return m_data == other.m_data;
  }

  bool operator!=(const SparseMatrix& other) const noexcept { return !(*this == other); }

  const std::unordered_map<Index, T, IndexHasher>& data() const noexcept {
    return m_data;
  }

 private:
  std::unordered_map<Index, T, IndexHasher> m_data;
};