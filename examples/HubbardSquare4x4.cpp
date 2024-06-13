// Copyright (c) 2024 Matheus Sousa
// SPDX-License-Identifier: BSD-2-Clause

#include <armadillo>
#include <array>
#include <iomanip>

#include "Assert.h"
#include "Basis.h"
#include "FermionicBasis.h"
#include "Models/HubbardSquare.h"

// Table 2 of https://journals.aps.org/prb/pdf/10.1103/PhysRevB.45.10741
constexpr static std::array<double, 4> hubbardModelU = {20, 10, 8, 4};

constexpr static std::array<std::array<double, 4>, 15> hubbardModelTable = {
    {{-7.67497, -7.74018, -7.76409, -7.83893},      // 2 electrons
     {-9.37517, -9.49370, -9.53829, -9.68124},      // 3 electrons
     {-11.08201, -11.25621, -11.32150, -11.53029},  // 4 electrons
     {-12.68834, -12.93226, -13.02648, -13.33283},  // 5 electrons
     {-14.26547, -14.59584, -14.72354, -15.13601},  // 6 electrons
     {-14.78807, -15.34537, -15.56910, -16.32054},  // 7 electrons
     {-15.35716, -16.14321, -16.46063, -17.53490},  // 8 electrons
     {-15.46814, -16.55123, -17.00190, -18.55363},  // 9 electrons
     {-15.45150, -16.90356, -17.51037, -19.58094},  // 10 electrons
     {-13.92019, -15.51372, -16.20437, -18.64833},  // 11 electrons
     {-12.45330, -14.16438, -14.92531, -17.72958},  // 12 electrons
     {-10.47859, -12.46313, -13.38126, -16.72700},  // 13 electrons
     {-8.46148, -10.80701, -11.86884, -15.74459},   // 14 electrons
     {-6.06801, -8.89301, -10.14724, -14.66524},    // 15 electrons
     {-3.73991, -7.02900, -8.46888, -13.62185}}     // 16 electrons
};

// Construct a basis with total spin equal to zero
class ZeroTotalSpinFilter : public BasisFilter {
 public:
  bool filter(const BasisElement& element) const override {
    int totalSpin = 0;
    for (const auto& op : element) {
      totalSpin += 1 - 2 * static_cast<int>(op.spin());
    }
    return totalSpin == 0;
  }
};

int main() {
  std::size_t rowsToTake = 4;
  std::cout << "Result:   Expected:" << std::endl;

  for (std::size_t row = 0; row < rowsToTake; row++) {
    for (std::size_t uidx = 0; uidx < 4; uidx++) {
      const double t = 1.0;
      const double u = hubbardModelU[uidx];
      const std::size_t nx = 4;
      const std::size_t ny = 4;

      HubbardSquare model(t, u, nx, ny);
      FermionicBasis basis(model.size(), row + 2, new ZeroTotalSpinFilter);

      arma::SpMat<arma::cx_double> mat(basis.size(), basis.size());
      model.compute_matrix_elements(basis, mat);
      LIBMB_ASSERT(mat.is_hermitian());

      arma::cx_vec eigval;
      arma::eigs_gen(eigval, mat, 1, "sr");
      std::cout << std::setw(10) << eigval[0].real() << "   "
                << hubbardModelTable[row][uidx] << std::endl;
    }
  }

  return 0;
}
