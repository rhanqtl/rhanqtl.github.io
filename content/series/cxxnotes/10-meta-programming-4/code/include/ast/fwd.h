#pragma once

#include <tuple>
namespace ast {
struct Expr;
struct LiteralExpr;
struct VarRefExpr;
struct AssignExpr;
struct UnaryExpr;
struct BinaryExpr;

using Nodes =
    std::tuple<LiteralExpr, VarRefExpr, AssignExpr, UnaryExpr, BinaryExpr>;
}  // namespace ast
