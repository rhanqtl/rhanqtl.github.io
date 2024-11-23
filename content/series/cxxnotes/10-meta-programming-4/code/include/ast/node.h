#pragma once

#include <memory>
#include <string>
#include <utility>

#include "reflect/model.h"

namespace ast {
struct Expr {
  enum class Kind {
    kLiteralExpr,
    kVarRefExpr,
    kAssignExpr,
    kUnaryExpr,
    kBinaryExpr
  };

  const Kind kind;

  Expr(Kind kind) : kind{kind} {}

  virtual ~Expr() = default;

  META_INFO(Expr, 0, void, kind);
};

struct LiteralExpr : public Expr {
  const double value;

  LiteralExpr(double value) : Expr{Kind::kLiteralExpr}, value{value} {}

  META_INFO(LiteralExpr, Kind::kLiteralExpr, Expr, value);
};

struct VarRefExpr : public Expr {
  const std::string name;

  VarRefExpr(std::string_view name) : Expr{Kind::kVarRefExpr}, name{name} {}

  META_INFO(VarRefExpr, Kind::kVarRefExpr, Expr, name);
};

struct AssignExpr : public Expr {
  const std::string name;
  const std::unique_ptr<Expr> value;

  AssignExpr(std::string_view name, std::unique_ptr<Expr> value)
      : Expr{Kind::kAssignExpr}, name{name}, value{std::move(value)} {}

  META_INFO(AssignExpr, Kind::kAssignExpr, Expr, name, value);
};

struct UnaryExpr : public Expr {
  enum class OpCode { kNegate };

  const OpCode op;
  const std::unique_ptr<Expr> arg;

  UnaryExpr(OpCode op, std::unique_ptr<Expr> arg)
      : Expr{Kind::kUnaryExpr}, op{op}, arg{std::move(arg)} {}

  META_INFO(UnaryExpr, Kind::kUnaryExpr, Expr, op, arg);
};

struct BinaryExpr : public Expr {
  enum class OpCode { kAdd, kSub, kMul, kDiv, kMod };

  const OpCode op;
  const std::unique_ptr<Expr> lhs;
  const std::unique_ptr<Expr> rhs;

  BinaryExpr(OpCode op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs)
      : Expr{Kind::kBinaryExpr},
        op{op},
        lhs{std::move(lhs)},
        rhs{std::move(rhs)} {}

  META_INFO(BinaryExpr, Kind::kBinaryExpr, Expr, op, lhs, rhs);
};

template <typename T, typename... Args>
inline std::unique_ptr<T> make(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}
}  // namespace ast
