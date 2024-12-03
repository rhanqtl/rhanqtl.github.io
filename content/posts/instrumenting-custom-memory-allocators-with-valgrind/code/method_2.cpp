#include <valgrind/valgrind.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>

class Allocator {
 private:
  static constexpr std::size_t kNumBlocks = 64;
  struct alignas(8) Block {
    bool used{false};
    uint8_t redzone1[7]{};
    uint8_t payload[64]{};
    uint8_t redzone2[7]{};
  };
  static_assert(sizeof(Block) == 80);

 public:
  void *allocate(std::size_t nbytes) {
    if (nbytes > 64) return nullptr;
    for (auto &raw_block : _memory) {
      if (!raw_block.used) {
        raw_block.used = true;
        auto *addr = reinterpret_cast<void *>(raw_block.payload);
        std::memset(addr, 0, nbytes);
        VALGRIND_MALLOCLIKE_BLOCK(addr, nbytes, 7, 1);
        return addr;
      }
    }
    return nullptr;
  }
  void deallocate(void *p) {
    if (!p) return;

    for (auto &raw_block : _memory) {
      if (p == reinterpret_cast<void *>(raw_block.payload)) {
        raw_block.used = false;
        VALGRIND_FREELIKE_BLOCK(p, 7);
        return;
      }
    }
    assert(false && "invalid pointer");
  }

 public:
  Block _memory[kNumBlocks]{};
};

int main() {
  {
    auto alloc = Allocator{};
    alloc.allocate(1);
    alloc.allocate(2);
    alloc.allocate(3);
    alloc.allocate(4);
  }

  std::cerr << "---\n";
  {
    auto alloc = Allocator{};
    auto *p = reinterpret_cast<char *>(alloc.allocate(10));
    *(p - 5) = 0x80;
    alloc.deallocate(p);
  }
  std::cerr << "---\n";

  {
    auto alloc = Allocator{};
    alloc.deallocate(reinterpret_cast<void *>(0x123456789));
  }
}
