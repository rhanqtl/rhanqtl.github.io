#ifndef SUPPORT_MEMORY_CONTEXT_H
#define SUPPORT_MEMORY_CONTEXT_H

#include <cstddef>
namespace xyz {

class MemoryContext {
 public:
  void *allocate(std::size_t nbytes);
  void deallocate(void *ptr);
};

}  // namespace xyz

#endif  // SUPPORT_MEMORY_CONTEXT_H
