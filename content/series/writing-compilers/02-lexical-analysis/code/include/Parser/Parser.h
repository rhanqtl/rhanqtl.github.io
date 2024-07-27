#pragma once

#include "AST/Node.h"

namespace xyz {

class Parser {
  virtual ast::Node *parse() = 0;
};

}  // namespace xyz
