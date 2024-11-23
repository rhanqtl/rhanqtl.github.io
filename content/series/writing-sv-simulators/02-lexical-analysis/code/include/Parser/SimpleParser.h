#pragma once

#include "AST/Node.h"
#include "Parser/Parser.h"

namespace xyz {

class SimpleParser final : public Parser {
 public:
  ast::Node *parse() override;
};

}  // namespace xyz
