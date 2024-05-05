#include "Basis.h"
#include "NormalOrder.h"
#include "Term.h"

class Model {
 public:
  template <typename SpMat>
  void compute_matrix_elements(Basis& basis, SpMat& mat) {
    std::vector<Term> hamilt = hamiltonian();
    for (const auto& basis_element : basis.elements()) {
      int basis_index = basis.index(basis_element);
      std::vector<Term> terms;
      for (const Term& hamilt_term : hamilt) {
        terms.push_back(hamilt_term.product(basis_element));
      }
      Expression e = normal_order(terms);
      for (const auto& [term, coeff] : e.terms()) {
        if (basis.contains(term)) {
          int term_index = basis.index(term);
          mat(basis_index, term_index) = coeff;
        }
      }
    }
  }

 private:
  virtual std::vector<Term> hamiltonian() const = 0;
};