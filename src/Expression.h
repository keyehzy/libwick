#pragma once

#include <unordered_map>
#include <vector>

#include "Term.h"

template <>
struct std::hash<std::vector<Operator>> {
  size_t operator()(const std::vector<Operator>& operators) const {
    size_t hash = 0;
    for (const auto& op : operators) {
      hash_combine(hash, std::hash<Operator>{}(op));
    }
    return hash;
  }

 private:
  template <typename T>
  void hash_combine(size_t& seed, const T& value) const {
    seed ^= std::hash<T>{}(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }
};

class Expression {
 public:
  Expression() = default;

  using ExpressionMap =
      std::unordered_map<std::vector<Operator>, double>;

  Expression(const ExpressionMap& terms) : m_terms(terms) {}

  Expression(const std::vector<Term>& terms) {
    for (const auto& term : terms) {
      m_terms[term.operators()] += term.coefficient();
    }
  }

  const ExpressionMap& terms() const { return m_terms; }

  bool operator==(const Expression& other) const {
    return m_terms == other.m_terms;
  }

  bool operator!=(const Expression& other) const { return !(*this == other); }

  void clean() {
    for (auto it = m_terms.begin(); it != m_terms.end();) {
      if (std::abs(it->second) < m_tolerance) {
        it = m_terms.erase(it);
      } else {
        ++it;
      }
    }
  }

 private:
  ExpressionMap m_terms;
  constexpr static double m_tolerance = 1e-10;
};