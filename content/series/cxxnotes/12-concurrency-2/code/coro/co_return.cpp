#include <coroutine>
#include <cstdlib>
#include <format>
#include <functional>
#include <iostream>

class Lazy {
 public:
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  struct promise_type {
    std::function<int()> func;

    std::suspend_always initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    Lazy get_return_object() { return Lazy{handle_type::from_promise(*this)}; }
    void unhandled_exception() { std::abort(); }

    void return_value(std::function<int()> func) { this->func = func; }
  };

 public:
  Lazy(handle_type h) : _coro{h} {}

 public:
  int &get() const {
    if (!_eval) {
      _value = _coro.promise().func();
      _eval = true;
    }
    return _value;
  }

 private:
  handle_type _coro;
  mutable bool _eval{false};
  mutable int _value;
};

Lazy lazy(std::function<int()> func) { co_return func; }

int main() {
  auto delayed = lazy([] {
    std::cout << __PRETTY_FUNCTION__ << '\n';
    return 42;
  });
  std::cout << __PRETTY_FUNCTION__ << '\n';
  std::cout << std::format("got {}", delayed.get()) << '\n';
}
