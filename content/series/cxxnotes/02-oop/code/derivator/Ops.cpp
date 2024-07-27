#include "Ops.h"

#include "AST.h"

Expr& operator+(Expr& lhs, Expr& rhs) { return ExprManager::mk_add(lhs, rhs); }
Expr& operator+(Expr& lhs, int rhs) {
  return lhs + ExprManager::mk_constant(rhs);
}
Expr& operator+(int lhs, Expr& rhs) {
  return ExprManager::mk_constant(lhs) + rhs;
}

Expr& operator*(Expr& lhs, Expr& rhs) { return ExprManager::mk_mul(lhs, rhs); }
Expr& operator*(Expr& lhs, int rhs) {
  return lhs * ExprManager::mk_constant(rhs);
}
Expr& operator*(int lhs, Expr& rhs) {
  return ExprManager::mk_constant(lhs) * rhs;
}

Expr& operator^(Expr& lhs, Expr& rhs) { return ExprManager::mk_pow(lhs, rhs); }
Expr& operator^(Expr& lhs, int rhs) {
  return lhs ^ ExprManager::mk_constant(rhs);
}
Expr& operator^(int lhs, Expr& rhs) {
  return ExprManager::mk_constant(lhs) ^ rhs;
}
