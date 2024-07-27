#ifndef CXXNOTES_DERIVATOR_AST_H
#define CXXNOTES_DERIVATOR_AST_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Visitor;

class Expr {
  friend class ExprManager;

 public:
  virtual void accept(Visitor &) = 0;

 public:
  std::string to_str();
};

class Constant final : public Expr {
 public:
  Constant(uint64_t value);

 public:
  uint64_t value() const;

 public:
  void accept(Visitor &) override;

 private:
  uint64_t value_;
};

class Symbol final : public Expr {
 public:
  Symbol(const std::string &name) : name_{name} {}

  const std::string &name() const { return name_; }

 public:
  void accept(Visitor &) override;

 private:
  std::string name_;
};

class BinaryExpr final : public Expr {
 public:
  enum class Operator {
    ADD,
    MUL,
    POW,
  };

 public:
  BinaryExpr(Operator op, Expr &lhs, Expr &rhs);

 public:
  Operator opcode() const;
  Expr &left() const;
  Expr &right() const;

 public:
  void accept(Visitor &) override;

 private:
  Operator op_;
  Expr &lhs_;
  Expr &rhs_;
};

class ExprManager {
 public:
  static Constant &mk_constant(uint64_t value);
  static Symbol &mk_symbol(const std::string &name);
  static BinaryExpr &mk_add(Expr &lhs, Expr &rhs);
  static BinaryExpr &mk_mul(Expr &lhs, Expr &rhs);
  static BinaryExpr &mk_pow(Expr &lhs, Expr &rhs);

 private:
  static std::vector<std::unique_ptr<Symbol>> symbols_;
  static std::unordered_map<std::string, std::size_t> symbol_indexes_by_name_;

  static std::vector<std::unique_ptr<Constant>> constants_;
  static std::unordered_map<uint64_t, std::size_t> constant_indexes_by_value_;

  static std::vector<std::unique_ptr<Expr>> exprs_;
};

#endif  // CXXNOTES_DERIVATOR_AST_H
