#include <vector>
#include <cstddef>
#include <iostream>

#ifndef MAX_SLOTS
#  define MAX_SLOTS 1024
#endif

template <typename T/*, std::size_t N = 1024*/>
struct StackAllocator {
    using value_type = T;
    using reference = value_type&;
    using pointer = value_type*;
    using const_reference = const value_type&;

    // template <typename U>
    // struct rebind { using other = StackAllocator<T, N>; };
    static constexpr std::size_t N = MAX_SLOTS;

        static constexpr std::size_t kAlignedSlotSize = sizeof(T);

    std::byte mem [kAlignedSlotSize * N];
    std::byte *head = mem;

    pointer allocate(std::size_t n) {
      std::cout << N << ' ' << kAlignedSlotSize << ' ' << mem << '\n';
        const auto end = std::end(mem);
        if (end - head < n) {
            std::terminate();
        }
        auto *ans = head;
        head += n;
        std::cout << "allocated " << n << " slot(s)\n";
        return reinterpret_cast<pointer>(ans);
    }

    void deallocate(void *ptr, std::size_t) {
        std::cout << "deallocated " << ptr << '\n';
    }
};

int main() {
    std::vector<int, StackAllocator<int>> xs(StackAllocator<int>{});
    xs.push_back(1);
    xs.push_back(2);
    xs.push_back(3);
    xs.push_back(4);
}
