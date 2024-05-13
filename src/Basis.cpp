#include "Basis.h"

#include <cassert>
#include <sstream>

void prepare_up_and_down_representation(const Basis::BasisElement& element,
                                        std::vector<int>& up,
                                        std::vector<int>& down) {
  for (auto it = element.rbegin(); it != element.rend(); it++) {
    const Operator& o = *it;
    if (o.type() == Operator::Type::ANNIHILATION) {
      if (o.spin() == Operator::Spin::UP) {
        up[o.orbital()] += -1;
      } else {
        down[o.orbital()] += -1;
      }
    } else {
      if (o.spin() == Operator::Spin::UP) {
        up[o.orbital()] += 1;
      } else {
        down[o.orbital()] += 1;
      }
    }
  }
}

std::string Basis::state_string(const Basis::BasisElement& element) const {
  assert(contains(element));
  std::vector<int> up(m_orbitals, 0);
  std::vector<int> down(m_orbitals, 0);
  prepare_up_and_down_representation(element, up, down);
  std::stringstream out;
  out << "|";
  for (std::size_t i = 0; i < m_orbitals; i++) {
    if (up[i] == 1 && down[i] == 1) {
      out << "\u2191\u2193,";
    } else if (up[i] == 1 && down[i] == 0) {
      out << "\u2191 ,";
    } else if (up[i] == 0 && down[i] == 1) {
      out << " \u2193,";
    } else if (up[i] == 0 && down[i] == 0) {
      out << "  ,";
    } else {
      assert(false);
    }
  }
  out << ">";
  return out.str();
}