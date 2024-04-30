#pragma once

#include "Expression.h"
#include "Term.h"

Expression normalOrder(const Term& term);
Expression normalOrder(const Expression& expression);
Expression commute(const Term& term1, const Term& term2);
Expression commute(const Expression& expression1, const Expression& expression2);
