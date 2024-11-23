#pragma once

#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace cxxnotes {

class FormatArgumentShell {
 public:
  virtual void print_to(std::string &res) = 0;
};

template <typename T>
class FormatArgument : public FormatArgumentShell {
 public:
  FormatArgument(T &&x) : data_{std::forward<T>(x)} {
  }

  void print_to(std::string &res) override {
    std::ostringstream oss;
    oss << data_;
    res.append(oss.str());
  }

 private:
  T data_;
};

template <typename H, typename... T>
inline void fill_format_args(std::vector<FormatArgumentShell *> &res, H &&head,
                             T &&...tail) {
  res.push_back(new FormatArgument<H>(std::forward<H>(head)));
  fill_format_args(res, tail...);
}

inline void fill_format_args(std::vector<FormatArgumentShell *> &res) {
}

struct FormatSpec {
  const std::optional<std::size_t> index;
};

class Formatter {
 public:
  Formatter(std::string_view fmt, std::vector<FormatArgumentShell *> args)
      : fmt{fmt}, args_{std::move(args)} {
  }

 public:
  std::string format() {
    std::string ans;
    while (i_ < fmt.length()) {
      char c = fmt[i_];
      if (try_consume_seq({'{', '{'})) {
        ans.push_back('{');
      } else if (try_consume_seq({'}', '}'})) {
        ans.push_back('}');
      } else if (try_consume('{')) {
        auto spec = parse_format_spec(fmt, i_);
        handle_spec(spec, ans);
        must_consume('}');
      } else {
        // Unmatched '}'s are treated as normal characters.
        ans.push_back(c);
        i_++;
      }
    }
    return ans;
  }

 private:
  FormatSpec parse_format_spec(std::string_view fmt, std::size_t &i) {
    assert(i < fmt.length());
    auto index_opt = parse_index();
    if (try_consume(':')) {
      // parse_others(fmt, i);
      std::abort();
    }
    return FormatSpec{
        .index = std::move(index_opt),
    };
  }

  std::optional<std::size_t> parse_index() {
    if (i_ >= fmt.length() || !std::isdigit(fmt[i_])) return std::nullopt;
    std::size_t ans = 0;
    while (i_ < fmt.length() && std::isdigit(fmt[i_])) {
      if (std::numeric_limits<decltype(ans)>::max() / 10 < ans)
        throw std::invalid_argument("overflow");
      auto inc = fmt[i_] - '0';
      if (std::numeric_limits<decltype(ans)>::max() % 10 < inc)
        throw std::invalid_argument("overflow");
      ans += inc;
      i_++;
    }
    std::cout << ans << '\n';
    return {ans};
  }

  void handle_spec(const FormatSpec &spec, std::string &ans) {
    auto index = curr_index;
    if (spec.index.has_value()) {
      index = spec.index.value();
    } else {
      curr_index++;
    }
    args_[index]->print_to(ans);
  }

  bool try_consume_seq(std::initializer_list<char> cs) {
    auto it = cs.begin();
    for (; i_ < fmt.length() && it != cs.end(); it++, i_++) {
      if (fmt[i_] != *it) break;
    }
    const auto ans = it == cs.end();
    if (!ans) i_ -= std::distance(cs.begin(), it);
    return ans;
  }
  bool try_consume(char c) {
    return try_consume_seq({c});
  }

  void must_consume(char c) {
    if (i_ >= fmt.length() || fmt[i_] != c) std::abort();
    i_++;
  }

 private:
  std::string_view fmt;
  std::size_t i_{0};
  std::vector<FormatArgumentShell *> args_;
  std::size_t curr_index{0};
};

/// format_str  : components
/// components  : components component
/// component   : character
///             | format_spec
/// character   : plain_character
///             | "{{"
///             | "}}"
/// format_spec : "{" "}"
///             | "{" index "}"
///             | "{" ":" spec "}"
///             | "{" index ":" spec "}"
/// index       : INTEGER
/// spec        : error
template <typename... Args>
std::string format(std::string_view fmt, Args &&...args0) {
  std::vector<FormatArgumentShell *> args;
  // args.reserve(sizeof(args0));
  fill_format_args(args, args0...);

  return Formatter{fmt, std::move(args)}.format();
}

}  // namespace cxxnotes
