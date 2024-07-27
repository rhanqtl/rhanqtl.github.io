#include "PrettyPrinter.h"

#include "AST.h"

PrettyPrinter::PrettyPrinter(std::ostream &os) : os_{os} {}

void PrettyPrinter::visit(Constant &c) { os_ << c.value(); }

void PrettyPrinter::visit(Symbol &sym) { os_ << sym.name(); }

void PrettyPrinter::visit(BinaryExpr &expr) {
  auto print_opr = [op = expr.opcode(), &os = os_] {
    using E = BinaryExpr::Operator;
    switch (op) {
      case E::ADD:
        os << u'＋';
        break;
      case E::MUL:
        os << '\0';
        break;
      case E::POW:
        os << L'²';
        break;
    }
  };
  os_ << '(';
  expr.left().accept(*this);
  print_opr();
  expr.right().accept(*this);
  os_ << ')';
}
