#include "Parser/Parser.h"

#include <cstdlib>

#include "Parser/SimpleParser.h"

namespace xyz {

ast::Node *SimpleParser::parse() {
  for (auto *f : files) {
    parse_translation_unit(f);
  }
}

void SimpleParser::parse_translation_unit() {
  while (true) {
    auto tok = peek_token();
    if (!tok->is_keyword()) {
      // report error
      std::abort();
    }

    if (tok->kind() == Token::Kind::KW_USE) {
      parse_use_decl();
    } else if (tok->kind() == Token::Kind::KW_FN) {
      parse_func_decl();
    } else {
      std::abort();
    }
  }
}

void SimpleParser::parse_use_decl() {
}

void SimpleParser::parse_func_decl() {
}

// void SimpleParser::parse_struct_decl() {
// }

// void SimpleParser::parse_trait_decl() {
// }

}  // namespace xyz
