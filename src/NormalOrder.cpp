#include "NormalOrder.h"

#include <vector>

// Sorts the term and count the number of swaps. Additionally, we need to
// store a stack of Terms that need to be sorted in the next iteration.

// 1) If two adjacent operators are of Creation type, then sort by identifier1 <
// identifier2 If two adjacent operators are of Annihilation type, then sort by
// identifier1 > identifier2.
// 2) If the first operator is of Annihilation type and the second operator is
// of Creation type, then swap.
// 3) If the first operator is of Creation type and the second operator is of
// Annihilation type, if the identifiers are the same, we need to push a new
// Term to the stack with coefficient 1.0 and with the two operators removed.
// In any case, we need to swap the operators.

std::pair<Term, int> sortTerm(Term term, std::vector<Term>& stack) {
  if (term.operators().size() < 2) {
    return {term, 0};
  }

  int swaps = 0;
  bool retry = true;

  auto do_swap = [&](size_t i, size_t j) {
    std::swap(term.operators().at(i), term.operators().at(j));
    swaps++;
    retry = true;
  };

  while (retry) {
    retry = false;
    for (size_t i = 0; i < term.operators().size() - 1; i++) {
      Operator op1 = term.operators().at(i);
      Operator op2 = term.operators().at(i + 1);
      if (op1.type() == OperatorType::CREATION &&
          op2.type() == OperatorType::CREATION) {
        if (op1.identifier() > op2.identifier()) {
          do_swap(i, i + 1);
        }
      } else if (op1.type() == OperatorType::ANNIHILATION &&
                 op2.type() == OperatorType::ANNIHILATION) {
        if (op1.identifier() < op2.identifier()) {
          do_swap(i, i + 1);
        }
      } else if (op1.type() == OperatorType::ANNIHILATION &&
                 op2.type() == OperatorType::CREATION) {
        if (op1.identifier() == op2.identifier()) {
          Term newTerm(
              swaps % 2 == 0 ? term.coefficient() : -term.coefficient(), {});
          for (size_t j = 0; j < term.operators().size(); j++) {
            if (j != i && j != i + 1) {
              newTerm.operators().push_back(term.operators().at(j));
            }
          }
          stack.push_back(newTerm);
        }
        do_swap(i, i + 1);
      }
    }
  }
  return {term, swaps};
}

Expression normalOrder(const Term& term) {
  std::vector<Term> stack = {term};
  Expression::ExpressionMap terms;
  while (!stack.empty()) {
    Term currentTerm = stack.back();
    stack.pop_back();
    auto [result, swaps] = sortTerm(currentTerm, stack);
    if (swaps % 2 == 0) {
      terms[result.operators()] += result.coefficient();
    } else {
      terms[result.operators()] -= result.coefficient();
    }
  }
  return Expression(terms);
}