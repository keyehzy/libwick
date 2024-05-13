// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <armadillo>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numbers>

#include "Model.h"
#include "SparseMatrix.h"

class Clock {
 private:
  using ClockType = std::chrono::steady_clock;

 public:
  Clock() : m_start(ClockType::now()) {}

  double elapsed() {
    auto end = ClockType::now();
    return std::chrono::duration<double>(end - m_start).count();
  }

  void reset() { m_start = ClockType::now(); }

 private:
  std::chrono::time_point<ClockType> m_start;
};

class HubbardChainKSpace : public Model {
 public:
  HubbardChainKSpace(double t, double u, std::size_t n, std::size_t m)
      : m_t(t), m_u(u), m_size(n), m_particles(m) {}

 private:
  void hopping_term(std::vector<Term>& result) const {
    auto index = [&](size_t i) {
      // The index will make the sequence
      // 0, 1, 2, ..., m_particles
      // look like
      // 0, 1, ..., m_particles / 2, -m_particles / 2, -m_particles / 2 - 1,
      // ...,  -1
      int lower = -m_particles / 2;
      int upper = m_particles / 2;
      return (i - lower) % (upper - lower + 1) + lower;
    };
    for (size_t i = 0; i < m_size; i++) {
      for (Operator::Spin spin : {Operator::Spin::UP, Operator::Spin::DOWN}) {
        double k = 2.0 * std::numbers::pi_v<double> *
                   static_cast<double>(index(i)) / static_cast<double>(m_size);
        double a = m_size == 2 ? 1 : 2;
        result.push_back(
            Term::one_body(-a * m_t * std::cos(k), spin, i, spin, i));
      }
    }
  }

  void interaction_term(std::vector<Term>& result) const {
    for (std::size_t k1 = 0; k1 < m_size; k1++) {
      for (std::size_t k2 = 0; k2 < m_size; k2++) {
        for (std::size_t k3 = 0; k3 < m_size; k3++) {
          for (std::size_t k4 = 0; k4 < m_size; k4++) {
            if (((k2 + k4) % m_size == (k1 + k3) % m_size)) {
              result.push_back(
                  Term::term(m_u / static_cast<double>(m_size),
                             Operator::creation(Operator::Spin::UP, k1),
                             Operator::annihilation(Operator::Spin::UP, k2),
                             Operator::creation(Operator::Spin::DOWN, k3),
                             Operator::annihilation(Operator::Spin::DOWN, k4)));
            }
          }
        }
      }
    }
  }

  std::vector<Term> hamiltonian() const override {
    std::vector<Term> result;
    hopping_term(result);
    interaction_term(result);
    return result;
  }

  double m_t;
  double m_u;
  size_t m_size;
  size_t m_particles;
};

void ground_state_analysis(const Basis& basis, const arma::vec& eigvec) {
  double double_occ_weight = 0.0;
  double single_occ_weight = 0.0;

  for (std::size_t i = 0; i < basis.size(); i++) {
    const Basis::BasisElement& element = basis.element(i);
    std::vector<int> up(basis.orbitals(), 0);
    std::vector<int> down(basis.orbitals(), 0);
    prepare_up_and_down_representation(element, up, down);
    std::size_t n_double_occ = 0;
    std::size_t n_single_occ = 0;
    for (std::size_t j = 0; j < basis.orbitals(); j++) {
      n_double_occ += up[j] == 1 && down[j] == 1;
      n_single_occ +=
          (up[j] == 1 && down[j] == 0) || (up[j] == 0 && down[j] == 1);
    }

    if (n_double_occ == basis.particles() / 2) {
      double_occ_weight += eigvec(i) * eigvec(i);
    } else if (n_single_occ == basis.particles()) {
      single_occ_weight += eigvec(i) * eigvec(i);
    }

    std::cout << std::setw(15) << eigvec(i) * eigvec(i) << "    "
              << basis.state_string(element) << " single_occ: " << n_single_occ
              << " double_occ: " << n_double_occ << std::endl;
  }

  std::cout << "Double occupancy weight: " << double_occ_weight << std::endl;
  std::cout << "Single occupancy weight: " << single_occ_weight << std::endl;
  std::cout << "Ratio between single and double: "
            << single_occ_weight / double_occ_weight << std::endl;
  std::cout << "Ratio between single and total: "
            << single_occ_weight / (1.0 - double_occ_weight) << std::endl;
}

int main() {
  Clock clock;
  const std::size_t size = 8;
  const std::size_t particles = 8;
  const std::size_t sz = 0;

  clock.reset();
  HubbardChainKSpace model(/*t=*/1.0, /*u=*/1e6, size, particles);
  std::cerr << "Model initialization: " << clock.elapsed() << " s" << std::endl;

  clock.reset();
  Basis basis(size, particles, [](const Basis::BasisElement& element) {
    int total_spin = 0;
    for (const auto& op : element) {
      total_spin += op.spin() == Operator::Spin::UP ? 1 : -1;
    }
    return total_spin == sz;
  });
  std::cerr << "Basis initialization: " << clock.elapsed() << " s" << std::endl;
  std::cerr << "Basis size: " << basis.size() << std::endl;

  if (basis.size() == 0) {
    std::cerr << "Basis is empty" << std::endl;
    return 1;
  }

  clock.reset();
  arma::sp_mat m(basis.size(), basis.size());
  model.compute_matrix_elements(basis, m);
  // arma::mat full_matrix(m);
  std::cerr << "Matrix initialization: " << clock.elapsed() << " s"
            << std::endl;

  if (!m.is_symmetric()) {
    std::cerr << "Matrix is not symmetric" << std::endl;
    return 1;
  }

  // std::cout << arma::mat(m) << std::endl;

#if 1

  clock.reset();
  const std::size_t eigval_count = 1000;
  arma::vec eigval;
  arma::mat eigvec;
  bool ok = arma::eigs_sym(eigval, eigvec, m, eigval_count, "sa");
  // bool ok = arma::eig_sym(eigval, eigvec, full_matrix);
  std::cerr << "Diagonalization: " << clock.elapsed() << " s" << std::endl;

  if (!ok) {
    std::cerr << "Diagonalization failed" << std::endl;
    return 1;
  }

  std::cout << "Eigenvalues:" << std::endl;
  for (std::size_t i = 0; i < eigval.size(); i++) {
    std::cout << std::setprecision(10) << eigval(i) << std::endl;
  }

  for (std::size_t i = 0; i < eigval.size(); i++) {
    std::cout << "Ground state " << i << ":" << std::endl;
    ground_state_analysis(basis, eigvec.col(i));
  }
#endif
}
