// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <benchmark/benchmark.h>

#include "NormalOrder.h"

using enum Operator::Statistics;
using enum Operator::Spin;

static void BM_NormalOrderTermEasy(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();

    std::vector<Operator> operators;
    const int size = state.range(0);
    operators.reserve(size);
    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::creation<Fermion>(Up, i));
    }
    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::annihilation<Fermion>(Up, size - i + 1));
    }
    Term term(1.0, operators);

    state.ResumeTiming();
    Expression e = normal_order(term);
    benchmark::DoNotOptimize(e);
  }
}

BENCHMARK(BM_NormalOrderTermEasy)->Range(8, 8 << 10);

static void BM_NormalOrderTermHard(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();

    std::vector<Operator> operators;
    const int size = state.range(0);
    operators.reserve(size);

    for (int i = 0; i < size; i++) {
      operators.push_back(Operator::annihilation<Fermion>(Up, size - i + 1));
    }
    operators.push_back(Operator::creation<Fermion>(Up, 0));
    Term term(1.0, operators);

    state.ResumeTiming();
    Expression e = normal_order(term);
    benchmark::DoNotOptimize(e);
  }
}

BENCHMARK(BM_NormalOrderTermHard)->Range(8, 8 << 10);

static void BM_NormalOrderTermHarder(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();

    std::vector<Operator> operators;
    const int size = state.range(0);
    operators.reserve(size);

    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::annihilation<Fermion>(Up, size - i + 1));
    }
    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::creation<Fermion>(Up, i));
    }
    Term term(1.0, operators);

    state.ResumeTiming();
    Expression e = normal_order(term);
    benchmark::DoNotOptimize(e);
  }
}

BENCHMARK(BM_NormalOrderTermHarder)->Range(8, 8 << 10);
