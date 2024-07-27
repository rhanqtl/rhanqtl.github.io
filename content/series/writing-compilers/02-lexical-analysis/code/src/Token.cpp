#include "Lexer/Token.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>

#include "Support/Utility/MemoryContext.h"
#include "Support/Utility/TypeTraits.h"

namespace xyz {

static MemoryContext ctx;

Token Token::keyword_cache[] = {
    Token{KW_IF},  Token{KW_ELSE}, Token{KW_MATCH}, Token{KW_FN},
    Token{KW_FOR}, Token{KW_IMPL}, Token{KW_TRAIT},
};

Token *Token::make_punctuation(Kind kind) {
  return &punctuation_cache[kind - __punctuation_begin];
}

Token *Token::make_keyword(Kind kind) {
  return &keyword_cache[kind - __keyword_begin];
}

Token *Token::make_operator(Kind kind) {
  return &operator_cache[kind - __operator_begin];
}

Token *Token::match_keyword(std::string_view id) {
  if (id.length() < 2)
    return nullptr;

  switch (id[0]) {
    case 'e': {
      if (std::strcmp(id.data(), "else"))
        return KW_ELSE;
      break;
    }
    case 'f': {
      if (std::strcmp(id.data(), KW_FUNC.to_string()))
        return KW_FUNC;
    }
    case 'i': {
      if (id[1] == 'f' && id.length() == 2)
        return KW_IF;
      if (id[1] == 'm' &&)
        return KW_IMPORT;
      break;
    }
    case 'm': {
      if (id.length() == 5 && std::strcmp(id.substr(1).data(), "atch"))
        return KW_MATCH;
      break;
    }
  }

  return nullptr;
}

Token *Token::make_identifier(const std::string &name) {
  void *mem = ctx.allocate(sizeof(Token) + sizeof(std::string));
  return new (mem) Token{LIT_IDENT, name};
}

Token::Kind Token::kind() const {
  return kind_;
}

bool Token::is_keyword() const {
  return __keyword_begin <= kind() && kind() < __keyword_end;
}

bool Token::is_literal() const {
  return __literal_begin <= kind() && kind() < __literal_end;
}

}  // namespace xyz
