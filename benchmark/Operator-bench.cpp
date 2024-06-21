// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <benchmark/benchmark.h>

#include "Operator.h"

using enum Operator::Type;
using enum Operator::Statistics;
using enum Operator::Spin;

static std::size_t get_orbital(Operator o) { return o.orbital(); }

static void BM_CreateOperator(benchmark::State& state) {
  for (auto _ : state) {
    for (int orbital = 0; orbital < state.range(0); orbital++) {
      for (Operator::Type type : {Creation, Annihilation}) {
        for (Operator::Statistics stats : {Boson, Fermion}) {
          for (Operator::Spin spin : {Up, Down}) {
            Operator o = Operator(type, stats, spin, orbital % 32);
            std::size_t p = get_orbital(o);
            benchmark::DoNotOptimize(p);
          }
        }
      }
    }
  }
}

BENCHMARK(BM_CreateOperator)->Range(8, 8 << 27);
