#include <coroutine>
#include <format>
#include <iostream>

template <typename... Args>
void println(std::format_string<Args...> fmt, Args&&... args) {
  std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
}

struct Range {
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;

  struct promise_type {
    int i = -1;

    std::suspend_always initial_suspend() {
      println("in function {}, this={}", __PRETTY_FUNCTION__, (void*)this);
      return {};
    }
    std::suspend_always yield_value(int i) {
      println("in function {}, this={} i={}", __PRETTY_FUNCTION__, (void*)this,
              i);
      this->i = i;
      return {};
    }
    std::suspend_always final_suspend() noexcept {
      println("in function {}, this={}", __PRETTY_FUNCTION__, (void*)this);
      return {};
    }

    Range get_return_object() {
      println("in function {}, this={}", __PRETTY_FUNCTION__, (void*)this);
      return Range{handle_type::from_promise(*this)};
    }
    void unhandled_exception() { std::abort(); }
  };

  handle_type coro;

  Range(handle_type h) : coro{h} {
    println("enter function {}, this={}", __PRETTY_FUNCTION__, (void*)this);
    // coro.resume();
    println("leave function {}, this={}", __PRETTY_FUNCTION__, (void*)this);
  }

  mutable bool first = true;
  bool has_more() const {
    println("in function {}, this={}", __PRETTY_FUNCTION__, (void*)this);
    return !coro.done();
  }
  void next() { coro.resume(); }
  int get() const {
    if (first) {
      first = false;
      coro.resume();
    }
    return coro.promise().i;
  }

  // struct iterator {
  //   Range* r;

  //   iterator& operator++() {
  //     if (!r->coro.done()) {
  //       if (r) r->coro.resume();
  //     } else {
  //       r = nullptr;
  //     }
  //     return *this;
  //   }

  //   bool operator==(const iterator& other) const { return r == other.r; }
  //   bool operator!=(const iterator& other) const { return !operator==(other);
  //   }

  //   int operator*() const { return r->coro.promise().i; }
  // };

  // iterator begin() {
  //   println("in function begin, this={}", (void*)this);
  //   return {this};
  // }
  // iterator end() const {
  //   println("in function end, this={}", (void*)this);
  //   return {nullptr};
  // }
};

Range range(int end) {
  for (int begin = 0; begin != end; begin++) {
    println("in function range, yielding {}", begin);
    co_yield begin;
  }
}

int main() {
  println("entered function main");
  auto rng = range(10);
  for (; rng.has_more(); rng.next()) {
    auto i = rng.get();
    println("in function main, got {}", i);
  }
}
