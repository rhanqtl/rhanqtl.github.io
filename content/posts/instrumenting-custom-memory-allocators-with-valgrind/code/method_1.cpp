#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>

/// Scheme:
///   +--------+---------+--------+--------+---------+--------+-----+
///   | Header | Payload | Footer | Header | Payload | Footer | ... |
///   +--------+---------+--------+--------+---------+--------+-----+
///
class Allocator {
 private:
  static constexpr std::size_t kNumBlocks = 64;
  struct alignas(8) Block {
    bool used;
    uint8_t redzone1[7];
    uint8_t payload[64];
    uint8_t redzone2[8];
  };
  static_assert(sizeof(Block) == 80);

  enum BlockStatus {
    kOK = 0,
    kLeaked = 1,
    kCorrupted = 2,
  };

 public:
  Allocator() {
    for (auto &raw_block : _memory) {
      clear_block(raw_block);
    }
  }
  ~Allocator() {
    for (auto &raw_block : _memory) {
      if (raw_block.used)
        std::cerr << "Block " << static_cast<void *>(raw_block.payload)
                  << " is leaked\n";
    }
  }

 public:
  void *allocate(std::size_t nbytes) {
    if (nbytes > 64) return nullptr;
    for (auto &raw_block : _memory) {
      if (!raw_block.used) {
        raw_block.used = true;
        return reinterpret_cast<void *>(raw_block.payload);
      }
    }
    return nullptr;
  }
  void deallocate(void *p) {
    if (!p) return;

    for (auto &raw_block : _memory) {
      if (p == reinterpret_cast<void *>(raw_block.payload)) {
        if (std::any_of(std::begin(raw_block.redzone1),
                        std::end(raw_block.redzone1),
                        [](uint8_t byte) { return byte != 0xcc; }) ||
            std::any_of(std::begin(raw_block.redzone2),
                        std::end(raw_block.redzone2),
                        [](uint8_t byte) { return byte != 0xcd; })) {
          std::cerr << "Block " << static_cast<void *>(raw_block.payload)
                    << " is corrupted\n";
        }
        clear_block(raw_block);
        return;
      }
    }
    assert(false && "invalid pointer");
  }

 private:
  void clear_block(Block &raw_block) {
    raw_block.used = false;
    std::memset(reinterpret_cast<char *>(&raw_block.redzone1), 0xcc,
                sizeof(raw_block.redzone1));
    std::memset(reinterpret_cast<char *>(&raw_block.redzone2), 0xcd,
                sizeof(raw_block.redzone2));
    std::memset(reinterpret_cast<char *>(raw_block.payload), 0,
                sizeof(raw_block.payload));
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
