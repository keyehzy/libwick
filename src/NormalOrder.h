#pragma once

#include "Expression.h"
#include "Term.h"

Expression normal_order(const Term& term);
Expression normal_order(const Expression& expression);
Expression commute(const Term& term1, const Term& term2);
Expression commute(const Expression& expression1, const Expression& expression2);
