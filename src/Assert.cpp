// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include "Assert.h"

void libmb_assert_impl(
    bool condition, const char* expr, const char* file, int line) {
  if (!condition) {
    std::fprintf(stderr, "Assertion failed: %s at %s:%d\n", expr, file, line);
    __builtin_trap();
  }
}

void libmb_unreachable_impl(const char* file, int line) {
  std::fprintf(stderr, "Unreachable code reached at %s:%d\n", file, line);
  __builtin_trap();
}
