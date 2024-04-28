#pragma once

// We encode a single creation/annihilation operator as a byte.
// 0b00000000
//          ^ 0 = creation operator, 1 = annihilation operator
//         ^  0 = spin up, 1 = spin down
//   ^^^^^^     = orbital index (0-63)

#include <cstdint>
#include <string>

constexpr std::uint8_t OPERATOR_MASK = 0x1;  // 0b00000001
constexpr std::uint8_t SPIN_MASK = 0x2;      // 0b00000010
constexpr std::uint8_t ORBITAL_MASK = 0xFC;  // 0b11111100

enum class OperatorType { CREATION = 0, ANNIHILATION = 1 };

enum class Spin { UP = 0, DOWN = 1 };

class Operator {
 public:
  Operator(OperatorType type, Spin spin, std::uint8_t orbital)
      : m_data{orbital << 2 | static_cast<std::uint8_t>(spin) << 1 |
               static_cast<std::uint8_t>(type)} {}

  Operator(const Operator& other) : m_data(other.m_data) {}

  Operator& operator=(const Operator& other) {
    if (this != &other) {
      m_data = other.m_data;
    }
    return *this;
  }

  OperatorType type() const {
    return static_cast<OperatorType>(m_data & OPERATOR_MASK);
  }

  Spin spin() const { return static_cast<Spin>((m_data & SPIN_MASK) >> 1); }

  std::uint8_t orbital() const { return (m_data & ORBITAL_MASK) >> 2; }

  std::uint8_t raw() const { return m_data; }

  bool operator==(const Operator& other) const {
    return m_data == other.m_data;
  }

  bool operator!=(const Operator& other) const {
    return m_data != other.m_data;
  }

  std::string toString() const {
    std::string typeStr =
        (type() == OperatorType::CREATION) ? "Creation" : "Annihilation";
    std::string spinStr = (spin() == Spin::UP) ? "Up" : "Down";
    return "Operator { Type: " + typeStr + ", Spin: " + spinStr +
           ", Orbital: " + std::to_string(orbital()) + " }";
  }

  friend std::ostream& operator<<(std::ostream& os, const Operator& op) {
    return os << op.toString();
  }

  void swap(Operator& other) { std::swap(m_data, other.m_data); }

 private:
  std::uint8_t m_data;
};
