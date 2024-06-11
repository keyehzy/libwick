// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Expression.h"

Expression add(const Term& a, const Term& b) {
  Expression result;
  result.insert(a);
  result.insert(b);
  return result;
}
