// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <benchmark/benchmark.h>

#include "BasisFilter.h"
#include "BosonicBasis.h"
#include "FermionicBasis.h"
#include "GenericBasis.h"

static auto basis_range = benchmark::CreateDenseRange(8, 12, 2);

static void BM_CreateGenericBasis(benchmark::State& state) {
  for (auto _ : state) {
    GenericBasis basis(
        /*orbitals*/ state.range(0), /*particles*/ state.range(1));
    benchmark::DoNotOptimize(basis);
  }
}

BENCHMARK(BM_CreateGenericBasis)->ArgsProduct({basis_range, basis_range});

static void BM_CreateFermionicBasis(benchmark::State& state) {
  for (auto _ : state) {
    FermionicBasis basis(
        /*orbitals*/ state.range(0), /*particles*/ state.range(1));
    benchmark::DoNotOptimize(basis);
  }
}

BENCHMARK(BM_CreateFermionicBasis)->ArgsProduct({basis_range, basis_range});

static void BM_CreateBosonicBasis(benchmark::State& state) {
  for (auto _ : state) {
    BosonicBasis basis(
        /*orbitals*/ state.range(0), /*particles*/ state.range(1));
    benchmark::DoNotOptimize(basis);
  }
}

BENCHMARK(BM_CreateBosonicBasis)->ArgsProduct({basis_range, basis_range});

class ZeroTotalSpinFilter : public BasisFilter {
 public:
  bool filter(const BasisElement& element) const override {
    int total_spin = 0;
    for (const auto& op : element) {
      total_spin += op.spin() == Operator::Spin::Up ? 1 : -1;
    }
    return total_spin == 0;
  }
};

static void BM_CreateGenericBasisWithFilter(benchmark::State& state) {
  for (auto _ : state) {
    GenericBasis basis(
        /*orbitals*/ state.range(0), /*particles*/ state.range(1),
        new ZeroTotalSpinFilter);
    benchmark::DoNotOptimize(basis);
  }
}

BENCHMARK(BM_CreateGenericBasisWithFilter)
    ->ArgsProduct({basis_range, basis_range});

static void BM_CreateFermionicBasisWithFilter(benchmark::State& state) {
  for (auto _ : state) {
    FermionicBasis basis(
        /*orbitals*/ state.range(0), /*particles*/ state.range(1),
        new ZeroTotalSpinFilter);
    benchmark::DoNotOptimize(basis);
  }
}

BENCHMARK(BM_CreateFermionicBasisWithFilter)
    ->ArgsProduct({basis_range, basis_range});

static void BM_CreateBosonicBasisWithFilter(benchmark::State& state) {
  for (auto _ : state) {
    BosonicBasis basis(
        /*orbitals*/ state.range(0), /*particles*/ state.range(1),
        new ZeroTotalSpinFilter);
    benchmark::DoNotOptimize(basis);
  }
}

BENCHMARK(BM_CreateBosonicBasisWithFilter)
    ->ArgsProduct({basis_range, basis_range});
