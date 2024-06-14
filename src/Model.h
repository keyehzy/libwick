// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include "Basis.h"
#include "NormalOrder.h"

class Model {
 public:
  virtual ~Model() = default;

  Model(const Model& other) = delete;
  Model& operator=(const Model& other) = delete;
  Model(Model&& other) = delete;
  Model& operator=(Model&& other) = delete;

  template <typename SpMat>
  void compute_matrix_elements(const Basis& basis, SpMat& mat) const {
    const std::vector<Term>& hamilt = hamiltonian();
#pragma omp parallel for schedule(dynamic)
    for (const auto& basis_element : basis.elements()) {
      std::size_t basis_index = basis.index(basis_element);
      std::vector<Term> terms;
      terms.reserve(hamilt.size());
      for (const Term& hamilt_term : hamilt) {
        terms.push_back(hamilt_term.product(basis_element));
      }
      const Expression& e = NormalOrderer(terms).expression();
      for (const auto& [term, coeff] : e.terms()) {
        if (term.back().type() == Operator::Type::Creation &&
            basis.contains(term)) {
          std::size_t term_index = basis.index(term);
#pragma omp critical
          mat(basis_index, term_index) = coeff;
        }
      }
    }
  }

 protected:
  Model() = default;

 private:
  virtual std::vector<Term> hamiltonian() const = 0;
};
