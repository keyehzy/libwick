// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <new>
#include <utility>

#include "Assert.h"

template <typename T>
class NonnullOwnPtr {
 public:
  NonnullOwnPtr() = delete;

  explicit NonnullOwnPtr(T* new_ptr) : ptr(new_ptr) {
    LIBMB_ASSERT(new_ptr != nullptr);
  }

  NonnullOwnPtr(NonnullOwnPtr&& other) noexcept
      : ptr(std::exchange(other.ptr, nullptr)) {}

  NonnullOwnPtr& operator=(NonnullOwnPtr&& other) noexcept {
    if (this != &other) {
      ptr = std::exchange(other.ptr, nullptr);
    }
    return *this;
  }

  ~NonnullOwnPtr() { delete ptr; }

  NonnullOwnPtr(const NonnullOwnPtr&) = delete;
  NonnullOwnPtr& operator=(const NonnullOwnPtr&) = delete;

  T* get() const { return ptr; }

  T* operator->() const { return ptr; }

  T& operator*() const { return *ptr; }

  void reset(T* new_ptr) {
    LIBMB_ASSERT(new_ptr != nullptr);
    if (ptr != new_ptr) {
      delete ptr;
      ptr = new_ptr;
    }
  }

 private:
  T* ptr;
};

template <typename T, typename... Args>
NonnullOwnPtr<T> make(Args&&... args) {
  T* ptr = new T(std::forward<Args>(args)...);
  LIBMB_ASSERT(ptr);
  return NonnullOwnPtr<T>(ptr);
}

template <typename T>
NonnullOwnPtr<T> adopt_own(T* ptr) {
  LIBMB_ASSERT(ptr != nullptr);
  return NonnullOwnPtr<T>(ptr);
}
