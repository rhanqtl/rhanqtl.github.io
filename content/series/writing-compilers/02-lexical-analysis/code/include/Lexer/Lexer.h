#ifndef LEXER_H
#define LEXER_H

#include "Lexer/Token.h"

namespace xyz {

class Lexer {
 public:
  virtual ~Lexer();

 public:
  Token next_token();
};

}  // namespace xyz

#endif  // LEXER_H
