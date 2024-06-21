// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Term.h"

std::ostream& operator<<(std::ostream& os, const Term& term) {
  os << "Term { Coefficient: " << term.coefficient();
  os << ", Operators: [";
  for (size_t i = 0; i < term.operators().size() - 1; ++i) {
    os << term.operators()[i].toString() << ", ";
  }
  os << term.operators()[term.operators().size() - 1].toString();
  os << "]}";
  return os;
}
