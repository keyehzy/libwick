// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

// We encode a single creation/annihilation operator as a byte.
// 0b00000000
//          ^ 0 = creation operator, 1 = annihilation operator
//         ^  0 = boson, 1 = fermion
//        ^   0 = spin up, 1 = spin down
//   ^^^^^      = orbital index (0-31)

#include <cstdint>
#include <functional>
#include <string>

constexpr std::uint8_t OPERATOR_MASK = 0x1;    // 0b00000001
constexpr std::uint8_t STATISTICS_MASK = 0x2;  // 0b00000010
constexpr std::uint8_t SPIN_MASK = 0x4;        // 0b00000100
constexpr std::uint8_t ORBITAL_MASK = 0xF8;    // 0b11111000

class Operator {
 public:
  enum class Type { CREATION = 0, ANNIHILATION = 1 };
  enum class Statistics { BOSON = 0, FERMION = 1 };
  enum class Spin { UP = 0, DOWN = 1 };

  Operator(Type type, Statistics stats, Spin spin, std::uint8_t orbital) {
    m_data = static_cast<std::uint8_t>(type) |
             (static_cast<std::uint8_t>(stats) << 1) |
             (static_cast<std::uint8_t>(spin) << 2) | (orbital << 3);
  }

  Operator(const Operator& other) : m_data(other.m_data) {}

  Operator& operator=(const Operator& other) {
    if (this != &other) {
      m_data = other.m_data;
    }
    return *this;
  }

  Type type() const { return static_cast<Type>(m_data & OPERATOR_MASK); }

  Statistics statistics() const {
    return static_cast<Statistics>((m_data & STATISTICS_MASK) >> 1);
  }

  Spin spin() const { return static_cast<Spin>((m_data & SPIN_MASK) >> 2); }

  std::uint8_t orbital() const { return m_data >> 3; }

  std::uint8_t identifier() const { return m_data >> 1; }

  std::uint8_t raw() const { return m_data; }

  bool operator==(const Operator& other) const {
    return m_data == other.m_data;
  }

  bool operator!=(const Operator& other) const {
    return m_data != other.m_data;
  }

  bool is_boson() const { return statistics() == Statistics::BOSON; }

  bool is_fermion() const { return statistics() == Statistics::FERMION; }

  std::string toString() const {
    std::string typeStr =
        (type() == Type::CREATION) ? "Creation" : "Annihilation";
    std::string spinStr = (spin() == Spin::UP) ? "Up" : "Down";
    std::string orbitalStr = std::to_string(orbital());
    return "Operator { Type: " + typeStr + ", Spin: " + spinStr +
           ", Orbital: " + orbitalStr + " }";
  }

  friend std::ostream& operator<<(std::ostream& os, const Operator& op) {
    return os << op.toString();
  }

  Operator adjoint() const {
    return Operator(
        type() == Type::CREATION ? Type::ANNIHILATION : Type::CREATION,
        statistics(), spin(), orbital());
  }

  static Operator creation(Spin spin, std::uint8_t orbital) {
    return Operator(Type::CREATION, Statistics::FERMION, spin, orbital);
  }

  static Operator annihilation(Spin spin, std::uint8_t orbital) {
    return Operator(Type::ANNIHILATION, Statistics::FERMION, spin, orbital);
  }

 private:
  std::uint8_t m_data;
};

template <>
struct std::hash<Operator> {
  size_t operator()(const Operator& op) const {
    return std::hash<std::uint8_t>()(op.raw());
  }
};

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
