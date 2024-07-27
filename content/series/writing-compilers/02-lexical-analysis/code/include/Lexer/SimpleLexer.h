#pragma once

#include <initializer_list>
#include <queue>

#include "Lexer/Lexer.h"
#include "Lexer/Token.h"
#include "Support/IO/InputStream.h"
#include "Support/IO/UTF8InputStream.h"
#include "Support/Utility/Expected.h"

namespace xyz {

class SimpleLexer : public Lexer {
 public:
  SimpleLexer(InputStream<Rune> &);
  ~SimpleLexer();

 public:
  Token *next_token();

 private:
  Expected<Rune> next_char();
  void consume_char(Rune);
  bool try_consume_char(Rune);
  bool try_consume_chars(std::initializer_list<Rune>);

  std::string collect_number_literal(Rune first_char);
  Rune collect_char_literal();
  std::string collect_string_literal(bool multi_line = false);
  std::string collect_identifier(Rune first_char);

 private:
  /// \addtogroup diagnostics
  ///@{
  void warn(std::string_view msg);
  ///@}

 private:
  InputStream<Rune> &s_;
  std::queue<Rune> buf_;
};
}  // namespace xyz
