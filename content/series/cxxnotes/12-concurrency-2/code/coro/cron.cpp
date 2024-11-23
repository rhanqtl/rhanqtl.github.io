#include <coroutine>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>

class Scheduler {
 private:
  struct Task {
    std::coroutine_handle<> coro;
    std::function<void()> cb;
  };

 private:
  std::multimap<uint64_t, Task> _queue;

 public:
  auto delay(uint64_t time_step, std::function<void()> task) {
    struct Awaitable {
      uint64_t time_step;
      decltype(_queue) &q;
      std::function<void()> task;

      bool await_ready() const noexcept { return false; }
      void await_suspend(std::coroutine_handle<> h) {
        q.emplace(time_step, Task{h, task});
      }
      void await_resume() const noexcept {}
    };
    return Awaitable{time_step, _queue, std::move(task)};
  }

  void run() {
    for (uint64_t t = 0; !_queue.empty(); t++) {
      auto [first, last] = _queue.equal_range(t);
      if (first == _queue.end()) continue;
      for (auto it = first; it != last; ++it) {
        auto &task = it->second;
        if (task.cb) task.cb();
        task.coro();
      }
      _queue.erase(first, last);
    }
  }
};

struct Coroutine {
  struct promise_type {
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    Coroutine get_return_object() { return {}; }
    void unhandled_exception() { std::abort(); }
    void return_void() const noexcept {}
  };
};

template <uint64_t N>
Coroutine f(Scheduler &sched) {
  co_await sched.delay(N, [] { std::cout << N << '\n'; });
}

int main() {
  Scheduler sched;
  f<5>(sched);
  f<1>(sched);
  f<3>(sched);
  sched.run();
}
