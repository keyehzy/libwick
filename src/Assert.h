// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <cstdio>

void libmb_assert_impl(
    bool condition, const char* expr, const char* file, int line);
void libmb_unreachable_impl(const char* file, int line);

#ifndef NDEBUG
#define LIBMB_ASSERT(condition) \
  libmb_assert_impl(condition, #condition, __FILE__, __LINE__)
#define LIBMB_UNREACHABLE() libmb_unreachable_impl(__FILE__, __LINE__)
#else
#define LIBMB_ASSERT(condition)
#define LIBMB_UNREACHABLE()
#endif
