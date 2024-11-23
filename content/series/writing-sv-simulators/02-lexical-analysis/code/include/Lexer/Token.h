#ifndef TOKEN_H
#define TOKEN_H

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

#include "Support/Location/Location.h"

namespace xyz {

class Token final {
 private:
  static constexpr const int __punctuation_begin = 1;
  static constexpr const int __operator_begin = 1001;
  static constexpr const int __keyword_begin = 2001;
  static constexpr const int __literal_begin = 3001;

 public:
  enum Kind {
    INVALID = 0,

    PUNCT_LPAREN = __punctuation_begin,
    PUNCT_RPAREN,
    PUNCT_LBRACKET,
    PUNCT_RBRACKET,
    PUNCT_LBRACE,
    PUNCT_RBRACE,
    PUNCT_DOT,
    PUNCT_SEMICOLON,
    PUNCT_COLON,
    PUNCT_COMMA,
    PUNCT_COLON_COLON,  // ::
    __punctuation_end,

    OP_ADD = __operator_begin,
    OP_ADD_ASSIGN,
    OP_SUB,
    OP_SUB_ASSIGN,
    OP_MUL,
    OP_MUL_ASSIGN,
    OP_DIV,
    OP_DIV_ASSIGN,
    OP_MOD,
    OP_MOD_ASSIGN,
    OP_EQ,
    OP_NE,
    OP_LT,
    OP_LE,
    OP_GT,
    OP_GE,
    OP_BIT_AND,         // &
    OP_BIT_AND_ASSIGN,  // &=
    OP_BIT_OR,          // |
    OP_BIT_OR_ASSIGN,   // |=
    OP_BIT_XOR,         // ^
    OP_BIT_XOR_ASSIGN,  // ^=
    OP_BIT_NOT,         // ~
    OP_SHL,
    OP_SHR,
    OP_ASSIGN,
    OP_RIGHT_ARROW,    // =>
    OP_RANGE_EXCL,     // ..
    OP_RANGE_INCL,     // ..=
    OP_QUESTION_MARK,  // ?
    __operator_end,

    KW_IF = __keyword_begin,  // if
    KW_ELSE,                  // else
    KW_MATCH,                 // match
    KW_FN,                    // fn
    KW_FOR,                   // for
    KW_WHILE,                 // while
    KW_IMPL,                  // impl
    KW_TRAIT,                 // trait
    KW_USE,                   // use
    __keyword_end,

    LIT_IDENT = __literal_begin,  // identifiers
    LIT_INT,
    LIT_FLOAT,
    LIT_STRING,
    __literal_end,
  };

 private:
  // For caches only, there may be holes.
  Token() = default;
  explicit Token(Kind kind) : kind_{kind} {
  }
  template <typename T>
  Token(Kind kind, T&& data) : kind_{kind} {
    new (storage_) T{std::forward<T>(data)};
  }

 public:
  static Token* make_operator(Kind);
  static Token* make_punctuation(Kind);
  static Token* make_keyword(Kind);
  static Token* match_keyword(std::string_view id);
  static Token* make_integer_literal();
  static Token* make_floating_point_literal();
  static Token* make_char_literal(Rune);
  static Token* make_string_literal(std::string);
  static Token* make_identifier(std::string_view name);

 public:
  Kind kind() const;
  bool is_keyword() const;
  bool is_literal() const;

  const Location& location() const;

  std::string to_string() const;

 private:
  static Token keyword_cache[__keyword_end - __keyword_begin];
  static Token operator_cache[__operator_end - __operator_begin];
  static Token punctuation_cache[__punctuation_end - __punctuation_begin];

 private:
  const Kind kind_;
  const Location loc_;
  uint8_t storage_[0];
};

}  // namespace xyz

#endif  // TOKEN_H
