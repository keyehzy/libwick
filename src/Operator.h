#pragma once

#include <cstdint>

// We encode a single creation/annihilation operator as a byte.
// 0b00000000
//          ^ 0 = creation operator, 1 = annihilation operator
//         ^  0 = spin up, 1 = spin down
//   ^^^^^^     = orbital index (0-63)

constexpr std::uint8_t OPERATOR_MASK = 0x1; // 0b00000001
constexpr std::uint8_t SPIN_MASK = 0x2;     // 0b00000010
constexpr std::uint8_t ORBITAL_MASK = 0xFC; // 0b11111100

enum class OperatorType { CREATION = 0, ANNIHILATION = 1 };

enum class Spin { UP = 0, DOWN = 1 };

class Operator {
 public:
Operator(OperatorType type, Spin spin, std::uint8_t orbital)
    : data{orbital << 2 | static_cast<std::uint8_t>(spin) << 1 |
           static_cast<std::uint8_t>(type)} {}

  OperatorType type() const {
    return static_cast<OperatorType>(data & OPERATOR_MASK);
  }

  Spin spin() const { return static_cast<Spin>(data & SPIN_MASK); }

  std::uint8_t orbital() const { return data & ORBITAL_MASK; }

  std::uint8_t raw() const { return data; }

 private:
  std::uint8_t data;
};
