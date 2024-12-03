#include <valgrind/memcheck.h>
#include <valgrind/valgrind.h>

#include <vector>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>

template <typename T>
class Pool {
 private:
  static constexpr std::size_t kNumSlots = 64;
  struct alignas(alignof(T)) Slot {
    uint8_t payload[sizeof(T)];
    bool used{false};
    uint8_t _pad[alignof(T) - sizeof(used)];
  };

 public:
  Pool() { VALGRIND_CREATE_MEMPOOL(this, 0, 1); }
  ~Pool() { /* VALGRIND_DESTROY_MEMPOOL(this);*/ }

  T *allocate() {
    for (auto &slot : _memory) {
      if (!slot.used) {
        slot.used = true;
        auto *addr = reinterpret_cast<T *>(slot.payload);
        std::memset(addr, 0, sizeof(T));
        VALGRIND_MEMPOOL_ALLOC(this, addr, sizeof(T));
        return addr;
      }
    }
    return nullptr;
  }
  void deallocate(T *p) {
    if (!p) return;

    for (auto &slot : _memory) {
      if (p == reinterpret_cast<void *>(slot.payload)) {
        slot.used = false;
        VALGRIND_MEMPOOL_FREE(this, p);
        return;
      }
    }
    assert(false && "invalid pointer");
  }

 public:
  Slot _memory[kNumSlots];
};

int main() {
  {
    auto alloc = Pool<int>{};
    alloc.allocate();
    alloc.allocate();
    alloc.allocate();
    alloc.allocate();
  }

  {
    auto alloc = Pool<std::vector<int>>{};
    alloc.allocate();
    alloc.allocate();
  }
}
