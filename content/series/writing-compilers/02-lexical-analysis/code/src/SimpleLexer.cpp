#include "Lexer/SimpleLexer.h"

#include "Lexer/Token.h"
#include "Support/IO/InputStream.h"
#include "Support/IO/UTF8InputStream.h"

namespace xyz {

SimpleLexer::SimpleLexer(InputStream<Rune> &stream) : s_{stream} {
}

SimpleLexer::~SimpleLexer() {
}

Token *SimpleLexer::next_token() {
  while (auto res = next_char()) {
    auto ch = *res;
    switch (ch) {
      case '(':
        return Token::make_punctuation(Token::Kind::PUNCT_LPAREN);
      case ')':
        return Token::make_punctuation(Token::Kind::PUNCT_RPAREN);
      case '{':
        return Token::make_punctuation(Token::Kind::PUNCT_LBRACE);
      case '}':
        return Token::make_punctuation(Token::Kind::PUNCT_RBRACE);
      case '[':
        return Token::make_punctuation(Token::Kind::PUNCT_LBRACKET);
      case ']':
        return Token::make_punctuation(Token::Kind::PUNCT_RBRACKET);
      case ',':
        return Token::make_punctuation(Token::Kind::PUNCT_COMMA);
      case ':': {
        if (try_consume_char(':'))
          return Token::make_punctuation(Token::Kind::PUNCT_COLON_COLON);
        return Token::make_punctuation(Token::Kind::PUNCT_COLON);
      }
      case ';':
        return Token::make_punctuation(Token::Kind::PUNCT_SEMICOLON);

      case '+': {
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_ADD_ASSIGN);
        return Token::make_operator(Token::OP_ADD);
      }
      case '-':
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_SUB_ASSIGN);
        return Token::make_operator(Token::OP_SUB);
      case '*':
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_MUL_ASSIGN);
        return Token::make_operator(Token::OP_MUL);
      case '/':
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_DIV_ASSIGN);
        return Token::make_operator(Token::OP_DIV);
      case '%':
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_MOD_ASSIGN);
        return Token::make_operator(Token::OP_MOD);
      case '&':
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_BIT_AND_ASSIGN);
        return Token::make_operator(Token::OP_BIT_AND);
      case '|': {
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_BIT_OR_ASSIGN);
        return Token::make_operator(Token::OP_BIT_OR);
      }
      case '^': {
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_BIT_XOR_ASSIGN);
        return Token::make_operator(Token::OP_BIT_XOR);
      }
      case '~': {
        return Token::make_operator(Token::OP_BIT_NOT);
      }
      case '<': {
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_LE);
        if (try_consume_char('<'))
          return Token::make_operator(Token::OP_SHL);
        return Token::make_operator(Token::OP_LT);
      }
      case '>': {
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_GE);
        if (try_consume_char('>'))
          return Token::make_operator(Token::OP_SHR);
        return Token::make_operator(Token::OP_GT);
      }
      case '=': {
        if (try_consume_char('='))
          return Token::make_operator(Token::OP_EQ);
        if (try_consume_char('>'))
          return Token::make_operator(Token::OP_RIGHT_ARROW);
        return Token::make_operator(Token::OP_ASSIGN);
      }
      case '!': {
        consume_char('=');
        return Token::make_operator(Token::OP_NE);
      }
      case '.': {
        if (try_consume_chars({'.', '='}))
          return Token::make_operator(Token::OP_RANGE_INCL);
        if (try_consume_char('.'))
          return Token::make_operator(Token::OP_RANGE_EXCL);
        return Token::make_punctuation(Token::PUNCT_DOT);
      }

      case '\'': {
        auto str = collect_char_literal();
        return Token::make_char_literal(str);
      }
      case '"': {
        auto str = collect_string_literal();
        return Token::make_string_literal(std::move(str));
      }
      case '`': {
        auto str = collect_string_literal(true);
        return Token::make_string_literal(std::move(str));
      }

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9': {
        auto text = collect_number_literal(ch);
        if (ch == '0') {
          if (try_consume_char('x')) {
          }
          if (try_consume_char('o')) {
          }
          if (try_consume_char('b')) {
          }
          warn("number literals start with '0'");
        }
        return Token::make_integer_literal();
      }

      default: {
        auto id = collect_identifier(ch);
        if (auto *kw = Token::match_keyword(id))
          return kw;
        return Token::make_identifier(std::move(id));
      }
    }
  }
}

}  // namespace xyz
