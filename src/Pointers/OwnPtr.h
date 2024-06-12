// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <new>
#include <utility>

template <typename T>
class OwnPtr {
 public:
  OwnPtr() = default;

  explicit OwnPtr(T* ptr) : ptr(ptr) {}

  OwnPtr(OwnPtr&& other) noexcept : ptr(std::exchange(other.ptr, nullptr)) {}

  OwnPtr& operator=(OwnPtr&& other) noexcept {
    if (this != &other) {
      reset(std::exchange(other.ptr, nullptr));
    }
    return *this;
  }

  ~OwnPtr() { delete ptr; }

  OwnPtr(const OwnPtr&) = delete;
  OwnPtr& operator=(const OwnPtr&) = delete;

  T* get() const { return ptr; }

  T* operator->() const { return ptr; }

  T& operator*() const { return *ptr; }

  void reset(T* new_ptr = nullptr) {
    if (ptr != new_ptr) {
      delete ptr;
      ptr = new_ptr;
    }
  }

 private:
  T* ptr = nullptr;
};

template <typename T>
OwnPtr<T> adopt(T* ptr) {
  return OwnPtr<T>(ptr);
}
