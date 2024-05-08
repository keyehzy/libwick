#include "NormalOrder.h"

#include <vector>

// TODO(m): change statistics for bosonic operators

std::size_t sort_term(Term& term, std::vector<Term>& stack) {
  std::size_t swaps = 0;
  std::vector<Operator> elms;

  auto swap_and_accumulate = [&](size_t i, size_t j) {
    std::swap(term.operators().at(i), term.operators().at(j));
    swaps++;
  };

  auto push_new_term = [&](size_t i, size_t j) {
    elms.clear();
    elms.reserve(term.operators().size() - 2);
    std::copy(term.operators().begin(), term.operators().begin() + j - 1,
              std::back_inserter(elms));
    std::copy(term.operators().begin() + j + 1, term.operators().end(),
              std::back_inserter(elms));
    stack.emplace_back(
        swaps % 2 == 0 ? term.coefficient() : -term.coefficient(),
        std::move(elms));
  };

  for (std::size_t i = 1; i < term.operators().size(); ++i) {
    for (std::size_t j = i; j > 0; --j) {
      Operator op1 = term.operators().at(j - 1);
      Operator op2 = term.operators().at(j);
      if (op1.type() == OperatorType::CREATION &&
          op2.type() == OperatorType::ANNIHILATION) {
        break;
      } else if (op1.type() == OperatorType::CREATION &&
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
  return swaps;
}

Expression normal_order(const Term& term) {
  std::vector<Term> stack = {term};
  Expression::ExpressionMap terms;
  while (!stack.empty()) {
    Term cur = stack.back();
    stack.pop_back();
    if (cur.operators().empty()) {
      terms[{}] += cur.coefficient();
      continue;
    }
    std::size_t swaps = sort_term(cur, stack);
    terms[cur.operators()] += (swaps % 2 == 0 ? 1 : -1) * cur.coefficient();
  }
  return Expression(terms);
}

Expression normal_order(const std::vector<Term>& terms) {
  Expression::ExpressionMap result;
  for (const Term& term : terms) {
    Expression e = normal_order(term);
    for (const auto& [term, coeff] : e.terms()) {
      result[term] += coeff;
    }
  }
  return Expression(result);
}

Expression normal_order(const Expression& expression) {
  Expression::ExpressionMap result;
  for (const auto& [term, coeff] : expression.terms()) {
    Expression e = normal_order(Term(coeff, term));
    for (const auto& [term, coeff] : e.terms()) {
      result[term] += coeff;
    }
  }
  return Expression(result);
}

Expression commute(const Term& term1, const Term& term2) {
  Term t1 = term1.product(term2);
  Term t2 = term2.product(term1);
  t2.coefficient() *= -1;
  return Expression(std::vector<Term>{t1, t2});
}
