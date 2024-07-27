#ifndef CXXNOTES_DERIVATOR_OPS_H
#define CXXNOTES_DERIVATOR_OPS_H

#include "AST.h"

Expr& operator+(Expr& lhs, Expr& rhs);
Expr& operator+(Expr& lhs, int rhs);
Expr& operator+(int lhs, Expr& rhs);

Expr& operator*(Expr& lhs, Expr& rhs);
Expr& operator*(Expr& lhs, int rhs);
Expr& operator*(int lhs, Expr& rhs);

Expr& operator^(Expr& lhs, Expr& rhs);
Expr& operator^(Expr& lhs, int rhs);
Expr& operator^(int lhs, Expr& rhs);

#endif  // CXXNOTES_DERIVATOR_OPS_H
