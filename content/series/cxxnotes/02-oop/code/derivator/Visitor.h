#ifndef CXXNOTES_DERIVATOR_VISITOR_H
#define CXXNOTES_DERIVATOR_VISITOR_H

#include "AST.h"

class Visitor {
 public:
  virtual void visit(Constant &);
  virtual void visit(Symbol &);
  virtual void visit(BinaryExpr &);
};

#endif  // CXXNOTES_DERIVATOR_VISITOR_H
