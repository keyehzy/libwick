// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <benchmark/benchmark.h>

#include "NormalOrder.h"

using enum Operator::Statistics;
using enum Operator::Spin;

static constexpr std::size_t max_orbital = 32;

static void BM_NormalOrderTermEasy(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();

    std::vector<Operator> operators;
    const int size = state.range(0);
    operators.reserve(size);
    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::creation<Fermion>(Up, i % max_orbital));
    }
    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::annihilation<Fermion>(
          Up, (size / 2 - i + 1) % max_orbital));
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
      operators.push_back(Operator::annihilation<Fermion>(
          Up, (size / 2 - i + 1) % max_orbital));
    }
    operators.push_back(Operator::creation<Fermion>(Up, 0));
    Term term(1.0, operators);

    state.ResumeTiming();
    Expression e = normal_order(term);
    benchmark::DoNotOptimize(e);
  }
}

BENCHMARK(BM_NormalOrderTermHard)->Range(8, 8 << 10);

static void BM_NormalOrderTermHarderEasyVersion(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();

    std::vector<Operator> operators;
    const int size = state.range(0);
    operators.reserve(size);

    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::annihilation<Fermion>(Up, 1));
    }
    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::creation<Fermion>(Up, 0));
    }
    Term term(1.0, operators);

    state.ResumeTiming();
    Expression e = normal_order(term);
    benchmark::DoNotOptimize(e);
  }
}

BENCHMARK(BM_NormalOrderTermHarderEasyVersion)->Range(8, 8 << 10);

static void BM_NormalOrderTermHarder1(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();

    std::vector<Operator> operators;
    const int size = state.range(0);
    operators.reserve(size);

    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::annihilation<Fermion>(Up, 0));
    }
    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::creation<Fermion>(Up, 0));
    }
    Term term(1.0, operators);

    state.ResumeTiming();
    Expression e = normal_order(term);
    benchmark::DoNotOptimize(e);
  }
}

BENCHMARK(BM_NormalOrderTermHarder1)->RangeMultiplier(2)->Range(8, 32);

static void BM_NormalOrderTermHarder2(benchmark::State& state) {
  for (auto _ : state) {
    state.PauseTiming();

    std::vector<Operator> operators;
    const int size = state.range(0);
    operators.reserve(size);

    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::annihilation<Fermion>(
          Up, (size / 2 - i + 1) % max_orbital));
    }
    for (int i = 0; i < size / 2; i++) {
      operators.push_back(Operator::creation<Fermion>(Up, i % max_orbital));
    }
    Term term(1.0, operators);

    state.ResumeTiming();
    Expression e = normal_order(term);
    benchmark::DoNotOptimize(e);
  }
}

BENCHMARK(BM_NormalOrderTermHarder2)->RangeMultiplier(2)->Range(8, 64);
