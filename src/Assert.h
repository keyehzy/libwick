// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <cstdio>

#ifndef NDEBUG
#define LIBMB_ASSERT(condition)                                            \
  do {                                                                     \
    if (!(condition)) {                                                    \
      std::fprintf(                                                        \
          stderr, "Assertion failed: %s at %s:%d\n", #condition, __FILE__, \
          __LINE__);                                                       \
      __builtin_trap();                                                    \
    }                                                                      \
  } while (0)

#define LIBMB_UNREACHABLE()                                                 \
  do {                                                                      \
    std::fprintf(                                                           \
        stderr, "Unreachable code reached at %s:%d\n", __FILE__, __LINE__); \
    __builtin_trap();                                                       \
  } while (0)
#else
#define LIBMB_ASSERT(condition)
#define LIBMB_UNREACHABLE()
#endif
