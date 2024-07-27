#ifndef CXXNOTES_DERIVATOR_PP_H
#define CXXNOTES_DERIVATOR_PP_H

#include <ostream>

#include "AST.h"
#include "Visitor.h"

class PrettyPrinter : public Visitor {
 public:
  PrettyPrinter(std::ostream &os);

 public:
  void visit(Constant &) override;
  void visit(Symbol &) override;
  void visit(BinaryExpr &) override;

 private:
  std::ostream &os_;
};

#endif  // CXXNOTES_DERIVATOR_PP_H
