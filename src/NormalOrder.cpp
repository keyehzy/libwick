#include "NormalOrder.h"

#include <vector>

// Sorts the term and count the number of swaps. Additionally, we need to
// store a stack of Terms that need to be sorted in the next iteration.

std::pair<Term, int> sortTerm(Term term, std::vector<Term>& stack) {
  if (term.operators().size() < 2) {
    return {term, 0};
  }

  int swaps = 0;

  auto swap_and_accumulate = [&](size_t i, size_t j) {
    std::swap(term.operators().at(i), term.operators().at(j));
    swaps++;
  };

  auto push_new_term = [&](size_t i, size_t j) {
    Term newTerm(swaps % 2 == 0 ? term.coefficient() : -term.coefficient(), {});
    for (size_t k = 0; k < term.operators().size(); k++) {
      if (k != i && k != j) {
        newTerm.operators().push_back(term.operators().at(k));
      }
    }
    stack.push_back(newTerm);
  };

  for (std::size_t i = 1; i < term.operators().size(); ++i) {
    for (std::size_t j = i; j > 0; --j) {
      Operator op1 = term.operators().at(j - 1);
      Operator op2 = term.operators().at(j);
      if (op1.type() == OperatorType::CREATION &&
          op2.type() == OperatorType::CREATION &&
          op1.identifier() > op2.identifier()) {
        swap_and_accumulate(j - 1, j);
      } else if (op1.type() == OperatorType::ANNIHILATION &&
                 op2.type() == OperatorType::ANNIHILATION &&
                 op1.identifier() < op2.identifier()) {
        swap_and_accumulate(j - 1, j);
      } else if (op1.type() == OperatorType::ANNIHILATION &&
                 op2.type() == OperatorType::CREATION) {
        if (op1.identifier() == op2.identifier()) {
          push_new_term(j - 1, j);
        }
        swap_and_accumulate(j - 1, j);
      }
    }
  }
  return {term, swaps};
}

Expression normal_order(const Term& term) {
  std::vector<Term> stack = {term};
  Expression::ExpressionMap terms;
  while (!stack.empty()) {
    Term currentTerm = stack.back();
    stack.pop_back();
    auto [result, swaps] = sortTerm(currentTerm, stack);
    terms[result.operators()] +=
        (swaps % 2 == 0 ? 1 : -1) * result.coefficient();
  }
  return Expression(terms);
}

Expression normal_order(const Expression& expression) {
  Expression::ExpressionMap terms;
  for (const auto& [term, coefficient] : expression.terms()) {
    Expression result = normal_order(Term(coefficient, term));
    for (const auto& [newTerm, newCoefficient] : result.terms()) {
      terms[newTerm] += coefficient * newCoefficient;
    }
  }
  return Expression(terms);
}

Expression commute(const Term& term1, const Term& term2) {
  Term t1 = term1.product(term2);
  Term t2 = term2.product(term1);
  t2.coefficient() *= -1;
  return Expression(std::vector<Term>{t1, t2});
}
