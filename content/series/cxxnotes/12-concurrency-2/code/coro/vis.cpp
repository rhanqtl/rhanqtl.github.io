#include <coroutine>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <stack>
#include <string>

struct Expr {
  enum class Kind {
    kLiteral,
    kVar,
    kUnary,
    kBinary,
  };

  const Kind kind;

  Expr(Kind k) : kind{k} {}

  virtual ~Expr() = default;
};

std::ostream &operator<<(std::ostream &outs, Expr::Kind k) {
  switch (k) {
    case Expr::Kind::kLiteral:
      outs << "LITERAL";
      break;
    case Expr::Kind::kVar:
      outs << "VAR";
      break;
    case Expr::Kind::kUnary:
      outs << "UNARY";
      break;
    case Expr::Kind::kBinary:
      outs << "BINARY";
      break;
  }
  return outs;
}

struct Literal final : Expr {
  const int value;

  Literal(int v) : Expr{Kind::kLiteral}, value{v} {}
};

struct Var final : Expr {
  const std::string name;

  Var(std::string_view name) : Expr{Kind::kVar}, name{name} {}
};

struct Unary final : Expr {
  enum Operator {
    kNegate,
    kLogicalNot,
  };

  const Operator op;
  const std::unique_ptr<Expr> arg;

  Unary(Operator op, std::unique_ptr<Expr> arg)
      : Expr{Kind::kUnary}, op{op}, arg{std::move(arg)} {}
};

struct Binary final : Expr {
  enum Operator {
    kAdd,
    kSub,
    kMul,
    kDiv,
  };

  const Operator op;
  std::unique_ptr<Expr> left;
  std::unique_ptr<Expr> right;

  Binary(Operator op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
      : Expr{Kind::kBinary},
        op{op},
        left{std::move(left)},
        right{std::move(right)} {}
};

struct Iterator {
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  struct promise_type {
    // std::reference_wrapper<Expr> value;

    Iterator get_return_object() {
      return Iterator{handle_type::from_promise(*this)};
    }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_always yield_value(Expr &expr) {
      value = expr;
      return {};
    }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { std::abort(); }
  };

  handle_type coro;

  bool has_more() const { return !coro.done(); }
  void next() { coro.resume(); }
  Expr &get() const { return coro.promise().value.get(); }
};

Iterator iterate(Expr &ast) {
  std::stack<std::reference_wrapper<Expr>> st;
  st.emplace(ast);
  while (!st.empty()) {
    auto u = st.top();
    st.pop();
    co_yield u;

    switch (u.get().kind) {
      case Expr::Kind::kLiteral:
        break;
      case Expr::Kind::kVar:
        break;
      case Expr::Kind::kUnary: {
        auto &unary = static_cast<Unary &>(u.get());
        st.emplace(*unary.arg);
        break;
      }
      case Expr::Kind::kBinary: {
        auto &binary = static_cast<Binary &>(u.get());
        st.emplace(*binary.right);
        st.emplace(*binary.left);
        break;
      }
    }
  }
}

std::unique_ptr<Expr> L(int value) { return std::make_unique<Literal>(value); }
std::unique_ptr<Expr> V(std::string_view name) {
  return std::make_unique<Var>(name);
}
std::unique_ptr<Expr> U(Unary::Operator op, std::unique_ptr<Expr> arg) {
  return std::make_unique<Unary>(op, std::move(arg));
}
std::unique_ptr<Expr> B(Binary::Operator op, std::unique_ptr<Expr> left,
                        std::unique_ptr<Expr> right) {
  return std::make_unique<Binary>(op, std::move(left), std::move(right));
}

int main() {
  auto ast =
      B(Binary::Operator::kMul, V("a"), U(Unary::Operator::kNegate, L(-2)));
  auto iter = iterate(*ast);
  while (iter.has_more()) {
    std::cout << iter.get().kind << '\n';
    iter.next();
  }
}
