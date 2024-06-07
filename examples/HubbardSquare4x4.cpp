// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <armadillo>
#include <array>
#include <iomanip>

#include "Basis.h"
#include "FermionicBasis.h"
#include "Models/HubbardSquare.h"

// Table 2 of https://journals.aps.org/prb/pdf/10.1103/PhysRevB.45.10741
struct HubbardModel4x4Results {
  size_t num_electrons;
  std::array<double, 4> ground_state_energy;
};

constexpr static double hubbardU[] = {
    20,
    10,
    8,
    4,
};

constexpr static HubbardModel4x4Results results[] = {
    {2, {-7.67497, -7.74018, -7.76409, -7.83893}},
    {3, {-9.37517, -9.49370, -9.53829, -9.68124}},
    {4, {-11.08201, -11.25621, -11.32150, -11.53029}},
    {5, {-12.68834, -12.93226, -13.02648, -13.33283}},
    {6, {-14.26547, -14.59584, -14.72354, -15.13601}},
    {7, {-14.78807, -15.34537, -15.56910, -16.32054}},
    {8, {-15.35716, -16.14321, -16.46063, -17.53490}},
    {9, {-15.46814, -16.55123, -17.00190, -18.55363}},
    {10, {-15.45150, -16.90356, -17.51037, -19.58094}},
    {11, {-13.92019, -15.51372, -16.20437, -18.64833}},
    {12, {-12.45330, -14.16438, -14.92531, -17.72958}},
    {13, {-10.47859, -12.46313, -13.38126, -16.72700}},
    {14, {-8.46148, -10.80701, -11.86884, -15.74459}},
    {15, {-6.06801, -8.89301, -10.14724, -14.66524}},
    {16, {-3.73991, -7.02900, -8.46888, -13.62185}},
};

int example() {
  auto filter = [](const Basis::BasisElement& element) -> bool {
    int total_spin = 0;
    for (const auto& op : element) {
      total_spin += 1 - 2 * static_cast<int>(op.spin());
    }
    return total_spin == 0;
  };
  int rows_to_take = 4;
  int uidx_to_take = 4;
  std::cout << std::setw(10);
  std::cout << "Result:" << "    " << "Expected:" << std::endl;
  for (int row = 0; row < rows_to_take; row++) {
    for (int uidx = 0; uidx < uidx_to_take; uidx++) {
      HubbardSquare model(1.0, hubbardU[uidx], /*nx=*/4, /*ny=*/4);
      FermionicBasis basis(model.size(), results[row].num_electrons, filter);
      arma::sp_mat mat(basis.size(), basis.size());
      model.compute_matrix_elements(basis, mat);
      arma::vec eigval;
      arma::eigs_sym(eigval, mat, 1, "sa");
      std::cout << std::setw(10) << eigval[0] << "   "
                << results[row].ground_state_energy[uidx] << std::endl;
    }
  }

  return 0;
}
